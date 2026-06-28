#include <iostream>
#include <conio.h>
#include <windows.h>
#include "game.h"

using namespace std;

void showMenu()
{
    int n;
    do
    {
        system("cls");
        setColor(13);
        cout << "---- ENDLESS RUNNER ----\n\n";
        setColor(7);
        cout << "1) Play Game\n";
        cout << "2) View High Scores\n";
        cout << "3) Exit\n\n";
        cout << "Enter your choice: ";
        cin >> n;

        cin.ignore();
        switch (n)
        {
        case 1:
        {
            do
            {
                cout << "Enter your name: ";
                getline(cin, Name);
                if (find_Name(Name))
                {
                    cout << "Name already Exists!!!\n";
                }
            } while (find_Name(Name));

            runGame();

            break;
        }
        case 2:
        {
            viewScore();
            break;
        }
        case 3:
        {
            cout << "Exiting!!!\n";
            break;
        }
        default:
        {
            cout << "Invalid!!!\n";
            _getch();
        }
        }
    } while (n != 3);
}

int main()
{
    srand(time(0));
    hideCursor();
    showMenu();
    return 0;
}
