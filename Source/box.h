#ifndef BOX_H
#define BOX_H
#include <windows.h>
#include <mmsystem.h>
#include "engine.h"

#pragma comment(lib, "winmm.lib")

#define F first
#define S second

using namespace std;

void fillBoxColor(int, int);
void fillBoxCharacter(int, int, char**);
void drawBox(int, int, char**);
void drawPath(bool, vector<pair<int, pair<int, int>>>);
void drawBoxBackground(int, int);
void deleteBox(int, int);
void chosenBox(int, int, char**);
void normalBox(int, int, pair<int, int>, char**);
void highlightBox(int, int, char**);
void invalidBox(int, int, char**);
void validBox(int, int, char**);
void hintBox(int, int, char**);
void printBoard(char**);
void clearBoard(char**, int, int, pair<int, int>);
void getBoardBackground();
void getAlphabet();
#endif