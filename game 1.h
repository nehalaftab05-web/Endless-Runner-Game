#pragma once
#include <string>

const int Map_width = 30, Map_height = 20;
const char Player = 'P', Coin = 'C', Empty = ' ', Wall = '#', Enemy_P = 'E';
const char Obstacle = 219;

extern int P_X, P_Y, E_X, E_Y, score, lives, playerDist;
extern bool gameOver, isJump, isSlide;
extern int jumpC, slide_C;
extern char map[Map_height][Map_width];
extern std::string Name;

void setColor(int color);
void setCursorPosition(int x, int y);
void hideCursor();

void setupMap();
void drawMap();
void genObst_Coins();
void scrollMap();
void updateEnemy();
void updateGame(char n);
void GameOverScreen();
void saveScore();
void viewScore();
void runGame();
bool find_Name(const std::string& name);
void showMenu();
