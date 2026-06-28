#include <iostream>
#include <conio.h>
#include <windows.h>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <string>
#include "game.h"

using namespace std;

int P_X = Map_width / 2, P_Y = Map_height - 2; // Player coordinates
int E_Y = -1, E_X = -1; // Enemy coordinates
int score = 0, lives = 3; // Player score and lives
int playerDist = 0;
bool gameOver = false;
bool isJump = false;
int jumpC = 0;
bool isSlide = false;
int slide_C = 0;
char map[Map_height][Map_width]; // array to hold the map 
string Name;


void setColor(int color)
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void setCursorPosition(int x, int y)
{
    COORD crd = { (SHORT)x, (SHORT)y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), crd);
}

void hideCursor()
{
    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(out, &cursorInfo);
    cursorInfo.bVisible = false;
    SetConsoleCursorInfo(out, &cursorInfo);
}

void setupMap()  // Ye function map ko setup karne ke liye hai
{
    for (int i = 0; i < Map_height; ++i)
    {
        for (int j = 0; j < Map_width; ++j)
        {
            if (j == 0 || j == Map_width - 1)
            {
                map[i][j] = Wall;
            }
            else
            {
                map[i][j] = Empty;
            }
        }
    }
}

void drawMap()
{
    setCursorPosition(0, 0); // Prevent flickering by drawing from top-left

    for (int i = 0; i < Map_height; ++i)
    {
        for (int j = 0; j < Map_width; ++j)
        {
            if (i == P_Y && j == P_X)
            {
                setColor(10);
                if (isJump)
                {
                    cout << '^';
                }
                else if (isSlide)
                {
                    cout << 'v';
                }
                else
                {
                    cout << Player;
                }
                setColor(7);
            }
            else if (playerDist >= 2000 && i == E_Y && j == E_X) // Enemy ka position
            {
                setColor(12);
                cout << Enemy_P;
                setColor(7);
            }
            else if (map[i][j] == Coin)
            {
                setColor(14);
                cout << Coin;
                setColor(7);
            }
            else if (map[i][j] == Obstacle)
            {
                setColor(8);
                cout << Obstacle;
                setColor(7);
            }
            else if (map[i][j] == Wall)
            {
                setColor(13);
                cout << Wall;
                setColor(7);
            }
            else
            {
                cout << map[i][j];
            }
        }
        cout << '\n';
    }


    setColor(7);
    cout << "Score: ";
    setColor(11);
    cout << score;

    setColor(7);
    cout << "\tLives: ";
    setColor(12);
    cout << lives;

    setColor(7);
    cout << "\tDistance: ";
    setColor(13);
    cout << playerDist << endl;

    setColor(7);
}

void genObst_Coins() // Ye function map ke first row me obstacles aur coins generate karne ke liye hai
{
    for (int j = 1; j < Map_width - 1; ++j) // 1 se lekar Map_width-1 tak ke columns me obstacles aur coins generate karte hain
    {
        if (rand() % 10 < 1) // 10 me se 1 baar obstacle generate hota hai
        {
            map[0][j] = Obstacle;
        }
        else if (rand() % 20 == 0) // 20 me se 1 baar coin generate hota hai
        {
            map[0][j] = Coin;
        }
        else // agar koi obstacle ya coin nahi hai toh empty space rakhte hain
        {
            map[0][j] = Empty;
        }
    }
}

void scrollMap() // Ye function map ko scroll karne ke liye hai
{
    for (int i = Map_height - 1; i > 0; --i)
    {
        for (int j = 1; j < Map_width - 1; ++j)
        {
            map[i][j] = map[i - 1][j]; // current row ko previous row se fill karte hain
        }
    }
    genObst_Coins(); // naye obstacles aur coins generate karte hain first row me
}

void updateEnemy() // Ye function enemy ko update karne ke liye hai
{
    if (playerDist < 2000) return; // agar player ki distance 2000 se kam hai toh enemy nahi aata

    if (E_Y < 0) // agar enemy ka position -1 hai toh wo player ke position par aata hai
    {
        E_X = P_X;
        E_Y = 0;
    }
    else // agar enemy ka position 0 se bada hai toh wo player ke position ki taraf move karta hai
    {
        if (E_X < P_X) // agar enemy ka position player ke position se chhota hai toh wo right ki taraf move karta hai
        {
            E_X++;
        }
        else if (E_X > P_X) // agar enemy ka position player ke position se bada hai toh wo left ki taraf move karta hai
        {
            E_X--;
        }

        E_Y++;

        // Check for collision
        if (E_Y == P_Y && E_X == P_X)
        {
            if (isJump || isSlide)
            {
                // Player dodges the enemy
                E_Y = -1;  // Reset enemy
            }
            else
            {
                lives--;   // Hit by enemy
                E_Y = -1;
            }
        }
    }
}


void updateGame(char n) // Ye function game ko update karne ke liye hai, taaki player ke input ke according game chale
{
    if (jumpC > 0) // Jump counter ko decrement karte hain
    {
        jumpC--;
        if (jumpC == 0)
        {
            isJump = false;
        }
    }

    if (slide_C > 0) // Slide counter ko decrement karte hain
    {
        slide_C--;
        if (slide_C == 0)
        {
            isSlide = false;
        }
    }

    switch (n)
    {
    case 'a':
    {
        if (P_X > 1) P_X--;
        break;
    }
    case 'd':
    {
        if (P_X < Map_width - 2) P_X++;
        break;
    }
    case 'w':
    {
        if (!isJump && !isSlide) // Jump karne ke liye
        {
            isJump = true; // Jumping player
            jumpC = 3; // Jump counter set karte hain
        }
        break;
    }
    case 's':
    {
        if (!isSlide && !isJump)
        {
            isSlide = true; // Sliding player
            slide_C = 3; // Slide counter set karte hain
        }
        break;
    }
    }

    if (map[P_Y][P_X] == Obstacle) // agar player kisi obstacle se collide krta hai toh uski life kam hoti hai
    {
        if (!isJump && !isSlide) // agar player jump ya slide nahi kar raha hai toh life kam hoti 
        {
            lives--;
        }
        map[P_Y][P_X] = Empty; // obstacle ko empty space se replace karte hain
    }
    else if (map[P_Y][P_X] == Coin) // agar player kisi coin se collide krta hai toh uska score badhta hai
    {
        score += 10;
        map[P_Y][P_X] = Empty;
    }

    if (lives <= 0) gameOver = true; // agar player ki life 0 ho jati hai toh game over hota hai
}

void GameOverScreen() // Ye function game over screen dikhata hai end pe
{
    system("cls");
    cout << "\n\n\n\n\n";
    setColor(12);
    cout << "\t\t=== GAME OVER ===\n";

    Sleep(300);
    setColor(7);
}

void saveScore() // Ye function score ko file me save karne ke liye hai
{
    ofstream file("highscores.txt", ios::app);
    file << Name << "," << score << "," << playerDist << "\n";
    file.close();
}

void viewScore()
{
    system("cls");
    setColor(13);
    cout << "\t==== High Scores ====\n\n";
    setColor(7);
    cout << "Name\t\tScore\t\tDistance\n";
    cout << "_________________________________________\n";

    const int max = 100; // maximum number of scores
    string names[max]; // array to store names
    int scores[max], dist[max]; // arrays to store scores and distances
    int n = 0;

    ifstream file("highscores.txt");
    string line;

    while (getline(file, line) && n < max) // file se line by line read karte hain
    {
        size_t pos1 = line.find(','); // pehli comma ki position
        size_t pos2 = line.find(',', pos1 + 1); // dusri comma ki position

        if (pos1 != string::npos && pos2 != string::npos) // agar dono commas milte hain toh ye condition true hoti hai
        {
            names[n] = line.substr(0, pos1); // name ko extract karte hain
            scores[n] = stoi(line.substr(pos1 + 1, pos2 - pos1 - 1)); // score ko extract karte hain
            dist[n] = stoi(line.substr(pos2 + 1)); // distance ko extract karte hain
            n++;
        }
    }
    file.close();

    for (int i = 0; i < n - 1; ++i) // ye loop scores ko sort karne ke liye hai using bubble sort
    {
        for (int j = 0; j < n - i - 1; ++j)
        {
            if (scores[j] < scores[j + 1])
            {
                swap(scores[j], scores[j + 1]);
                swap(dist[j], dist[j + 1]);
                swap(names[j], names[j + 1]);
            }
        }
    }

    for (int i = 0; i < n; ++i) // Display sorted results
    {
        cout << names[i];
        if (names[i].length() < 8)
        {
            cout << "\t\t"; // 8 characters name ke liye tab space
        }
        else
        {
            cout << "\t"; // 8 se zyada characters name ke liye tab space
        }
        cout << scores[i] << "\t\t" << dist[i] << "\n";
    }

    cout << "\nTo return to menu, press any key >>>>";
    _getch();
}


void runGame()
{
    P_X = Map_width / 2; // Player ka initial position set karte hain center me
    P_Y = Map_height - 2; // Player ka initial position set karte hain bottom me
    E_Y = E_X = -1;
    score = 0;
    lives = 3;
    playerDist = 0;
    gameOver = false;

    isJump = false;
    jumpC = 0;
    isSlide = false;
    slide_C = 0;

    int speed = 250;

    setupMap();
    genObst_Coins();

    while (!gameOver)
    {
        drawMap();

        if (_kbhit()) // agar koi key press hoti hai toh updateGame function ko call karte
        {
            updateGame(_getch());
        }
        else // agar koi key press nahi hoti hai toh updateGame function ko space character ke sath call karte hain
        {
            updateGame(' ');
        }

        scrollMap();
        updateEnemy();
        playerDist += 50;

        if (playerDist >= 6000) // Speed increase as player distance increases
        {
            speed = 50;
        }
        else if (playerDist >= 4000)
        {
            speed = 100;
        }
        else if (playerDist >= 2000)
        {
            speed = 150;
        }
        else if (playerDist >= 1000)
        {
            speed = 200;
        }

        Sleep(speed);
    }

    GameOverScreen();

    cout << "\nFinal Score: ";
    setColor(11);
    cout << score;

    setColor(7);
    cout << "\tDistance: ";
    setColor(13);
    cout << playerDist << endl;
    setColor(7);

    saveScore();
    cout << "\nTo return to menu, press any key >>>>";
    _getch();
}

bool find_Name(const string& name) // Ye function check karta hai agar name already file me hai toh
{
    ifstream file("highscores.txt");
    string line;

    while (getline(file, line))
    {
        size_t pos = line.find(',');
        if (pos != string::npos)
        {
            string filename = line.substr(0, pos);
            if (filename == name)
            {
                return true; // agar name milta hai toh true return karte hain
            }
        }
    }
    return false;
}
