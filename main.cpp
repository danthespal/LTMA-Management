#include <iostream>     // for basic I/O operations e.g. cin, cout, etc.
#include <fstream>      // for data file handling
#include <iomanip>      // manipulates floating point numbers and also manipulates console I/O
#include <string.h>     // for strcmp(), strcpy() and other string methods
#include <stdio.h>      // for remove() and rename()
#include "Header.h"     // contains utils required for proper I/O

// we don't need to add 'std::' standard library methods
using namespace std;

// declaring name length constant
const short NAME_LENGTH = 29;

// everything which a member of school have in common is declared here
class SchoolMember
{
protected:
    char name[NAME_LENGTH]{};
    int id;

public:
    SchoolMember()
    {
        id = 0;
    }

    int getId() const { return id; }
};

// using function template to remove contents of student, teacher or staff
template <typename T>
void removeMember(const string filePath, const string tempFilePath, const T schoolMember)
{
    system("cls");
    int inputId = 0;
    short flag = 0;
    ifstream fileToRead(filePath, ios::binary);
    ofstream fileToWrite(tempFilePath, ios::binary | ios::app);
    fileToRead.seekg(0);
    cout << "Enter ID of person whose data has to be removed: ";
    cin >> inputId;
    while (fileToRead.read((char*)&schoolMember, sizeof(schoolMember)))
    {
        if (inputId == schoolMember.getId())
        {
            flag++;
        }
        else
        {
            fileToWrite.write((char*)&schoolMember, sizeof(schoolMember));
        }
    }
    if (flag == 0)
    {
        cout << "Sorry, No Match found.";
    }
    else
    {
        cout << "Data of ID " << inputId << " has been removed from file.";
    }
    fileToRead.close();
    fileToWrite.close();
    remove(filePath.c_str());
    if (rename(tempFilePath.c_str(), filePath.c_str()))
        return;
    basicNavigation();
}

// using function template to add contents of student, teacher or staff
template <typename T>
void addMember(const string filePath, T schoolMember)
{
    fstream file(filePath, ios::in | ios::oct | ios::app | ios::binary);
    schoolMember.inputDetails();
    file.write((char*)&schoolMember, sizeof(schoolMember));
    file.close();
    basicNavigation();
}

// allow users to view student, teacher or staff data records
template <typename T>
void viewData(const string filePath, const T schoolMember)
{
    system("cls");
    ifstream fileToRead(filePath, ios::binary);
    int inputId = 0;
    short flag = 0;
    cout << "Enter ID of person whose data records you want to see: ";
    cin >> inputId;
    while (fileToRead.read((char*)&schoolMember, sizeof(schoolMember)))
    {
        if (inputId == schoolMember.getId())
        {
            ++flag;
            break;
        }
    }
    if (flag > 0)
    {
        schoolMember.displayData();
    }
    else
    {
        cout << "No match found.";
    }
    basicNavigation();
}

// allow users to update student, teacher or staff data records
// ask for id or roll number. proceed to next step only if roll number exist in passed 'filePath'
// if roll number exist, call 'updateData()' and write to file
template <typename T>
void updateMemberData(const string filePath, T schoolMember)
{
    system("cls");
    int inputId = 0;
    short flag = 0;
    streamoff filePosition = 0;
    fstream file(filePath, ios::in | ios::out | ios::binary);
    if (file)
    {
        file.seekg(0);
        cout << "Enter ID or roll number of person whose data has to be updated: ";
        cin >> inputId;
        while (!file.eof())
        {
            filePosition = file.tellg();
            file.read((char*)&schoolMember, sizeof(schoolMember));
            if (inputId == schoolMember.getId())
            {
                ++flag;
                break;
            }
        }
        if (flag > 0)
        {
            schoolMember.updateData();
            file.seekg(filePosition);
            file.write((char*)&schoolMember, sizeof(schoolMember));
            cout << "\nFile has beed successfully update.";
        }
        else
        {
            cout << "Sorry, no match found.";
        }
    }
    else
    {
        cout << "\nOops! There's something wrong with file "
            << filePath << ". Please, make sure it exists.\n";
    }
    basicNavigation();
}

// pay salary to teacher or staff
// deduct the paid salary from remaining
// reflect back in respective data file
template <typename T>
void paySalaryToMember(const string filePath, T schoolMember)
{
    system("cls");
    int inputId = 0, salaryPaid = 0;
    streamoff filePosition = 0;
    short flag = 0;
    fstream file(filePath, ios::in | ios::out | ios::binary);
    file.seekg(0);
    cout << "Enter ID to pay salary: ";
    cin >> inputId;
    while (!file.eof())
    {
        filePosition = file.tellg();
        file.read((char*)&schoolMember, sizeof(schoolMember));
        if (inputId == schoolMember.getId())
        {
            ++flag;
            break;
        }
    }
    if (flag == 0)
    {
        cout << "Sorry, ID " << inputId << " not found.";
    }
    else if (schoolMember.getSalary() == 0)
    {
        cout << "Congratulations! Total annual salary has been paid to " << schoolMember.getId() << ".";
    }
    else
    {
        cout << "Remaining salary to be paid to " << schoolMember.getId() << " in RON " << schoolMember.getSalary() << ". Enter the salary to be paid (in RON): ";
        cin >> salaryPaid;
        while (salaryPaid > schoolMember.getSalary() || salaryPaid < 0)
        {
            cout << "You have entered invalid salary. Please enter a valid salary (in RON): ";
            cin >> salaryPaid;
        }
        schoolMember.deductSalary(salaryPaid);
        file.seekg(filePosition);
        file.write((char*)&schoolMember, sizeof(schoolMember));
        cout << "You have successfully piad " << salaryPaid << ". Now remaining salary for " << schoolMember.getId() << " is : RON" << schoolMember.getSalary();
    }
    file.close();
    basicNavigation();
}

// student class has all the data members and required member functions for students
class Student : public SchoolMember
{
private:
    int studentFee{};

protected:
    using SchoolMember::id;

public:
    using SchoolMember::name;
    short studentClass{};
    char motherName[NAME_LENGTH]{}, fatherName[NAME_LENGTH]{}, studentSection{};

    Student()
    {
        id = 1;
        studentFee = 0;
    }

    int getId() const { SchoolMember::getId(); return id; }
    void inputDetails();
    void updateData();
    void generateRollNumber();
    void displayData() const
    {
        displayNameInUpper(name, strlen(name));
        cout << "\nRoll Number: " << id << "\nClass: " << studentClass << " '" << char(toupper(studentSection)) << "' "
            << "\nMother's name: " << motherName << "\nFather's name: " << fatherName << "\nRemaining fee: RON." << studentFee;
    }
    void deductFee(int amountPiad) { studentFee = studentFee - amountPiad; }
    int getStudentFee() const { return studentFee; }
};

// generate roll number by using class, section and first five characters of name
// student roll number format is explained below:
// first two digits is class, third digit is section and last two are alphabetical position of student in his/her class
// for e.g. 12252 is equivalent to class = 12th, section = 'B', alphabetical position = 52
void Student::generateRollNumber()
{
    Student schoolStudentTemp;
    fstream studentFile("data/student.dat", ios::in | ios::out | ios::app | ios::binary);
    short flag = 0;
    while (studentFile.read((char*)&schoolStudentTemp, sizeof(schoolStudentTemp)))
    {
        if (studentClass == schoolStudentTemp.studentClass && studentSection == schoolStudentTemp.studentSection)
        {
            ++flag;
            sanitizeCharAray(name, NAME_LENGTH);
            sanitizeCharAray(name, NAME_LENGTH);
            if (strcmp(name, schoolStudentTemp.name) == 0)
            {
                id = schoolStudentTemp.id + 1;
                break;
            }
            else
            {
                for (short i = 0; i < 5; ++i)
                {
                    if (schoolStudentTemp.name[i] > name[i])
                    {
                        schoolStudentTemp.id++;
                        id = schoolStudentTemp.id - 1;
                        break;
                    }
                    else if (schoolStudentTemp.name[i] < name[i])
                    {
                        id = schoolStudentTemp.id + 1;
                        break;
                    }
                    else
                    {
                        continue;
                    }
                }
            }
        }
    }
    if (flag == 0)
    {
        id = (studentClass * 1000) + (studentSection - 96) * 100 + id;
    }
}

void Student::inputDetails()
{
    system("cls");
    cout << "Enter student name (max. 28 characters): ";
    cin.ignore(); // fixes skipping of input; this must be used when 'cin' preceds 'getline()' or cin.get()
    gets_s(name);
    cout << "Enter the class (1 to 12): ";
    cin >> studentClass;
    cout << "Enter the section ('A' to 'D'): ";
    cin >> studentSection;
    studentSection = tolower(studentSection);
    cout << "Enter annual fee student needs to pay (in RON): ";
    cin >> studentFee;
    cout << "Enter student father's name (max. 28 characters): ";
    cin.ignore();
    gets_s(fatherName);
    cout << "Enter student mother's name (max. 28 characters): ";
    gets_s(motherName);
    generateRollNumber();
    cout << "\nGenerated roll number is " << getId() << ". Please, note it safely as it'll be asked during data modification.";
    basicNavigation();
}

// receives student fee and deducts from the remaining
// reflects the same in data file
void receiveStudentFee()
{
    system("cls");
    Student schoolStudent;
    int inputRollNumber = 0, amountToPay = 0;
    short flag = 0;
    streamoff filePosition = 0;
    fstream studentFile("data/student.dat", ios::in | ios::out | ios::binary);
    cout << "Enter the roll number of student whose fees you want to receive: ";
    cin >> inputRollNumber;
    while (!studentFile.eof())
    {
        filePosition = studentFile.tellg();
        studentFile.read((char*)&schoolStudent, sizeof(schoolStudent));
        if (inputRollNumber == schoolStudent.getId())
        {
            flag++;
            break;
        }
    }
    if (flag == 0)
    {
        cout << "Sorry, roll number not found.";
    }
    else if (schoolStudent.getStudentFee() == 0)
    {
        cout << "Congratulations! All dues are clear for student " << schoolStudent.getId() << ".";
    }
    else
    {
        cout << "Remaining fees to be paid by student " << schoolStudent.getId() << " is RON" << schoolStudent.getStudentFee() << ". Enter fee you want to receive (in RON): ";
        cin >> amountToPay;
        while (amountToPay > schoolStudent.getStudentFee() || amountToPay < 0)
        {
            cout << "Sorry, we received an invalid fee. Please enter again (in RON): ";
            cin >> amountToPay;
        }
        schoolStudent.deductFee(amountToPay);
        studentFile.seekg(filePosition);
        studentFile.write((char *)&schoolStudent, sizeof(schoolStudent));
        cout << "We have successfully received " << amountToPay << ". Now remaining fee for student " << schoolStudent.getId() << " is: " << schoolStudent.getStudentFee();
    }
    studentFile.close();
    basicNavigation();
}

// display student data records storend in student.dat file
void Student::updateData()
{
    char userChoice = ' ';
    cout << "Press '+' to update student name or press 'Enter' key to retain old data: ";
    cin.ignore();
    cin.get(userChoice);
    if (userChoice == '+' && userChoice != 10) // 10 is ASCII value for enter key
    {
        cout << "Enter new name (max. 28 characters)";
        cin.ignore();
        gets_s(name);
    }
    cout << "\nPress '+' to update student class or press 'Enter' key to retain old data: ";
    cin.get(userChoice);
    if (userChoice == '+' && userChoice != 10)
    {
        cout << "Enter new class (1 - 12): ";
        cin.ignore();
        cin >> studentClass;
        cin.ignore();
    }
    cout << "\nPress '+' to update student section or press 'Enter' key to retain old data: ";
    cin.get(userChoice);
    if (userChoice == '+' && userChoice != 10)
    {
        cout << "Enter new section ('A' to 'D'): ";
        cin.ignore();
        cin >> studentSection;
        cin.ignore();
    }
    cout << "\nPress '+' to update student annual fee or press 'Enter' key to retain old data: ";
    cin.get(userChoice);
    if (userChoice == '+' && userChoice != 10)
    {
        cout << "Enter new annual feww (in RON): ";
        cin.ignore();
        cin >> studentFee;
        cin.ignore();
    }
    cout << "\nPress '+' to update student father's name or press 'Enter' key to retain old data: ";
    cin.get(userChoice);
    if (userChoice == '+' && userChoice != 10)
    {
        cout << "Enter father's name (max. 28 characters): ";
        cin.ignore();
        gets_s(fatherName);
    }
    cout << "\nPress '+' to update student mother's name or press 'Enter' key to retain old data: ";
    cin.get(userChoice);
    if (userChoice == '+' && userChoice != 10)
    {
        cout << "Enter mother's name (max. 28 characters): ";
        cin.ignore();
        gets_s(motherName);
    }
    generateRollNumber();
    cout << "\nGenerated roll number is " << getId() << ". Please, note it safely as it'll be asked during data modification.";
}

class Teacher : public SchoolMember
{
private:
    char teacherSubject[17]{}, teacherQualification[9]{};
    short teacherExperience{}, teacherClass{}, teacherSubjectCode{};
    int teacherSalary{};

public:
    Teacher()
    {
        teacherSubjectCode = 0;
        id = 1;
    }
    void inputDetails();
    void generateTeacherId();
    void updateData();
    void deductSalary(int salaryPaid) { teacherSalary = teacherSalary - salaryPaid; }
    void displayData() const
    {
        displayNameInUpper(name, strlen(name));
        cout << "\nID: " << id << "\nTeacher Qualification: " << teacherQualification << "\nExperience: " << teacherExperience << " years"
            << "\nClass taught: " << teacherClass << "\nSubject taught: " << teacherSubject
            << "\nSalary to be paid: RON" << teacherSalary;
    }
    int getSalary() const { return teacherSalary; }
};

// generates id in format: first three digist are subject code of subject the teacher teaches and the last two digits are
// incremented when same subject set of teacher is found. for e.g. 12305 indicates that teacher teach subject codes 1, 2 & 3 and
// is the 5th teacher to teache the same three subjects in school
void Teacher::generateTeacherId()
{
    Teacher schoolTeacherRead;
    ifstream teacherFile("data/teacher.dat", ios::binary);
    short flag = 0;
    while (teacherFile.read((char*)&schoolTeacherRead, sizeof(schoolTeacherRead)))
    {
        if (teacherSubjectCode == schoolTeacherRead.teacherSubjectCode && teacherClass == schoolTeacherRead.teacherClass)
        {
            ++flag;
            id = schoolTeacherRead.id + 1;
        }
    }
    if (flag == 0) // de verificat
    {
        id = (teacherClass * 1000) + (teacherSubjectCode * 100) + id;
    }
}

void Teacher::inputDetails()
{
    system("cls");
    cout << "Enter teacher name (max. 28 characters): ";
    cin.ignore();
    gets_s(name);
    cout << "Enter the class to be taught (1 to 12): ";
    cin >> teacherClass;
    cout << "\nSubject Code\tSubject\n     1\t\tScience\n     2\t\tMaths\n     3\t\tEnglish\n     4\t\tHindi\n     5\t\tSocial Studies";
    cout << "\nEnter the subject code from the above list to choose subject the teacher will teach, for e.g. Press '1' to choose 'Science': ";
    cin >> teacherSubjectCode;
    while (teacherSubjectCode > 5 || teacherSubjectCode < 1)
    {
        cout << "\nSorry, we received a wrong subject code. Please enter valid subject code viz. 1, 2, 3, 4 or 5: ";
        cin >> teacherSubjectCode;
    }
    switch (teacherSubjectCode)
    {
    case 1:
        strcpy_s(teacherSubject, "Science");
        break;
    case 2:
        strcpy_s(teacherSubject, "Maths");
        break;
    case 3:
        strcpy_s(teacherSubject, "English");
        break;
    case 4:
        strcpy_s(teacherSubject, "Hindi");
        break;
    case 5:
        strcpy_s(teacherSubject, "Social Studies");
        break;
    }
    cout << "\nEnter teacher annual salary (in RON): ";
    cin >> teacherSalary;
    cout << "Enter teacher work experience (in years): ";
    cin >> teacherExperience;
    cout << "Enter teacher educational qualification: ";
    cin.ignore();
    gets_s(teacherQualification);
    generateTeacherId();
    cout << "\nGenerated teacher ID is " << getId() << ". Please, note it safely as it'll be asked during data modification.";
    basicNavigation();
}

void Teacher::updateData()
{
    char userChoice = ' ';
    cout << "Press '+' to update teacher name or press 'Enter' key to retain old data: ";
    cin.ignore();
    cin.get(userChoice);
    if (userChoice == '+' && userChoice != 10) // 10 is ASCII value of linefeed (enter key)
    {
        char userChoice = ' ';
        cout << "Press '+' to update teacher name or press 'Enter' key to retain old data: ";
        cin.ignore();
        cin.get(userChoice);
        if (userChoice == '+' && userChoice != 10) // 10 is ASCII value of linefeed (enter key)
        {
            cout << "\nSorry, we received a wrong subject code. Please enter valid subject code viz. 1, 2, 3, 4 or 5: ";
            cin >> teacherSubjectCode;
        }
        switch (teacherSubjectCode)
        {
        case 1:
            strcpy_s(teacherSubject, "Science");
            break;
        case 2:
            strcpy_s(teacherSubject, "Maths");
            break;
        case 3:
            strcpy_s(teacherSubject, "English");
            break;
        case 4:
            strcpy_s(teacherSubject, "Hindi");
            break;
        case 5:
            strcpy_s(teacherSubject, "Social Studies");
            break;
        }
    }
    cout << "\nPress '+' to update teacher annual salary or press 'Enter' key to retain old data: ";
    cin.get(userChoice);
    if (userChoice == '+' && userChoice != 10)
    {
        cout << "Enter new annual salary (in Rs.): ";
        cin.ignore();
        cin >> teacherSalary;
        cin.ignore();
    }
    cout << "\nPress '+' to update teacher work experience or press 'Enter' key to retain old data: ";
    cin.get(userChoice);
    if (userChoice == '+' && userChoice != 10)
    {
        cout << "Enter new work experience (in years): ";
        cin.ignore();
        cin >> teacherExperience;
        cin.ignore();
    }
    cout << "\nPress '+' to update teacher educational qualification or press 'Enter' key to retain old data: ";
    cin.get(userChoice);
    if (userChoice == '+' && userChoice != 10)
    {
        cout << "Enter new educational qualification (max. 8 characters): ";
        cin.ignore();
        gets_s(teacherQualification);
        cin.ignore();
    }
    generateTeacherId();
    cout << "\nGenerated ID is " << getId() << ". Please, note it safely as it'll be asked during data modification.";
    basicNavigation();
}

class Staff : public SchoolMember
{
private:
    char staffDepartment[NAME_LENGTH]{};
    int staffSalary{};
    short staffDepartmentCode{};

public:
    void inputDetails();
    void updateData();
    void generateStaffId();
    void displayData() const
    {
        displayNameInUpper(name, strlen(name));
        cout << "\nID: " << id << "\nDepartment: " << staffDepartment
            << "\nSalary to be paid: RON" << staffSalary;
    }
    void deductSalary(int salaryPaid) { staffSalary = staffSalary - salaryPaid; }
    int getSalary() const { return staffSalary; }
};

// generate staff id in format as explained:
// first digit indicates staff department, it is multiplied by 1000
// then if same staff department is found, id is incremented by 1
void Staff::generateStaffId()
{
    Staff staffRead;
    short tempId = 1;
    ifstream fileToRead("data/staff.dat", ios::binary);
    while (fileToRead.read((char*)&staffRead, sizeof(staffRead)))
    {
        if (staffDepartmentCode == staffRead.staffDepartmentCode)
        {
            tempId++;
        }
    }
    id = (staffDepartmentCode * 1000) + tempId;
}

// all staff inputs
void Staff::inputDetails()
{
    system("cls");
    cout << "Enter staff name (max. 28 characters): ";
    cin.ignore();
    gets_s(name);
    cout << "\nDepartment Code\t\tName of Department\n\t1\t\tCleaning\n\t2\t\tManagement\n\t3\t\tOffice work\n\t4\t\tOthers";
    cout << "\nEnter department code from above list to assign staff to that department: ";
    cin >> staffDepartmentCode;
    while (staffDepartmentCode > 4 || staffDepartmentCode < 1)
    {
        cout << "\nSorry, we received a wrong department code. Please enter valid department code viz. 1, 2, 3 or 4: ";
        cin >> staffDepartmentCode;
    }
    switch (staffDepartmentCode)
    {
    case 1:
        strcpy_s(staffDepartment, "Cleaning");
        break;
    case 2:
        strcpy_s(staffDepartment, "Management");
        break;
    case 3:
        strcpy_s(staffDepartment, "Office Work");
    case 4:
        strcpy_s(staffDepartment, "Others");
        break;
    }
    cout << "Enter staff annual salary (in Rs.): ";
    cin >> staffSalary;
    generateStaffId();
    cout << "Generated staff ID is " << getId() << ". Please, note it safely as it'll be asked during data modification.";
    basicNavigation();
}

void Staff::updateData()
{
    char userChoice = ' ';
    cout << "Press '+' to update staff name or press 'Enter' key to retain old data: ";
    cin.ignore();
    cin.get(userChoice);
    if (userChoice == '+' && userChoice != 10) // 10 is ASCII value of linefeed (enter key)
    {
        cout << "Enter new name (max. 28 characters): ";
        cin.ignore();
        gets_s(name);
        cin.ignore();
    }
    cout << "\nPress '+' to update staff department code or press 'Enter' key to retain old data: ";
    cin.get(userChoice);
    if (userChoice == '+' && userChoice != 10)
    {
        cout << "\nDepartment Code\t\tName of Department\n\t1\t\tCleaning\n\t2\t\tManagement\n\t3\t\tOffice work\n\t4\t\tOthers";
        cout << "\nEnter department code from above list to assign staff to that department: ";
        cin.ignore();
        cin >> staffDepartmentCode;
        cin.ignore();
        while (staffDepartmentCode > 4 || staffDepartmentCode < 1)
        {
            cout << "\nSorry, we received a wrong department code. Please enter valid department code viz. 1, 2, 3 or 4: ";
            cin >> staffDepartmentCode;
        }
        switch (staffDepartmentCode)
        {
        case 1:
            strcpy_s(staffDepartment, "Cleaning");
            break;
        case 2:
            strcpy_s(staffDepartment, "Management");
            break;
        case 3:
            strcpy_s(staffDepartment, "Office Work");
            break;
        case 4:
            strcpy_s(staffDepartment, "Others");
            break;
        }
    }
    cout << "\nPress '+' to update staff annual salary or press 'Enter' key to retain old data: ";
    cin.get(userChoice);
    if (userChoice == '+' && userChoice != 10)
    {
        cout << "Enter new annual salary (in Rs.): ";
        cin.ignore();
        cin >> staffSalary;
        cin.ignore();
    }
    generateStaffId();
    cout << "\nGenerated ID is " << getId() << ". Please, note it safely as it'll be asked during data modification.";
}

class Academic : public Student
{
private:
    short marks[5];
    float daysPresent, totalWorkingDays, attendancePercentage, averageMarks;
    using Student::id;
    using Student::name;

public:
    Academic()
    {
        marks[0] = 0;
        marks[1] = 0;
        marks[2] = 0;
        marks[3] = 0;
        marks[4] = 0;
        daysPresent = 0.0f;
        totalWorkingDays = 0.0f;
        averageMarks = 0.0f;
        attendancePercentage = 0.0f;
    }

    int getId() const { Student::getId(); return id; }
    void generateAcademicReport(Student&, short&);
    void generateAttendanceReport(Student&, short&);
    void displayAcademicReport() const
    {
        cout << "\n\t\t\t\t\t-------------------";
        cout << "\n\t\t\t\t\t  ACADEMIC REPORT";
        cout << "\n\t\t\t\t\t-------------------\n";
        displayNameInUpper(name, strlen(name));
        cout << "\nClass: " << studentClass << " '" << char(toupper(studentSection)) << "' "
            << "\nMother's name: " << motherName << "\nFather's name: " << fatherName;
        // Subject codes: 1 = Science, 2 = Maths, 3 = English, 4 = Hindi, 5 = Social Studies
        cout << "\n\nSubject Code\t\tSubject\t\t\tMarks (out of 100)";
        cout << "\n     1\t\t\tScience\t\t\t\t" << marks[0];
        cout << "\n     2\t\t\tMaths\t\t\t\t" << marks[1];
        cout << "\n     3\t\t\tEnglish\t\t\t\t" << marks[2];
        cout << "\n     4\t\t\tHindi\t\t\t\t" << marks[3];
        cout << "\n     5\t\t\tSocial Studies\t\t\t" << marks[4];
        cout << fixed << setprecision(1) << "\n\n\t\t\tAverage Percentage = " << averageMarks << "%";
    }
    void displayAttendanceReport() const
    {
        cout << "\n\t\t\t\t\t---------------------";
        cout << "\n\t\t\t\t\t  ATTENDANCE REPORT";
        cout << "\n\t\t\t\t\t---------------------\n";
        displayNameInUpper(name, strlen(name));
        cout << "\nClass: " << studentClass << " '" << char(toupper(studentSection)) << "' "
            << "\nMother's name: " << motherName << "\nFather's name: " << fatherName;
        cout << "\n\nTotal Number of Working Days = " << totalWorkingDays;
        cout << "\nTotal days student " << id << " was present = " << daysPresent;
        cout << fixed << setprecision(1) << "\nAttendance percentage = " << attendancePercentage << "%";
    }
};

// generates student academic report
// validates roll number, if roll number is found
// then marks for five subject is asked then average is calculated
void Academic::generateAcademicReport(Student& studentRead, short& flag)
{
    system("cls");
    int inputRollNumber = 0;
    cout << "Enter student roll number whose academic report you want to generate: ";
    cin >> inputRollNumber;
    ifstream fileToRead("data/student.dat", ios::binary);
    while (fileToRead.read((char*)&studentRead, sizeof(studentRead)))
    {
        if (inputRollNumber == studentRead.getId())
        {
            Academic AcademicTemp;
            AcademicTemp.id = inputRollNumber;
            AcademicTemp.studentClass = studentRead.studentClass;
            AcademicTemp.studentSection = studentRead.studentSection;
            strcpy_s(AcademicTemp.name, studentRead.name);
            strcpy_s(AcademicTemp.fatherName, studentRead.fatherName);
            strcpy_s(AcademicTemp.motherName, studentRead.motherName);
            ++flag;
            break;
        }
    }
    if (flag > 0)
    {
        cout << "\nEnter marks of student " << inputRollNumber << " between 0 to 100 in subjects asked below: ";
        cout << "\n1. Science = ";
        cin >> marks[0];
        validateSubjectMarks(marks, 0);
        cout << "2. Maths = ";
        cin >> marks[1];
        validateSubjectMarks(marks, 1);
        cout << "3. English = ";
        cin >> marks[2];
        validateSubjectMarks(marks, 2);
        cout << "4. Hindi = ";
        cin >> marks[3];
        validateSubjectMarks(marks, 3);
        cout << "5. Social Studies = ";
        cin >> marks[4];
        validateSubjectMarks(marks, 4);
        averageMarks = (marks[0] + marks[1] + marks[2] + marks[3] + marks[4]) / 5.00f;
        cout << fixed << setprecision(1) << "Average Percentage = " << averageMarks << "%";
    }
    else
    {
        cout << "No match found.";
    }
}

// generates student attendance report
// validates roll number, if roll number is found
// then total number of working days and days present is asked
// attendance % is calculated as : (days preset / total no. days) * 100
void Academic::generateAttendanceReport(Student& studentRead, short& flag)
{
    system("cls");
    int inputRollNumber = 0;
    cout << "Enter student roll number whose attendance report you want to generate: ";
    cin >> inputRollNumber;
    ifstream fileToRead("data/student.dat", ios::binary);
    while (fileToRead.read((char*)&studentRead, sizeof(studentRead)))
    {
        if (inputRollNumber == studentRead.getId())
        {
            Academic AcademicTemp;
            AcademicTemp.id = inputRollNumber;
            AcademicTemp.studentClass = studentRead.studentClass;
            AcademicTemp.studentSection = studentRead.studentSection;
            strcpy_s(AcademicTemp.name, studentRead.name);
            strcpy_s(AcademicTemp.fatherName, studentRead.fatherName);
            strcpy_s(AcademicTemp.motherName, studentRead.motherName);
            ++flag;
            break;
        }
    }
    if (flag > 0)
    {
        cout << "\nEnter total number of working days (1 - 366): ";
        cin >> totalWorkingDays;
        validateDays(totalWorkingDays);
        cout << "Enter number of days student " << inputRollNumber << " was present (1 - 366): ";
        cin >> daysPresent;
        validateDays(daysPresent);
        attendancePercentage = ((daysPresent / totalWorkingDays) * 100.00f);
        cout << fixed << setprecision(1) << "Attendance Percentage = " << attendancePercentage << "%";
    }
    else
    {
        cout << "No match found.";
    }
}

// calls studentAcademics.generateAcademicReport(Student)
// and writes recorded information to academic_report.dat file
void academicReport()
{
    short flag = 0;
    Academic studentAcademics;
    Student studentRead;
    fstream academicFile("data/academic_report.dat", ios::in | ios::out | ios::app | ios::binary);
    studentAcademics.generateAcademicReport(studentRead, flag);
    if (flag > 0)
    {
        academicFile.write((char*)&studentAcademics, sizeof(studentAcademics));
    }
    academicFile.close();
    basicNavigation();
}

// calls studentAcademics.generateAttendanceReport(Student)
// and writes recorded information to attendance_report.dat file
void attendanceReport()
{
    short flag = 0;
    Academic studentAcademics;
    Student studentRead;
    fstream attendanceFile("data/attendance_report.dat", ios::in | ios::out | ios::app | ios::binary);
    studentAcademics.generateAttendanceReport(studentRead, flag);
    if (flag > 0)
    {
        attendanceFile.write((char*)&studentAcademics, sizeof(studentAcademics));
    }
    attendanceFile.close();
    basicNavigation();
}

void viewAcademicReports()
{
    system("cls");
    Academic academicRead;
    ifstream attendanceFile("data/attendance_report.dat", ios::binary);
    ifstream academicFile("data/academic_report.dat", ios::binary);
    int inputRollNumber = 0;
    short userChoice = 0, flag = 0;
    cout << "1. View academic report";
    cout << "\n2. View attendance report";
    cout << "\nEnter your choice (1 or 2): ";
    cin >> userChoice;
    switch (userChoice)
    {
    case 1:
        cout << "\nEnter student roll number whose academic report you want to see: ";
        cin >> inputRollNumber;
        while (academicFile.read((char*)&academicRead, sizeof(academicRead)))
        {
            if (inputRollNumber == academicRead.getId())
            {
                ++flag;
                break;
            }
        }
        if (flag > 0)
        {
            academicRead.displayAcademicReport();
        }
        else
        {
            cout << "Sorry, no academic report found for student " << inputRollNumber << ".";
        }
        break;
    case 2:
        cout << "\nEnter student roll number whose attendance report you want to see: ";
        cin >> inputRollNumber;
        while (attendanceFile.read((char*)&academicRead, sizeof(academicRead)))
        {
            if (inputRollNumber == academicRead.getId())
            {
                ++flag;
                break;
            }
        }
        if (flag > 0)
        {
            academicRead.displayAttendanceReport();
        }
        else
        {
            cout << "Sorry, no attendance report found for student " << inputRollNumber << ".";
        }
        break;
    }
    basicNavigation();
}

// displays screen for addition of new student, teacher or staff
void displayAddDataScreen()
{
    system("cls");
    int userChoice = 0;
    Student schoolStudent;
    Teacher schoolTeacher;
    Staff schoolStaff;
    cout << "1. Add a new student";
    cout << "\n2. Add a new teacher";
    cout << "\n3. Add a new staff";
    cout << "\n\n=> Enter your choice to proceed. For e.g. Press '1' to 'Add a new student': ";
    cin >> userChoice;
    switch (userChoice)
    {
    case 1:
        addMember("data/student.dat", schoolStudent);
        break;
    case 2:
        addMember("data/teacher.dat", schoolTeacher);
        break;
    case 3:
        addMember("data/staff.dat", schoolStaff);
        break;
    }
}

void displayRemoveDataScreen()
{
    system("cls");
    int userChoice = 0;
    Student schoolStudent;
    Teacher schoolTeacher;
    Staff schoolStaff;
    cout << "1. Remove an existing student";
    cout << "\n2. Remove an existing teacer";
    cout << "\n3. Remove an existing staff";
    cout << "\n\n=> Enter your choice to proceed. For e.g. Press '1' to 'Remove an existing student': ";
    cin >> userChoice;
    switch (userChoice)
    {
    case 1:
        removeMember("data/student.dat", "data/temp_student.dat", schoolStudent);
        break;
    case 2:
        removeMember("data/teacher.dat", "data/temp_teacher.dat", schoolTeacher);
        break;
    case 3:
        removeMember("data/staff.dat", "data/temp_staff.dat", schoolStaff);
        break;
    }
}

// give user options to choose whose data has to be updated
void displayUpdateDataScreen()
{
    system("cls");
    int userChoice = 0;
    Student schoolStudent;
    Teacher schoolTeacher;
    Staff schoolStaff;
    cout << "1. Update data of an existing student";
    cout << "\n2. Update data of an existing teacher";
    cout << "\n3. Update data of an existing staff";
    cout << "\n\n=> Enter your choice to proceed. For e.g. Press '1' to 'Update data of an existing student': ";
    cin >> userChoice;
    switch (userChoice)
    {
    case 1:
        updateMemberData("data/student.dat", schoolStudent);
        break;
    case 2:
        updateMemberData("data/teacher.dat", schoolTeacher);
        break;
    case 3:
        updateMemberData("data/staff.dat", schoolStaff);
        break;
    }
}

// displays home screen only if the administrator has entered the correct credentials
// note: Administrator is anyone using our school management system app
void HomeScreen()
{
    Student schoolStudent;
    Teacher schoolTeacher;
    Staff schoolStaff;
    system("cls");
    char menuOption;
    cout << "1. WORK WITH DATA RECORDS\t\t\t\t\t\t2. VIEW DATA RECORDS";
    cout << "\nA. Add a new student, teacher or staff\t\t\t\t\tD. View student data records";
    cout << "\nB. Remove an existing student, teacher or staff \t\t\tE. View teacher data records";
    cout << "\nC. Update data of an existing student, teacher or staff\t\t\tF. View staff data records";
    cout << "\n\n3. ACCOUNTING AND FINANCE\t\t\t\t\t\t4. STUDENT ACADEMICS";
    cout << "\nG. Receive student fee\t\t\t\t\t\t\tJ. View student academic or attendance report";
    cout << "\nH. Pay salary to school teacher\t\t\t\t\t\tK. Generate student academic report";
    cout << "\nI. Pay salary to school staff\t\t\t\t\t\tL. Generate student attendance report";
    cout << "\n\n=> You can choose from above displayed menu options. For e.g. 'A' to 'Add a new student, teacher or staff'.";
    cout << "\nOR\n=> Press 'Z' to quit.\n=> Enter your choice to proceed: ";
    char expectedInput[] = { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'z'};
    cin >> menuOption;
    validateCharInput(menuOption, expectedInput, 13);
    switch (menuOption)
    {
    case 'a':
        displayAddDataScreen();
        break;
    case 'b':
        displayRemoveDataScreen();
        break;
    case 'c':
        displayUpdateDataScreen();
        break;
    case 'd':
        viewData("data/student.dat", schoolStudent);
        break;
    case 'e':
        viewData("data/teacher.dat", schoolTeacher);
        break;
    case 'f':
        viewData("data/staff.dat", schoolStaff);
        break;
    case 'g':
        receiveStudentFee();
        break;
    case 'h':
        paySalaryToMember("data/teacher.dat", schoolTeacher);
        break;
    case 'i':
        paySalaryToMember("data/staff.dat", schoolStaff);
        break;
    case 'j':
        viewAcademicReports();
        break;
    case 'k':
        academicReport();
        break;
    case 'l':
        attendanceReport();
        break;
    case 'z':
        exit(0);
        break;
    }
}

int main()
{
    HomeScreen();
}