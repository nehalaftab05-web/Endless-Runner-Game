#include <iostream>
#include <conio.h> // library for _kbhit() and _getch()
#include <windows.h> // library for Sleep() and SetConsoleTextAttribute()
#include <fstream> // library for file handling
#include <cstdlib> // library for rand() and srand()
#include <ctime> // library for time()
#include <string> // library for string

using namespace std;

const int Map_width = 30, Map_height = 20;
const char Player = 'P', Coin = 'C', Empty = ' ', Wall = '#', Enemy_P = 'E'; // Yahan pe constants define kiye gaye hain jo game me use honge and const se wo apni value change nahi kar sakte
const char Obstacle = 219;

int P_X = Map_width / 2; // Player ka initial position center me hai
int P_Y = Map_height - 2; // Player ka initial position bottom me hai
int E_Y = -1, E_X = -1; // Enemy ka initial position -1 par hai taaki wo nahi aaye
int score = 0, lives = 3; // Player ka score aur lives ki initial value set ki gayi hai
int playerDist = 0;
bool gameOver = false;
char map[Map_height][Map_width]; // Map ko 2D array me define kiya gaya hai
string Name;

void setColor(int color) // Ye function console ka color change karne ke liye hai
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void setupMap() 
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
    system("cls");
    for (int i = 0; i < Map_height; ++i) 
    {
        for (int j = 0; j < Map_width; ++j) 
        {
            if (i == P_Y && j == P_X) 
            {
                setColor(10); cout << Player;
            }
            else if (playerDist >= 2000 && i == E_Y && j == E_X) 
            {
                setColor(12); cout << Enemy_P;
            }
            else 
            {
                if (map[i][j] == Obstacle) 
                { 
                    setColor(8); cout << Obstacle; 
                }
                else if 
                (map[i][j] == Coin) 
                { 
                    setColor(14); cout << Coin; 
                }  
                else if 
                (map[i][j] == Wall) 
                { 
                    setColor(7); cout << Wall; 
                }   
                else 
                {
                    cout << ' ';
                }
            }
        }
        cout << endl;
    }
    setColor(7);
    cout << "Score: "; setColor(11); cout << score;
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

void genObst_Coins() // ye function map ke first row me obstacles aur coins generate karne ke liye hai
{
    for (int j = 1; j < Map_width - 1; ++j) // 1 se lekar Map_width-1 tak ke columns me obstacles aur coins generate karte hain
    {
        if (rand() % 10 < 1) // ye condition 10 me se 1 baar true hoti hai, toh obstacle generate hota hai
        {
            map[0][j] = Obstacle;  
        }
        else if (rand() % 20 == 0) // ye condition 20 me se 1 baar true hoti hai, toh coin generate hota hai
        {
            map[0][j] = Coin;
        }
        else 
        {
            map[0][j] = Empty; // agar koi obstacle ya coin nahi hai toh empty space rakhte hain
        }
    }
}


void scrollMap() // ye function map ko scroll karne ke liye hai, taaki player ko aage continious chalne ki feel aaye
{
    for (int i = Map_height - 1; i > 0; --i) // Map_height-1 se lekar 1 tak ke rows ko scroll karte hain
    {
        for (int j = 1; j < Map_width - 1; ++j) // 1 se lekar Map_width-1 tak ke columns ko scroll karte hain
        {
            map[i][j] = map[i - 1][j]; // current row ko previous row se fill karte hain
        }
    }
    genObst_Coins(); // naye obstacles aur coins generate karte hain first row me
}

void updateEnemy() // ye function enemy ko update karne ke liye hai, taaki wo player ke peeche aaye
{
    if (playerDist < 2000) // agar player ki distance 2000 se kam hai toh enemy nahi aata
    {
        return;
    }

    if (E_Y < 0) // agar enemy ka position -1 hai toh wo player ke position par aata hai
    {
        E_X = P_X;
        E_Y = 0;
    }
    else 
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
        
        if (E_Y == P_Y && E_X == P_X) // agar enemy ka position player ke position par aa jata hai toh player ko ek life kam hoti hai
        {
            lives--;
            E_Y = -1;
        }
    }
}

void updateGame(char n) // ye function game ko update karne ke liye hai, taaki player ke input ke according game chale
{
    switch (n) 
    {
        case 'a': 
        if (P_X > 1) 
        {
            P_X--; 
        }
        break;
        
        case 'd': 
        if (P_X < Map_width - 2) 
        {
            P_X++; 
        }
        break;
        
        case 'w': 
        if (P_Y > 1) 
        {
            P_Y--; 
        }
        break;
        
        case 's': 
        if (P_Y < Map_height - 2) 
        {
            P_Y++; 
        }
        break;
    }

    if (map[P_Y][P_X] == Obstacle) // agar player kisi obstacle se collide krta hai toh uski life kam hoti hai
    {
        lives--;
        map[P_Y][P_X] = Empty;
    }
    else if (map[P_Y][P_X] == Coin) // agar player kisi coin se collide krta hai toh uska score badhta hai
    {
        score += 10;
        map[P_Y][P_X] = Empty;
    }
    
    if (lives <= 0) // agar player ki life 0 ho jati hai toh game over hota hai
    {
        gameOver = true;
    }
}

void GameOverScreen() // It just displays the game over screen and waits for a key press in the last
{
   
    system("cls");
    cout << "\n\n\n\n\n";
    setColor(12);
    cout << "\t\t" << "=== GAME OVER ===\n";
    Sleep(300);
    
    setColor(7);
}

void saveScore() // It saves the high score in a file
{
    ofstream file("highscores.txt", ios::app);
    file << Name << "," << score << "," << playerDist << "\n";
    file.close();
}

void viewScore() 
{
    system("cls");
    cout << "--- High Scores ---\n";
    ifstream file("highscores.txt");
    string line;
    while (getline(file, line)) cout << line << endl;
    file.close();
    cout << "\nPress any key to return to menu...";
    _getch();
}

void runGame() 
{
    P_X = Map_width / 2; // Player ka initial position set karte hain center me
    P_Y = Map_height - 2; // Player ka initial position set karte hain bottom me
    E_Y = E_X = -1; // Enemy ka initial position set karte hain -1 par taaki wo nahi aaye 
    score = 0;
    lives = 3;
    playerDist = 0;
    gameOver = false;

    int speed = 300;

    setupMap();
    genObst_Coins();

    while (!gameOver) 
    {
        drawMap();

        if (_kbhit()) updateGame(_getch()); //_kbhit() checks if a key was pressed & _getch() gets the key
        else updateGame(' ');

        scrollMap();
        updateEnemy();
        playerDist += 50; // Player ki distance ko 50 se increase krte hain har frame me

        if (playerDist >= 6000) speed = 100; // Speed increase as player distance increases
        else if (playerDist >= 4000) speed = 150;
        else if (playerDist >= 2000) speed = 200;
        else if (playerDist >= 1000) speed = 250;

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
    cout << "\nPress any key to return to menu...";
    _getch();
}

void showMenu() 
{
    int temp;
    do 
    {
        system("cls");
        setColor(9);
        cout << "==== Endless Runner Game ====\n";
        setColor(7);
        cout << "1) Start Game\n";
        cout << "2) View High Scores\n";
        cout << "3) Exit\n";
        cout << "Choose option: ";
        cin >> temp;
        cin.ignore();
        switch (temp) 
        {
            case 1:
            {
                cout << "Enter your name: ";
                getline(cin, Name);
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
                cout << "Exiting...\n";
                break;
            }
            default:
            {
                cout << "Invalid!!!";
                _getch();
            }
        }
    } 
    while (temp != 3);
}

int main() 
{
    srand(time(0));
    showMenu();
    return 0;
}
