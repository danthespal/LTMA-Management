#include <iostream>
using namespace std;

// declaring HomeScreen() for use in basicNavigation()
void HomeScreen();

// sanitize character array input
void sanitizeCharAray(char memberName[], short arraySize)
{
    for (short i = 0; i < arraySize; ++i)
    {
        if (memberName[i] > 64)
        {
            memberName[i] = tolower(memberName[i]);
        }
    }
}

// display character array in uppercase
void displayNameInUpper(const char memberName[], short arraySize)
{
    cout << "\nName: ";
    for (short i = 0; i < arraySize; ++i)
    {
        cout << char(toupper(memberName[i]));
    }
}

// validates char input taken from user
void validateCharInput(char& userInput, char expectedInput[], short expectedInputSize)
{
    short flag;
    userInput = tolower(userInput);
    do
    {
        flag = 0;
        for (short i = 0; i < expectedInputSize; i++)
        {
            if (userInput == expectedInput[i])
            {
                flag++;
                break;
            }
        }
        if (flag == 0)
        {
            cout << "\nSorry, We received a wrong input. Please enter a valid choice: \n=> ";
            cin >> userInput;
        }
    } while (flag == 0);
}

// verifies teacher subject code, checks if it is between 1 to 5 or not
void teacherSubjectCodeVerifier(short teacherSubjectCode[], short inputAtIndex)
{
    while (teacherSubjectCode[inputAtIndex] < 1 || teacherSubjectCode[inputAtIndex] > 5)
    {
        cout << "Sorry, we received a wrong subject code. Please enter subject code between 1 to 5: ";
        cin >> teacherSubjectCode[inputAtIndex];
    }
}

// Sets teacher's subject by using subject code; 1. Science   2. Maths    3. English  4. Hindi    5. Social Studies
void setTeacherSubjects(short teacherSubjectCode[], char teacherSubjects[])
{
    for (int i = 0; i < 3; i++)
    {
        if (teacherSubjectCode[i] == 1)
        {
            strcat_s(teacherSubjects, sizeof teacherSubjects, "Science ");
        }
        else if (teacherSubjects[i] == 2)
        {
            strcat_s(teacherSubjects, sizeof teacherSubjects, "Maths ");
        }
        else if (teacherSubjects[i] == 3)
        {
            strcat_s(teacherSubjects, sizeof teacherSubjects, "English ");
        }
        else if (teacherSubjects[i] == 4)
        {
            strcat_s(teacherSubjects, sizeof teacherSubjects, "Hindi ");
        }
        else if (teacherSubjects[i] == 5)
        {
            strcat_s(teacherSubjects, sizeof teacherSubjects, "Social Studies ");
        }
    }
}

// basic navigation system
// press 'H' to go back to home screen or 'Q' to quit
void basicNavigation()
{
    char userChoice;
    cout << "\n\n=> Press 'H' to go back to home screen or 'Q' to quit: ";
    cin >> userChoice;
    while (tolower(userChoice) != 'h' && tolower(userChoice) != 'q')
    {
        cout << "Sorry, we received a wrong input. Please re-enter ('H' -> Home Screen or 'Q' -> Quit): ";
        cin >> userChoice;
    }
    switch (tolower(userChoice))
    {
    case 'h':
        HomeScreen();
        break;
    case 'q':
        exit(0);
        break;
    }
}