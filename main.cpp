#include <iostream>     // for basic I/O operations e.g. cin, cout, etc.
#include <fstream>      // for data file handling
#include <iomanip>      // manipulates floating point numbers and also manipulates console I/O
#include <string.h>     // for strcmp()
#include <stdio.h>      // for remove() and rename()
#include "Header.h"     // contains utils required for proper I/O

using namespace std;    // we don't need to add 'std::' standard library methods

// everything which a member of school have in common is declared here
class SchoolMember
{
protected:
    char name[29]{};
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
void removeMember(const string filePath, const string tempFilePath, const T& schoolMember)
{
    int inputId;
    short flag = 0;
    ifstream fileToRead(filePath, ios::binary);
    ofstream fileToWrite(tempFilePath, ios::binary | ios::app);
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
    rename(tempFilePath.c_str(), filePath.c_str());
}

// using function template to add contents of student, teacher or staff
template <typename T>
void addMember(const string filePath, T& schoolMember)
{
    fstream file(filePath, ios::in | ios::oct | ios::app | ios::binary);
    schoolMember.inputDetails();
    file.write((char*)&schoolMember, sizeof(schoolMember));
    file.close();
}

// student class has all the data members and required member functions for students
class Student : public SchoolMember
{
private:
    int studentFee{};

protected:
    short studentClass{};
    char motherName[29]{}, fatherName[29]{}, studentSection{};

public:
    Student() // constructor
    {
        id = 1;
    }

    void inputDetails();
    void generateRollNumber();
    void displayStudentData()
    {
        cout << "\nName" << name << "\nRoll Number: " << id << "\nClass: " << studentClass << " '" << char(toupper(studentSection)) << "' "
            << "\nMother's name: " << motherName << "\nFather's name: " << fatherName << "\nRemaining fee: RON" << studentFee;
    }
    void deductFee(int amountPiad) { studentFee = studentFee - amountPiad; }
    int getStudentFee() const { return studentFee; }
    string getName() const { return name; }
};

// generate roll number by using class, section and first four characters of name
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
        if (studentClass == schoolStudentTemp.studentClass && tolower(studentSection) == tolower(schoolStudentTemp.studentSection))
        {
            ++flag;
            for (short i = 0; i < 4; ++i)
            {
                if (tolower(schoolStudentTemp.name[i]) > tolower(name[i]))
                {
                    schoolStudentTemp.id++;
                    id = schoolStudentTemp.id - 1;
                    break;
                }
                else if (tolower(schoolStudentTemp.name[i]) < tolower(name[i]))
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
    if (flag == 0)
    {
        id = (studentClass * 1000) + (tolower(studentSection) - 96) * 100 + id;
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
    cin.ignore();
    cin.get(studentSection);
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
void viewStudentData()
{
    system("cls");
    Student studentRead;
    ifstream fileToRead("data/student.dat", ios::binary);
    int inputRollNumber = 0;
    short flag = 0;
    cout << "Enter roll number of student whoose data records you want to see: ";
    cin >> inputRollNumber;
    while (fileToRead.read((char*)&studentRead, sizeof(studentRead)))
    {
        if (inputRollNumber == studentRead.getId())
        {
            ++flag;
            break;
        }
    }
    if (flag != 0)
    {
        studentRead.displayStudentData();
    }
    else
    {
        cout << "No match found.";
    }
    basicNavigation();
}

class Teacher : public SchoolMember
{
private:
    char teacherSubjects[29]{}, teacherQualification[9]{};
    short teacherExperience{}, teacherClass{}, teacherSubjectCode[3]{};
    int teacherSalary{};

public:
    Teacher()
    {
        teacherSubjectCode[0] = 0;
        teacherSubjectCode[1] = 0;
        teacherSubjectCode[2] = 0;
    }
    void inputDetails();
    void generateTeacherId();
    void deductTeacherSalary(int salaryPaid) { teacherSalary = teacherSalary - salaryPaid; }
    void displayTeacherData() const
    {
        cout << "\nName: " << name << "\nID: " << id << "\nTeacher Qualification: " << teacherQualification << "\nExperience: " << teacherExperience << " years"
            << "\nClass taught: " << teacherClass << "\nSubjects taught: " << teacherSubjects
            << "\nSalary to be paid: RON" << teacherSalary;
    }
    int getTeacherSalary() const { return teacherSalary; }
};

// generates id in format: first three digist are subject code of subject the teacher teaches and the last two digits are
// incremented when same subject set of teacher is found. for e.g. 12305 indicates that teacher teach subject codes 1, 2 & 3 and
// is the 5th teacher to teache the same three subjects in school
void Teacher::generateTeacherId()
{
    Teacher schoolTeacherRead;
    ifstream teacherFile("data/teacher.dat", ios::binary);
    short tempId = 1, arrayEquality = 0, loopCounter = 0;
    while (teacherFile.read((char*)&schoolTeacherRead, sizeof(schoolTeacherRead)))
    {
        arrayEquality = 0;
        for (loopCounter = 0; loopCounter < 3; ++loopCounter)
        {
            if (teacherSubjectCode[loopCounter] == schoolTeacherRead.teacherSubjectCode[loopCounter])
                ++arrayEquality;
        }
        if (arrayEquality == loopCounter)
            tempId++;
    }
    id = (teacherSubjectCode[0] * 10000) + (teacherSubjectCode[1] * 1000) + (teacherSubjectCode[2] * 100) + tempId;
}

void Teacher::inputDetails()
{
    system("cls");
    short flag = 0;
    char userChoice, keepRunning;
    cout << "Enter teacher name (max. 28 characters): ";
    cin.ignore();
    gets_s(name);
    cout << "Enter the class to be taught (1 to 12): ";
    cin >> teacherClass;
    cout << "\nSubject Code\tSubject\n     1\t\tScience\n     2\t\tMaths\n     3\t\tEnglish\n     4\t\tHindi\n     5\t\tSocial Studies";
    cout << "\nEnter the subject code(s) from the above list to choose subject(s) the teacher will teach, for e.g. Press '1' to choose 'Science': ";
    cin >> teacherSubjectCode[flag];
    teacherSubjectCodeVerifier(teacherSubjectCode, flag);
    ++flag;
    do
    {
        if (flag == 3)
        {
            cout << "Maximum limit of subject per teacher has reached. No more subject can be assigned.\n";
            break;
        }
        else
        {
            cout << "Do you want to assign more subjects? Enter 'Y' for yes or 'N' for no: ";
            cin >> userChoice;
            if (userChoice == 'y')
            {
                cout << "Please enter a subject code: ";
                cin >> teacherSubjectCode[flag];
                teacherSubjectCodeVerifier(teacherSubjectCode, flag);
                do
                {
                    keepRunning = 'n';
                    for (short i = 0; i < flag; ++i)
                    {
                        if (teacherSubjectCode[flag] == teacherSubjectCode[i])
                        {
                            cout << "You have already assigned this subject. Please enter another subject code: ";
                            cin >> teacherSubjectCode[flag];
                            teacherSubjectCodeVerifier(teacherSubjectCode, flag);
                            keepRunning = 'y';
                            break;
                        }
                    }
                } while (keepRunning == 'y');
                ++flag;
            }
        }
    } while (userChoice == 'y' && flag < 4);
    setTeacherSubjects(teacherSubjectCode, teacherSubjects);
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

// pay salary to teacher then deducts from remaining
// reflects the same in teacher.dat
void paySalaryToTeacher()
{
    system("cls");
    Teacher schoolTeacher;
    int inputTeacherId = 0, salaryPaid = 0;
    streamoff filePosition = 0;
    short flag = 0;
    fstream teacherFile("data/teacher.dat", ios::in | ios::out | ios::binary);
    cout << "Enter ID of teacher to pay salary: ";
    cin >> inputTeacherId;
    while (!teacherFile.eof())
    {
        filePosition = teacherFile.tellg();
        teacherFile.read((char *)&schoolTeacher, sizeof(schoolTeacher));
        if (inputTeacherId == schoolTeacher.getId())
        {
            ++flag;
            break;
        }
    }
    if (flag == 0)
    {
        cout << "Sorry, no teacher found with ID " << inputTeacherId << ".";
    }
    else if (schoolTeacher.getTeacherSalary() == 0)
    {
        cout << "Congratulations! Total annual salary has been paid to teacher " << schoolTeacher.getId() << ".";
    }
    else
    {
        cout << "Remaining salary to be paid to teacher " << schoolTeacher.getId() << " is RON " << schoolTeacher.getTeacherSalary() << ". Enter the salary to be paid (in RON): ";
        cin >> salaryPaid;
        while (salaryPaid > schoolTeacher.getTeacherSalary() || salaryPaid < 0)
        {
            cout << "You have entered invalid salary. Please enter a valid salary (in RON): ";
            cin >> salaryPaid;
        }
        schoolTeacher.deductTeacherSalary(salaryPaid);
        teacherFile.seekg(filePosition);
        teacherFile.write((char *)&schoolTeacher, sizeof(schoolTeacher));
        cout << "You have successfully paid " << salaryPaid << ". Now remaining salary for teacher " << schoolTeacher.getId() << " is : RON" << schoolTeacher.getTeacherSalary();
    }
    teacherFile.close();
    basicNavigation();
}

void viewTeacherData()
{
    system("cls");
    Teacher teacherRead;
    ifstream fileToRead("data/teacher.dat", ios::binary);
    int inputId = 0;
    short flag = 0;
    cout << "Enter ID of teacher whose data records you want to see: ";
    cin >> inputId;
    while (fileToRead.read((char*)&teacherRead, sizeof(teacherRead)))
    {
        if (inputId == teacherRead.getId())
        {
            ++flag;
            break;
        }
    }
    if (flag != 0)
    {
        teacherRead.displayTeacherData();
    }
    else
    {
        cout << "No match found.";
    }
    basicNavigation();
}

class Staff : public SchoolMember
{
private:
    char staffDepartment[29]{};
    int staffSalary{};
    short staffDepartmentCode{};

public:
    void inputDetails();
    void generateStaffId();
    void displayStaffData() const
    {
        cout << "\nName: " << name << "\nID: " << id << "\nDepartment: " << staffDepartment
            << "\nSalary to be paid: RON " << staffSalary;
    }
    void deductStaffSalary(int salaryPaid) { staffSalary = staffSalary - salaryPaid; }
    int getStaffSalary() const { return staffSalary; }
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

// pay salary to staff
// deduct salary paid and reflect in staff.dat file
void paySalaryToStaff()
{
    system("cls");
    Staff schoolStaff;
    int inputStaffId = 0, salaryPaid = 0;
    streamoff filePosition = 0;
    short flag = 0;
    fstream staffFile("data/staff.dat", ios::in | ios::out | ios::binary);
    cout << "Enter ID of staff to pay salary: ";
    cin >> inputStaffId;
    while (!staffFile.eof())
    {
        filePosition = staffFile.tellg();
        staffFile.read((char*)&schoolStaff, sizeof(schoolStaff));
        if (inputStaffId == schoolStaff.getId())
        {
            ++flag;
            break;
        }
    }
    if (flag == 0)
        cout << "Sorry, no staff found with ID " << inputStaffId << ".";
    else if (schoolStaff.getStaffSalary() == 0)
        cout << "Congratulations! Total annual salary has been paid to staff " << schoolStaff.getId() << ".";
    else
    {
        cout << "Remaining salary to be paid to staff " << schoolStaff.getId() << " is Rs. " << schoolStaff.getStaffSalary() << ". Enter the salary to be paid (in Rs.): ";
        cin >> salaryPaid;
        while (salaryPaid > schoolStaff.getStaffSalary() || salaryPaid < 0)
        {
            cout << "You have entered invalid salary. Please enter a valid salary (in Rs.): ";
            cin >> salaryPaid;
        }
        schoolStaff.deductStaffSalary(salaryPaid);
        staffFile.seekg(filePosition);
        staffFile.write((char*)&schoolStaff, sizeof(schoolStaff));
        cout << "You have successfully paid " << salaryPaid << ". Now remaining salary for staff " << schoolStaff.getId() << " is: Rs." << schoolStaff.getStaffSalary();
    }
    staffFile.close();
    basicNavigation();
}

void viewStaffData()
{
    system("cls");
    Staff staffRead;
    ifstream fileToRead("data/staff.dat", ios::binary);
    int inputId = 0;
    short flag = 0;
    cout << "Enter ID of staff whose data records you want to see: ";
    cin >> inputId;
    while (fileToRead.read((char*)&staffRead, sizeof(staffRead)))
    {
        if (inputId == staffRead.getId())
        {
            ++flag;
            break;
        }
    }
    if (flag != 0)
    {
        staffRead.displayStaffData();
    }
    else
    {
        cout << "No match found.";
    }
    basicNavigation();
}

class Academic
{
private:
    short marks[5]{}, daysPresent{}, totalWorkingDays{};
    float averageMarks{}, attendancePercentage{};

public:
    Academic()
    {
        marks[0] = 0;
        marks[1] = 0;
        marks[2] = 0;
        marks[3] = 0;
        marks[4] = 0;
        daysPresent = 0;
        totalWorkingDays = 0;
        averageMarks = 0.0f;
        attendancePercentage = 0.0f;
    }

    void generateAcademicReport(Student, short&);
    void generateAttendanceReport(Student&, short&);
    void displayAttendanceReport(Student student)
    {
        cout << "Name: " << student.getName();
        cout << "\nRoll number: " << student.getId();
        cout << fixed << setprecision(1) << "\nAttendance percentage = " << attendancePercentage;
    }
};

// generates student academic report
// validates roll number, if roll number is found
// then marks for five subject is asked then average is calculated
void Academic::generateAcademicReport(Student studentRead, short& flag)
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
        averageMarks = (marks[0] + marks[1] + marks[2] + marks[3] + marks[4]) / 5.f;
        cout << fixed << setprecision(1) << "Generated Average Marks = " << averageMarks;
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
        attendancePercentage = ((daysPresent / totalWorkingDays) * 100.f);
        cout << fixed << setprecision(1) << "Generated Attendance Percentage = " << attendancePercentage << "%";
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
    fstream reportFile("data/academic_report.dat", ios::in | ios::out | ios::app | ios::binary);
    studentAcademics.generateAcademicReport(studentRead, flag);
    if (flag > 0)
    {
        reportFile.write((char*)&studentAcademics, sizeof(studentAcademics));
    }
    reportFile.close();
}

// calls studentAcademics.generateAttendanceReport(Student)
// and writes recorded information to attendance_report.dat file
void attendanceReport()
{
    short flag = 0;
    Academic studentAcademics;
    Student studentRead;
    fstream reportFile("data/attendance_report.dat", ios::in | ios::out | ios::app | ios::binary);
    studentAcademics.generateAttendanceReport(studentRead, flag);
    if (flag > 0)
    {
        reportFile.write((char*)&studentRead, sizeof(studentRead));
        reportFile.write((char*)&studentAcademics, sizeof(studentAcademics));
    }
    reportFile.close();
}

void viewAcademicReports()
{
    system("cls");
    Academic academicRead;
    Student studentRead;
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
        break;
    case 2:
        cout << "\nEnter student roll number whose attendance report you want to see: ";
        cin >> inputRollNumber;
        while (attendanceFile.read((char*)&studentRead, sizeof(studentRead)))
        {
            if (inputRollNumber == studentRead.getId())
            {
                attendanceFile.read((char*)&academicRead, sizeof(academicRead));
                ++flag;
                break;
            }
        }
        if (flag > 0)
        {
            academicRead.displayAttendanceReport(studentRead);
        }
        else
        {
            cout << "No attendance report found for student " << inputRollNumber << ".";
            break;
        }
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

// displays home screen only if the administrator has entered the correct credentials
// note: Administrator is anyone using our school management system app
void HomeScreen()
{
    system("cls");
    char menuOption;
    cout << "1. WORK WITH DATA RECORDS\t\t\t\t\t\t2. VIEW DATA RECORDS";
    cout << "\nA. Add a new student, teacher or staff\t\t\t\t\tD. View student data records";
    cout << "\nB. Remove an existing student, teacher or staff \t\t\tE. View teacher data records";
    cout << "\nC. Update data of an existing student, teacher or staff\t\t\tF. View staff data records";
    cout << "\n\n3. ACCOUNTING AND FINANCE\t\t\t\t\t\t4. STUDENT ACADEMICS";
    cout << "\nG. Receive student fee\t\t\t\t\t\t\tJ. View student academics report";
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
        cout << "\nWork under progress...";
        break;
    case 'd':
        viewStudentData();
        break;
    case 'e':
        viewTeacherData();
        break;
    case 'f':
        viewStaffData();
        break;
    case 'g':
        receiveStudentFee();
        break;
    case 'h':
        paySalaryToTeacher();
        break;
    case 'i':
        paySalaryToStaff();
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