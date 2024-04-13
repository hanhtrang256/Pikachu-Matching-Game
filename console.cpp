#include "console.h"

void initConsoleWindow(int width, int height) {
	resizeConsoleWindow(width, height);
	disableEcho();
	showCursor(false);
	srand(time(NULL));
}

void resizeConsoleWindow(int width, int height) {
	HWND console = GetConsoleWindow(); // retrieves a handle to the console window
	RECT rect; // declares a structure used to store the coordinates of the console window
	GetWindowRect(console, &rect); // retrieves and stores the dimensions of the console window
	// moves and resizes the console window to the top-left corner with the new width and height of the window
	MoveWindow(console, rect.left, rect.top, width, height, TRUE); }

void disableEcho() {
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE); // retrieves a handle to the console screen
	DWORD mode = 0;
	GetConsoleMode(console, &mode);
	// disable character echo
	mode &= ~ENABLE_ECHO_INPUT;
	SetConsoleMode(console, mode);
}

void enableEcho() {
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	DWORD mode = 0;
	GetConsoleMode(console, &mode);
	mode |= ENABLE_ECHO_INPUT;
	SetConsoleMode(console, mode);
}

// hide or show the cursor
void showCursor(bool show) {
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cursor;
	GetConsoleCursorInfo(console, &cursor);
	// true: visible
	// false: invisible
	cursor.bVisible = show;
	SetConsoleCursorInfo(console, &cursor);
}


// move cursor to specific position
void movetoPos(int y, int x) {
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD position = { y, x };
	SetConsoleCursorPosition(console, position);
}