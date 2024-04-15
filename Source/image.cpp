#include "image.h"

void drawBackground() {
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(console, 3);
	for (int i = 0; i < 500; i++) {
		movetoPos(rand() % 200, rand() % 50);
		cout << ".";
		if (i % 50 == 0) Sleep(1);
	}
	SetConsoleTextAttribute(console, 11);
	for (int i = 0; i < 50; i++) {
		movetoPos(rand() % 200, rand() % 50);
		cout << "/";
		if (i % 20 == 0) Sleep(1);
	}
	SetConsoleTextAttribute(console, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
}

void drawLogo() {
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	ifstream ifs;
	// print in the center of the row
	string s = "|_   _|| |_| || ===|   |  \\/  | / () \\|_   _|/ (__`| |_| || ||  \\| |/ (_,`   / (_,` / () \\ |  \\/  || ===|";
	int len = s.size();
	int x = 3;
	int color[6] = { 1, 3, 4, 9, 11, 14 };
	ifs.open("logo.txt");
	while (getline(ifs, s)) {
		movetoPos((yMax - len) / 2, x++);
		SetConsoleTextAttribute(console, color[rand() % 6]);
		for (int i = 0; i < s.size(); i++) {
			cout << s[i];
		}
		Sleep(1);
	}
	ifs.close();
	SetConsoleTextAttribute(console, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
}

void drawYouWon() {
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	ifstream ifs;
	ifs.open("youwon.txt");
	int color[6] = { 1, 3, 4, 9, 11, 14 };
	string s = " _____      _____        _____     ____   ____         _____                   _____  _____   ______   ";
	int len = s.size();
	int x = 13;
	drawBackground();
	while (getline(ifs, s)) {
		movetoPos((yMax - len) / 2, x++);
		SetConsoleTextAttribute(console, color[rand() % 6]);
		for (int i = 0; i < s.size(); i++) {
			cout << s[i];
		}
		Sleep(1);
	}
	ifs.close();
}

void drawYouLost() {
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	ifstream ifs;
	ifs.open("youlost.txt");
	int color[6] = { 1, 3, 4, 9, 11, 14 };
	string s = " _____      _____        _____     ____   ____        ____                _____             ______   _________________ ";
	int len = s.size();
	int x = 13;
	drawBackground();
	while (getline(ifs, s)) {
		movetoPos((yMax - len) / 2, x++);
		SetConsoleTextAttribute(console, color[rand() % 6]);
		for (int i = 0; i < s.size(); i++) {
			cout << s[i];
		}
		Sleep(1);
	}
	ifs.close();
}

void printInstruction() {
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(console, BACKGROUND_BLUE);
	SetConsoleTextAttribute(console, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
	printStrPos(140, 31, "Press < ^ v > to move.          ");
	printStrPos(140, 32, "Press ENTER to select.          ");
	printStrPos(140, 33, "Press ENTER again to deselect.  ");
	printStrPos(140, 34, "Press W to get hint.            ");
	printStrPos(140, 35, "Press Q to quit.                ");
}

// print a string on screen at a specific position
void printStrPos(int y, int x, string s) {
	movetoPos(y, x);
	cout << s;
}

// print an int number on screen at a specific position
void printNumPos(int y, int x, int n) {
	movetoPos(y, x);
	cout << n;
}

void congrats(int scr) {
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(console, 10);
	string word[4] = { "Good!", "Great!", "Awesome!", "Nice!" };
	printStrPos(140, 14, word[rand() % 4]);
	printStrPos(150, 14, "+");
	printNumPos(151, 14, scr);
	Sleep(500);
	printStrPos(140, 14, "                   ");
}

// create option box and display on screen
int makeOptions(string options[], int color[], int n, int limit, int pos, int width, int initOpt) {
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(console, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
	for (int i = 0; i < n * 2 + 1; i++) {
		movetoPos((yMax - width) / 2 - 2, pos + i - 1);
		for (int j = -2; j < width + 2; j++)
			cout << " ";
	}
	SetConsoleTextAttribute(console, BACKGROUND_BLUE);
	movetoPos((yMax - width) / 2 - 1, pos - 2);
	for (int i = -1; i < width + 1; i++) {
		cout << " ";
	}
	movetoPos((yMax - width) / 2 - 1, pos + (n * 2));
	for (int i = -1; i < width + 1; i++) {
		cout << " ";
	}
	for (int i = -2; i < n * 2 + 1; i++) {
		movetoPos((yMax - width) / 2 - 2, pos + i);
		cout << " ";
		movetoPos((yMax + width) / 2 + 1, pos + i);
		cout << " ";
	}
	int option = initOpt;
	while (true) {
		for (int i = 0; i < n; i++) {
			if (i >= limit) SetConsoleTextAttribute(console, FOREGROUND_INTENSITY);
			else SetConsoleTextAttribute(console, color[i]);
			string s1, s2;
			if (i == option) {
				s1 = ">";
				s2 = "<";
			}
			else {
				s1 = " ";
				s2 = " ";
			}
			printStrPos((yMax - width) / 2, pos + i * 2, s1);
			printStrPos((yMax + width) / 2 - 1, pos + i * 2, s2);
			int len = options[i].size();
			printStrPos((yMax - len) / 2, pos + i * 2, options[i]);
		}
		int ch = _getch();
		switch (ch) {
		case KEY_UP:
			if (option > 0) {
				option--;
//				PlaySound(TEXT("move.wav"), NULL, SND_FILENAME | SND_ASYNC);
			}
			break;
		case KEY_DOWN:
			if (option < limit - 1) {
				option++;
//				PlaySound(TEXT("move.wav"), NULL, SND_FILENAME | SND_ASYNC);
			}
			break;
		default:
			break;
		}
		if (ch == KEY_ENTER) {
//			PlaySound(TEXT("choose.wav"), NULL, SND_FILENAME | SND_ASYNC);
			break;
		}
	}
	SetConsoleTextAttribute(console, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
	return option;
}
