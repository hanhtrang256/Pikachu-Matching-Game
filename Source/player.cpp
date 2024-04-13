#include "player.h"

// get all saved players from file
void getPlayersList(Node* &plHead) {
	ifstream ifs;
	ifs.open("players.txt");
	string plName = "";
	while (ifs >> plName) {
		addheadPlayer(plName, plHead);
		int num;
		ifs >> num;
		plHead->info.record = num;
		ifs >> num;
		plHead->info.stage = num;
	}
	ifs.close();
}

// add player to list
void addheadPlayer(string plName, Node*& plHead) {
	Node* plNode = new Node(plName);
	if (plHead == NULL)
		plHead = plNode;
	else {
		Node* plCur = plHead;
		plNode->plNext = plCur;
		plHead = plNode;
	}
}

// save list of players to a txt file
void savePlayersList(Node* plHead) {
	ofstream ofs;
	ofs.open("players.txt", ofstream::out | ofstream::trunc);
	Node* plCur = plHead;
	while (plCur != NULL) {
		ofs << plCur->info.name << " " << plCur->info.record << " " << plCur->info.stage << endl;
		plCur = plCur->plNext;
	}
	ofs.close();
}

// deallocate linked list
void deletePlayersList(Node* plHead) {
	while (plHead != NULL) {
		Node* plCur = plHead->plNext;
		delete plHead;
		plHead = plCur;
	}
}


// sort list descending by record (score)
void sortPlayersList(Node* plHead) {
	if (plHead == NULL || plHead->plNext == NULL)
		return;
	Node* pl1 = plHead;
	while (pl1->plNext) {
		Node* pl2 = pl1->plNext;
		while (pl2 != NULL) {
			if (pl1->info.record < pl2->info.record) {
				string stmp = pl1->info.name;
				pl1->info.name = pl2->info.name;
				pl2->info.name = stmp;
				int ntmp = pl1->info.record;
				pl1->info.record = pl2->info.record;
				pl2->info.record = ntmp;
				ntmp = pl1->info.stage;
				pl1->info.stage = pl2->info.stage;
				pl2->info.stage = ntmp;
			}
			pl2 = pl2->plNext;
		}
		pl1 = pl1->plNext;
	}
}


// find player by name
Node* findPlayer(string plName, Node* plHead) {
	Node* plCur = plHead;
	while (plCur != NULL && plCur->info.name != plName) {
		plCur = plCur->plNext;
	}
	return plCur;
}