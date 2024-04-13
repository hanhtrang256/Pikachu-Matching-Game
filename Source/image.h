#ifndef IMAGE_H
#define IMAGE_H
#include <windows.h>
#include <mmsystem.h>
#include <iostream>
#include <conio.h>
#include "console.h"
#include "player.h"

#pragma comment(lib, "winmm.lib")

#define KEY_ENTER 13
#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_LEFT 75
#define KEY_RIGHT 77

using namespace std;

void drawLogo();
void drawBackground();
void drawYouWon();
void drawYouLost();
void printInstruction();
void printStrPos(int, int, string);
void printNumPos(int, int, int);
void congrats(int);
int makeOptions(string[], int[], int, int, int, int, int);
#endif
