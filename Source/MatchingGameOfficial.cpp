#include "MatchingGameOfficial.h"

using namespace std;

int main(int argc, char* argv[]) {
	initConsoleWindow(yMax, xMax);
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	Node* plHead = NULL;
	getPlayersList(plHead);
	getAlphabet();
	getBoardBackground();
	createMenu(plHead);
}
