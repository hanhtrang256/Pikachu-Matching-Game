#ifndef ENGINE_H
#define ENGINE_H
#include <windows.h>
#include <mmsystem.h>
#include <algorithm>
#include <cstdlib>
#include <queue>
#include "box.h"
#include "image.h"

#pragma comment(lib, "winmm.lib")

#define KEY_ENTER 13
#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_LEFT 75
#define KEY_RIGHT 77

#define pb push_back
#define F first
#define S second

#define BOARD_WIDTH 8
#define BOARD_HEIGHT 5
#define offset 8


using namespace std;

// 0 -> up
// 1 -> down
// 2 -> left
// 3 -> right
struct Queue_Data {
	pair<int, int> pos;
	int dir;
	int change;
	Queue_Data(pair<int, int> _pos = { 0, 0 }, int _dir = 0, int _change = 0) {
		pos = _pos;
		dir = _dir;
		change = _change;
	}
};

struct cmp {
	bool operator()(const Queue_Data& x, const Queue_Data& y) {
		return x.change > y.change;
	}
};

char** setupBoard();
void shuffleBoard(int, int);
void deleteBoard();
void readPair(pair<int, int>&);
int Rand(int, int);
bool validPos(const pair<int, int>&);
bool canGo(int, int);
int findNewDir(int, int, int, int);
void tracing(pair<int, int>, pair<int, int>, int, int);
int checkMatching(pair<int, int>, pair<int, int>, bool);
bool checkWin(int);
bool Input(int);
void startGame(Node*, Node*, int, string[], int[]);
void setupGame(Node*&);
#endif
