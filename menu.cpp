#include "menu.h"

void createMenu(Node* plHead) {
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	int option = 0;
	while (true) {
		system("cls"); // clear console
		drawBackground();
		drawLogo();
		// menu option box
		string options[4] = { "Play", "View Leaderboard", "About us", "Exit" };
		int color[4] = { 14, 11, 9, 4 };
		option = makeOptions(options, color, 4, 4, 28, 20, option);
		if (option == 0) setupGame(plHead);
		else if (option == 1) viewLeaderboard(plHead);
		else if (option == 2) viewInfo();
		else if (exitGame(plHead)) break;
	}
}

void viewLeaderboard(Node* plHead) {
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	system("cls");
	drawBackground();
	drawLogo();
	string s = "LEADERBOARD";
	int len = s.size();
	printStrPos((yMax - len) / 2, 20, s);
	printStrPos(40, 22, "NAME");
	printStrPos(yMax / 2 + 10, 22, "RECORD");
	sortPlayersList(plHead); // sort list ascending
	int i = 0;
	Node* plCur = plHead;
	while (plCur != NULL && i < 10) {
		printStrPos(40, 24 + i, plCur->info.name);
		printNumPos(yMax / 2 + 10, 24 + i, plCur->info.record);
		plCur = plCur->plNext;
		i++;
	}
	string options[1] = { "Back" };
	int color[1] = { 14 };
	int option = makeOptions(options, color, 1, 1, xMax - 5, 10, 0);
}

void viewInfo() {
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	system("cls");
	drawBackground();
	drawLogo();
	printStrPos(8, 23, "                                                     ");
	printStrPos(8, 24, "     [CSC10002] PROJECT: Pikachu Matching Game            ");
	printStrPos(8, 25, "     Class: 23CLC08                                       ");
	printStrPos(8, 26, "                                                          ");
	printStrPos(8, 27, "     23127071 - NGUYEN Tan Huy Khoi                       ");
	printStrPos(8, 28, "     23127226 - NGUYEN Dang Minh                          ");
	printStrPos(8, 29, "                                                     ");
	string options[1] = { "Back" };
	int color[1] = { 14 };
	int option = makeOptions(options, color, 1, 1, xMax - 5, 10, 0);
}

bool exitGame(Node* plHead) {
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	system("cls");
	drawBackground();
	drawLogo();
	string s = "Are you sure you want to exit?";
	int len = s.size();
	printStrPos((yMax - len) / 2, 22, s);
	string options[2] = { "Sure", "No" };
	int color[2] = { 11, 4 };
	int option = makeOptions(options, color, 2, 2, 26, 10, 1);
	if (option == 0) {
		savePlayersList(plHead);
		deletePlayersList(plHead);
		system("cls");
		return true;
	}
	return false;
}