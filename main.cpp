#include <iostream>     // for basic I/O operations e.g. cin, cout, etc.
#include <fstream>      // for data file handling
#include <string>       // for string objects
#include "Header.h"     // contains utils required for proper I/O

using namespace std;    // we don't need to add 'std::' before cout, cin and other standard library methods

// student class has all the data members and required member functions for students
class Student
{
private:
    int rollNumber, studentClass{};
    char studentName[29]{}, studentSection{};
    string motherName, fatherName;

public:
    Student() // constructor
    {
        rollNumber = 1;
    }
    void inputStudentDetails();
    int generateRollNumber();
};

// generate and return student roll number in format - 12252 which is equivalent to 12th 'B' 52
// we generate roll number alphabetically by using the first three characters of name.
int Student::generateRollNumber()
{
    Student schoolStudentTemp;
    fstream studentFile("data/student.dat", ios::in | ios::out | ios::app | ios::binary);
    short flag = 0;

    while (studentFile.read((char*)&schoolStudentTemp, sizeof(schoolStudentTemp)))
    {
        if (studentClass == schoolStudentTemp.studentClass && tolower(studentSection) == tolower(schoolStudentTemp.studentSection))
        {
            flag++;
            for (short i = 0; i < 3; ++i)
            {
                if (tolower(schoolStudentTemp.studentName[i]) > tolower(studentName[i]))
                {
                    schoolStudentTemp.rollNumber++;
                    rollNumber = schoolStudentTemp.rollNumber - 1;
                    break;
                }
                else if (tolower(schoolStudentTemp.studentName[i]) < tolower(studentName[i]))
                {
                    rollNumber = schoolStudentTemp.rollNumber + 1;
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
        rollNumber = (studentClass * 1000) + (tolower(studentSection) - 96) * 100 + rollNumber;
    }
    return rollNumber;
}

void Student::inputStudentDetails()
{
    system("cls");
    cin.ignore(); // fixes skipping of input; this will be used throughout the code
    cout << "\nEnter student name (max. 28 characters): ";
    gets_s(studentName);
    cout << "Enter the class (1 to 12): ";
    cin >> studentClass;
    cout << "Enter the section ('A' to 'D'): ";
    cin.ignore();
    cin.get(studentSection);
    cout << "Enter student father's name: ";
    cin.ignore();
    getline(cin, fatherName);
    cout << "Enter student mother's name: ";
    getline(cin, motherName);
    cout << "\nGenerated roll number is " << generateRollNumber() << ". Please note it in a safe place for future reference.";
}

void addStudent()
{
    Student schoolStudent;
    fstream studentFile("data/student.dat", ios::in | ios::out | ios::app | ios::binary);
    schoolStudent.inputStudentDetails();
    studentFile.write((char*)&schoolStudent, sizeof(schoolStudent));
    studentFile.close();
}

class Teacher
{
private:
    string teacherName, teacherQualification;
    short teacherExperience{}, teacherClass{};
    char teacherSubject[11]{}, teacherId[5]{};

public:
    void inputTeacherDetails();
    void generateTeacherID();
};

void Teacher::generateTeacherID()
{
    Teacher schoolTeacherRead;
    fstream teacherFile("data/teacher.dat", ios::in | ios::out | ios::app | ios::binary);
    short flag = 0, id = 0;

    while (teacherFile.read((char*)&schoolTeacherRead, sizeof(schoolTeacherRead)))
    {
        flag++;
        if (strcmp(teacherSubject, schoolTeacherRead.teacherSubject) == 0)
        {
            id++;
        }
    }

    if (id == 0 || flag == 0)
    {
        newTeacherId(teacherId, teacherSubject);
    }

    if (id != 0)
    {
        for (int i = 0, j = 48; i < 5; i++) // in ASCII char 48 = '0'
        {
            if (i < 3)
            {
                teacherId[i] = toupper(teacherSubject[i]);
            }
            else
            {
                if (((id % 10) == 9) && i == 3)
                {
                    teacherId[i] = j + ((id / 10) + 1);
                    i++;
                    teacherId[i] = j;
                }
                else if (i == 3)
                {
                    teacherId[i] = j + (id / 10);
                    i++;
                    teacherId[i] = j + ((id % 10) + 1);
                }
            }
        }
    }
}

void Teacher::inputTeacherDetails()
{
    system("cls");
    cout << "\Enter teacher name: ";
    cin.ignore();
    getline(cin, teacherName);
    cout << "Enter the class to be taught (1 to 12): ";
    cin >> teacherClass;
    cout << "Enter the subject to be taught: ";
    cin.ignore();
    gets_s(teacherSubject);
    convertCharArrayToLower(teacherSubject, sizeof(teacherSubject));
    cout << "Enter teacher work experience (in years): ";
    cin >> teacherExperience;
    cout << "Enter teacher educational qualification: ";
    cin.ignore();
    getline(cin, teacherQualification);
    cout << "\nGenerated teacher ID is ";
    generateTeacherID();
    displayCharArray(teacherId, sizeof(teacherId));
    cout << ". Please note it in a safe place for future reference.";
}

void addTeacher()
{
    Teacher schoolTeacher;
    fstream teacherFile("data/teacher.dat", ios::in | ios::out | ios::app | ios::binary);
    schoolTeacher.inputTeacherDetails();
    teacherFile.write((char*)&schoolTeacher, sizeof(schoolTeacher));
    teacherFile.close();
}

// displays home screen only if the administrator has entered the correct credentials
// note: Administrator is anyone using our school management system app
void HomeScreen()
{
    system("cls");
    cout << "1. ADD OR REMOVE DATA RECORDS\t\t\t\t\t\t\t2. VIEW DATA RECORDS";
    cout << "\nA. Add or remove a student\t\t\t\t\t\t\tD. View student database";
    cout << "\nB. Add or remove a teacher\t\t\t\t\t\t\tE. View teacher database";
    cout << "\nC. Add or remove a school staff\t\t\t\t\t\t\tF. View staff database";
    cout << "\n\n3. ACCOUNTING AND FINANCE\t\t\t\t\t\t\t4. UPDATE OR MODIFY DATA RECORDS";
    cout << "\nG. Receive student fee\t\t\t\t\t\t\t\tJ. Update student data";
    cout << "\nH. Pay salaray to school staff or teacher\t\t\t\t\tK. Update class data";
    cout << "\nI. View last five transactions and account balance\t\t\t\tL. Update staff data";
    cout << "\n\n=> Enter the displayed menu option of your choice to proceed. For e.g. Press 'A' to 'Add or remove a student': ";
    char menuOption;
    cin.get(menuOption);
    switch (tolower(menuOption))
    {
    case 'a':
        addStudent();
        break;
    case 'b':
        addTeacher();
        break;
    case 'c':
        cout << "\nWork under progress...";
        break;
    case 'd':
        cout << "\nWork under progress...";
        break;
    case 'e':
        cout << "\nWork under progress...";
        break;
    case 'f':
        cout << "\nWork under progress...";
        break;
    case 'g':
        cout << "\nWork under progress...";
        break;
    case 'h':
        cout << "\nWork under progress...";
        break;
    case 'i':
        cout << "\nWork under progress...";
        break;
    default:
        cout << "\nWrong Input Received.";
    }
}

/**
 * main() starts here
*/
int main()
{
    HomeScreen();
    return 0;
}