#ifndef PLAYER_H
#define PLAYER_H
#include <windows.h>
#include <string>
#include <fstream>

using namespace std;

struct Player {
	string name;
	int record;
	int stage; // highest unlocked stage (mode)
};

// linked list to store players
struct Node {
	Player info;
	Node* plNext;
	Node(string plName) {
		info.name = plName;
		info.record = 0;
		info.stage = 0;
		plNext = NULL;
	}
};

void getPlayersList(Node* &);
void addheadPlayer(string, Node*&);
void savePlayersList(Node*);
void deletePlayersList(Node*);
void sortPlayersList(Node*);
Node* findPlayer(string, Node*);
#endif
