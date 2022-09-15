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

// validates marks
void validateSubjectMarks(short marks[], short index)
{
    while (marks[index] < 0 || marks[index] > 100)
    {
        cout << "Looks like you have entered an invalid response. Please enter marks again (0 - 100): ";
        cin >> marks[index];
    }
}

// validate number of days
void validateDays(short& days)
{
    while (days > 366 || days < 1)
    {
        cout << "Looks like you have entered an invalid response. Please enter number of days again (1 - 366): ";
        cin >> days;
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