#ifndef CONSOLE_H
#define CONSOLE_H
#include <windows.h>
#include <ctime>

#define yMax 200
#define xMax 45

using namespace std;

void initConsoleWindow(int, int);
void resizeConsoleWindow(int, int);
void disableEcho();
void enableEcho();
void showCursor(bool);
void movetoPos(int, int);
#endif // CONSOLE_H
