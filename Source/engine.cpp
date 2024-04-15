#include "engine.h"

char** board;
bool pass[BOARD_HEIGHT + offset][BOARD_WIDTH + offset][4][4];
bool gameLoop, canMatch;
const int dx[] = { -1, 0, 0, 1 }, dy[] = { 0, -1, 1, 0 };
int px = 1, py = 1; // current box
int hint = 0;
pair<int, int> fc = { 0, 0 }, sc = { 0, 0 }; // first and second chosen boxes
vector<pair<int, pair<int, int>>> path;
Queue_Data trace[BOARD_HEIGHT + offset][BOARD_WIDTH + offset][4][4];
int score = 0;

char** setupBoard() {
	char** board = new char* [BOARD_HEIGHT + offset];
	for (int i = 0; i <= BOARD_HEIGHT + 1; ++i) board[i] = new char[BOARD_WIDTH + offset];
	int cnt[26];
	memset(cnt, 0, sizeof cnt);
	int total = BOARD_HEIGHT * BOARD_WIDTH;
	const int len = total;
	while (total > 0) {
		for (int i = 0; i < 26; ++i) {
			if (total <= 0) break;
			int num = 2 * Rand(0, 2);
			if (total >= num) {
				cnt[i] += num;
				total -= num;
			}
		}
	}

	char shuff[100];
	int index = 0;
	for (int i = 0; i < 26; ++i) {
		while (cnt[i] > 0) {
			shuff[index++] = (char)(i + 'A');
			--cnt[i];
		}
	}

	random_shuffle(shuff, shuff + len);
	index = 0;

	for (int i = 0; i <= BOARD_HEIGHT + 1; ++i) {
		for (int j = 0; j <= BOARD_WIDTH + 1; ++j) {
			if (i == 0 || i == BOARD_HEIGHT + 1) board[i][j] = ' ';
			else if (j == 0 || j == BOARD_WIDTH + 1) board[i][j] = ' ';
			else board[i][j] = shuff[index++];
		}
	}
	return board;
}

void shuffleBoard(int dem, int mode) {
	char shuff[100];
	int ind = 0;
	vector<pair<int, int>> pos;
	for (int i = 1; i <= BOARD_HEIGHT; ++i) {
		for (int j = 1; j <= BOARD_WIDTH; ++j) {
			if (board[i][j] != ' ')
				shuff[ind++] = board[i][j];
			pos.push_back({ i, j });
		}
	}
	random_shuffle(shuff, shuff + dem);
	random_shuffle(pos.begin(), pos.end());

	for (int i = 1; i <= BOARD_HEIGHT; ++i)
		for (int j = 1; j <= BOARD_WIDTH; ++j)
			board[i][j] = ' ';

	ind = 0;
	for (int i = 0; i < dem; ++i) {
		board[pos[ind].first][pos[ind].second] = shuff[i];
		++ind;
	}

	if (mode == 2) {
		for (int i = 1; i <= BOARD_HEIGHT; ++i) {
			for (int j = 1; j <= BOARD_WIDTH; ++j) {
				while (board[i][j] == ' ') {
					bool has = false;
					for (int k = j; k <= BOARD_WIDTH; ++k) if (board[i][k] != ' ') has = true;
					if (has == false) break;
					for (int k = j; k < BOARD_WIDTH; ++k) board[i][k] = board[i][k + 1];
					board[i][BOARD_WIDTH] = ' ';
				}
			}
		}
	}
}

void deleteBoard() {
	for (int i = 0; i <= BOARD_HEIGHT + 1; ++i) delete[] board[i];
	delete[] board;
}

void readPair(pair<int, int>& p) {
	cin >> p.F >> p.S;
}

int Rand(int from, int to) {
	//assert(from <= to);
	return from + rand() % (to - from + 1);
}

// check if position can be chosen to match
bool validPos(const pair<int, int>& p) {
	return p.F >= 1 && p.F <= BOARD_HEIGHT && p.S >= 1 && p.S <= BOARD_WIDTH;
}

// check if (x, y) can be traversed in bfs
bool canGo(int x, int y) {
	return x >= 0 && x <= BOARD_HEIGHT + 1 && y >= 0 && y <= BOARD_WIDTH + 1;
}

// find new direction
int findNewDir(int x, int y, int u, int v) { // (x, y): cur position,  (u, v): next position
	if (x == u) return (y < v ? 3 : 2);
	return (x < u ? 1 : 0);
}

void tracing(pair<int, int> tar, pair<int, int> fc, int dir, int change) {
	path.clear();
	do {
		path.pb({ dir, tar });
		int pdir = trace[tar.F][tar.S][dir][change].dir;
		int pchange = trace[tar.F][tar.S][dir][change].change;
		tar = trace[tar.F][tar.S][dir][change].pos;
		dir = pdir;
		change = pchange;
	} while (tar != fc);
	path.pb({ dir, fc });
}

int checkMatching(pair<int, int> fc, pair<int, int> sc, bool edit) {
	if ((!validPos(fc) || !validPos(sc) || board[fc.F][fc.S] == ' ' || board[sc.F][sc.S] == ' ' || board[fc.F][fc.S] != board[sc.F][sc.S])) {
		if (edit) {
//			PlaySound(TEXT("error.wav"), NULL, SND_FILENAME | SND_ASYNC);
			invalidBox(fc.F, fc.S, board);
			invalidBox(sc.F, sc.S, board);
			Sleep(500);
		}
		return 0;
	}
	// BFS to check valid
	memset(pass, false, sizeof pass);
	for (int i = 0; i <= BOARD_HEIGHT + 1; ++i)
		for (int j = 0; j <= BOARD_WIDTH + 1; ++j)
			for (int dir = 0; dir < 4; ++dir)
				for (int c = 0; c < 4; ++c)
					trace[i][j][dir][c] = Queue_Data();
	priority_queue<Queue_Data, vector<Queue_Data>, cmp> q;

	q.push(Queue_Data(fc, -1));

	while (!q.empty()) {
		pair<int, int> pos = q.top().pos;
		int dir = q.top().dir;
		int change = q.top().change;
		q.pop();

		if (pos == sc) {
			if (change < 3) {
				if (edit) board[fc.F][fc.S] = board[sc.F][sc.S] = ' ';
				canMatch = true;
				if (edit) tracing(sc, fc, dir, change);
				return change + 1;
			}
			continue;
		}

		for (int i = 0; i < 4; ++i) {
			int u = pos.F + dx[i];
			int v = pos.S + dy[i];
			if (canGo(u, v)) { // can reach to (u, v)
				int nw_dir = findNewDir(pos.F, pos.S, u, v); // new direction (can equal previous direction)
				int nw_change = change;
				if (nw_dir != dir && dir != -1) ++nw_change; // only update change if we change direction
				if (nw_change < 3) { // only push to queue if number of direction changes is < 3
					if (board[u][v] == ' ' && !pass[u][v][nw_dir][nw_change]) {
						pass[u][v][nw_dir][nw_change] = true;
						q.push(Queue_Data({ u, v }, nw_dir, nw_change));
						trace[u][v][nw_dir][nw_change] = Queue_Data(pos, dir, change);
					}
					else if (board[u][v] != ' ') {
						if (u == sc.F && v == sc.S && nw_change < 3) {
							if (edit) board[u][v] = board[fc.F][fc.S] = ' ';
							canMatch = true;
							trace[u][v][nw_dir][nw_change] = Queue_Data(pos, dir, change);
							if (edit) tracing(sc, fc, nw_dir, nw_change);
							return nw_change + 1;
						}
					}
				}
			}
		}
	}
	if (edit) {
//		PlaySound(TEXT("error.wav"), NULL, SND_FILENAME | SND_ASYNC);
		invalidBox(fc.F, fc.S, board);
		invalidBox(sc.F, sc.S, board);
		Sleep(500);
	}
	return 0;
}

// check if the board is cleared
bool checkWin(int mode) {
	int dem = 0;
	for (int i = 1; i <= BOARD_HEIGHT; ++i)
		for (int j = 1; j <= BOARD_WIDTH; ++j)
			if (board[i][j] != ' ')
				++dem;
	if (dem == 0) return true;
	for (int i = 1; i <= BOARD_HEIGHT; ++i) {
		for (int j = 1; j <= BOARD_WIDTH; ++j) {
			for (int x = 1; x <= BOARD_HEIGHT; ++x) {
				for (int y = 1; y <= BOARD_WIDTH; ++y) {
					if (i == x && j == y) continue;
					canMatch = false;
					int b = checkMatching({ i, j }, { x, y }, false);
					if (canMatch) return false;
				}
			}
		}
	}
	// if not cleared but no valid match left
	clearBoard(board, px, py, fc);
	shuffleBoard(dem, mode); // shuffle again
	px = py = 1;
	printBoard(board);
	highlightBox(px, py, board);
	return false;
}

bool Input(int mode) {
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	if (_kbhit()) { // if user press a key
		int ch = _getch();
		switch (ch) {
		case KEY_UP:
//			PlaySound(TEXT("move2.wav"), NULL, SND_FILENAME | SND_ASYNC);
			normalBox(px, py, fc, board);
			px = max(1, px - 1);
			break;
		case KEY_DOWN:
//			PlaySound(TEXT("move2.wav"), NULL, SND_FILENAME | SND_ASYNC);
			normalBox(px, py, fc, board);
			px = min(BOARD_HEIGHT, px + 1);
			break;
		case KEY_LEFT:
//			PlaySound(TEXT("move2.wav"), NULL, SND_FILENAME | SND_ASYNC);
			normalBox(px, py, fc, board);
			py = max(1, py - 1);
			break;
		case KEY_RIGHT:
//			PlaySound(TEXT("move2.wav"), NULL, SND_FILENAME | SND_ASYNC);
			normalBox(px, py, fc, board);
			py = min(BOARD_WIDTH, py + 1);
			break;
		// press hint
		case 'W':
		case 'w':
			if (score < 3) {
				SetConsoleTextAttribute(console, 4);
				printStrPos(140, 14, "Get at least 3 points to use hint");
				Sleep(1000);
				printStrPos(140, 14, "                                 ");
				break;
			}
			else if (hint >= 3) {
				SetConsoleTextAttribute(console, 4);
				printStrPos(140, 14, "You have used all 3 hints");
				Sleep(1000);
				printStrPos(140, 14, "                         ");
				break;
			}
			hint++;
			printStrPos(140, 12, "Hint: ");
			cout << hint << "/3";
			// show hint
			for (int i = 1; i <= BOARD_HEIGHT; ++i) {
				bool has = false;
				for (int j = 1; j <= BOARD_WIDTH; ++j) if (board[i][j] != ' ') {
					for (int u = 1; u <= BOARD_HEIGHT; ++u) {
						for (int v = 1; v <= BOARD_WIDTH; ++v) if (board[u][v] != ' ' && (u != i || v != j)) {
							int minus = checkMatching({ i, j }, { u, v }, false);
							if (minus > 0) {
								hintBox(i, j, board);
								hintBox(u, v, board);
								has = true;
								SetConsoleTextAttribute(console, 9);
								printStrPos(140, 14, "Hint used  -");
								cout << minus;
								Sleep(1000);
								printStrPos(140, 14, "                  ");
								score -= minus;
								normalBox(i, j, fc, board);
								normalBox(u, v, fc, board);
								break;
							}
						}
						if (has) break;
					}
					if (has) break;
				}
				if (has) break;
			}
			break;
		// quit game
		case 'Q':
		case 'q':
			return false;
			break;
		case KEY_ENTER:
//			PlaySound(TEXT("move2.wav"), NULL, SND_FILENAME | SND_ASYNC);
			if (board[px][py] != ' ') { // check if box is not empty
				if (fc.F == 0) { // first chosen box
					fc = { px, py };
				}
				else {
					sc = { px, py }; // second chosen box
					if (px == fc.F && py == fc.S) { // deselect
						int fx = fc.F, fy = fc.S;
						fc = { 0,0 };
						normalBox(fx, fy, fc, board);
					}
					else {
						int check = checkMatching(fc, sc, true); // check if can match and return score
						if (check == 0) { // invalid
							int fx = fc.F, fy = fc.S;
							fc = { 0,0 };
							normalBox(fx, fy, fc, board);
							normalBox(sc.F, sc.S, fc, board);
						}
						else { //valid
//							PlaySound(TEXT("ting.wav"), NULL, SND_FILENAME | SND_ASYNC);
							drawPath(false, path);
							validBox(fc.F, fc.S, board);
							validBox(sc.F, sc.S, board);
							congrats(check);
							drawPath(true, path);
							score += check; // update score
							if (mode == 1) {
								deleteBox(fc.F, fc.S);
								deleteBox(sc.F, sc.S);
								drawBoxBackground(fc.F, fc.S);
								drawBoxBackground(sc.F, sc.S);
							}
							else if (mode == 2) { //if hard mode
								int fx = fc.F, fy = fc.S, xf, yf, xs, ys;
								fc = { 0, 0 };
								if (fy >= sc.S) {
									xf = fx; // box 1 more to the right
									yf = fy;
									xs = sc.F; // box 2 more to the left
									ys = sc.S;
								}
								else {
									xs = fx;
									ys = fy;
									xf = sc.F;
									yf = sc.S;
								}
								// move row to left from box 1
								for (int i = yf; i < BOARD_WIDTH; ++i) {
									board[xf][i] = board[xf][i + 1];
									if (board[xf][i] == ' ') {
										deleteBox(xf, i);
										drawBoxBackground(xf, i);
									}

									else drawBox(xf, i, board);
								}
								board[xf][BOARD_WIDTH] = ' ';
								deleteBox(xf, BOARD_WIDTH);
								drawBoxBackground(xf, BOARD_WIDTH);
								// move row to left from box 2
								for (int i = ys; i < BOARD_WIDTH; ++i) {
									board[xs][i] = board[xs][i + 1];
									if (board[xs][i] == ' ') {
										deleteBox(xs, i);
										drawBoxBackground(xs, i);
									}

									else drawBox(xs, i, board);
								}
								board[xs][BOARD_WIDTH] = ' ';
								deleteBox(xs, BOARD_WIDTH);
								drawBoxBackground(xs, BOARD_WIDTH);
							}
							fc = { 0, 0 };
						}
					}
					sc = { 0, 0 };
				}
			}
			break;
		default:
			break;
		}
		highlightBox(px, py, board);
	}
	return true;
}

void startGame(Node* plPlay, Node* plHead, int option, string options[], int color[]) {
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	px = py = 1;
	board = setupBoard();
	system("cls");
//	PlaySound(TEXT("board.wav"), NULL, SND_FILENAME | SND_ASYNC);
	drawBackground();
	printBoard(board);
	highlightBox(px, py, board);
	movetoPos(134, 41);
	SetConsoleTextAttribute(console, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
	printInstruction();
	gameLoop = true;
	score = hint = 0;
	string s;
	int len;
	const int duration = 120000;
	DWORD startTime = GetTickCount(); // retrieves the starting time
	printStrPos(140, 9, "Player");
	SetConsoleTextAttribute(console, color[option]);
	cout << " " << plPlay->info.name << "               ";
	printStrPos(140, 10, options[option]);
	cout << "            ";
	SetConsoleTextAttribute(console, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
	printStrPos(140, 11, "Time remaining:        ");
	printStrPos(140, 12, "Hint: 0/3              ");
	printStrPos(140, 13, "Score:                 ");
	while (gameLoop) {
		DWORD currentTime = GetTickCount(); // retrieves the current time
		DWORD elapsedTime = currentTime - startTime; // time used
		int remainingTime = duration - elapsedTime; // time remaining (milisecond)
		int remainingSeconds = remainingTime / 1000;
		int minutes = remainingSeconds / 60;
		int seconds = remainingSeconds % 60;
		movetoPos(156, 11);
		cout << minutes << ":" << (seconds < 10 ? "0" : "") << seconds << "             ";
		movetoPos(150, 13);
		cout << score << "            ";
		if (remainingTime <= 0) { // out of time
			SetConsoleTextAttribute(console, 4);
			printStrPos(140, 14, "Time's up!");
			clearBoard(board, px, py, fc);
			deleteBoard();
			Sleep(2000);
			system("cls");
			// "you won" screen
			drawYouLost();
//			PlaySound(TEXT("lose.wav"), NULL, SND_FILENAME | SND_ASYNC);
			SetConsoleTextAttribute(console, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
			s = "Try again.";
			len = s.size();
			printStrPos((yMax - len) / 2, 20, s);
			Sleep(2000);
			gameLoop = false;
			break;
		}
		if (checkWin(option)) { // check if the board is cleared
			clearBoard(board, px, py, fc);
			deleteBoard();
			Sleep(2000);
			system("cls");
			// "you won" screen
			drawYouWon();
//			PlaySound(TEXT("win.wav"), NULL, SND_FILENAME | SND_ASYNC);
			SetConsoleTextAttribute(console, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
			movetoPos((yMax - 40) / 2, 20);
			cout << "Score: " << score << " point(s)";
			movetoPos((yMax - 40) / 2, 21);
			cout << "Time remaining: " << remainingSeconds << " secs (+" << remainingSeconds << " point(s))";
			movetoPos((yMax - 40) / 2, 22);
			cout << "Total score: " << score + remainingSeconds << " point(s)";
			if (plPlay->info.stage == option - 1) { // if in the highest unlocked stage
				SetConsoleTextAttribute(console, color[option + 1]);
				if (option == 1)
					s = "You have unlocked Hard mode";
				len = s.size();
				printStrPos((yMax - len) / 2, 24, s);
				plPlay->info.stage = min(plPlay->info.stage + 1, 2); // save record
			}
			plPlay->info.record += (score + remainingSeconds);
			savePlayersList(plHead);
			Sleep(2000);
			gameLoop = false;
			break;
		}
		if (!Input(option)) { // if quit
			clearBoard(board, px, py, fc);
			deleteBoard();
			gameLoop = false;
			SetConsoleTextAttribute(console, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
			return;
		}
	}
	string back[1] = { "Back" };
	int clr[1] = { 14 };
	int opt = makeOptions(back, clr, 1, 1, xMax - 5, 10, 0);
	SetConsoleTextAttribute(console, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);

}

void setupGame(Node*& plHead) {
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	system("cls");
	drawBackground();
	drawLogo();
	string s = "Enter your name (must have no blank spaces)";
	int len = s.size();
	printStrPos((yMax - len) / 2, 22, s);
	SetConsoleTextAttribute(console, BACKGROUND_BLUE);
	movetoPos((yMax - 40) / 2 - 2, 24);
	for (int i = -2; i < 41; i++) {
		cout << " ";
	}
	movetoPos((yMax - 40) / 2 - 2, 28);
	for (int i = -2; i < 41; i++) {
		cout << " ";
	}
	for (int i = 0; i < 3; i++) {
		movetoPos((yMax - 40) / 2 - 2, 25 + i);
		cout << " ";
		for (int j = (yMax - 40) / 2 - 1; j < (yMax + 45) / 2 - 2; ++j) {
			SetConsoleTextAttribute(console, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
			cout << ' ';
		}
		SetConsoleTextAttribute(console, BACKGROUND_BLUE);
		cout << " ";
	}
	SetConsoleTextAttribute(console, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
	movetoPos((yMax - 40) / 2, 26);
	showCursor(true);
	enableEcho();
	string plName;
	cin >> plName;
	disableEcho();
	showCursor(false);
//	PlaySound(TEXT("hello.wav"), NULL, SND_FILENAME | SND_ASYNC);
	printStrPos((yMax - 40) / 2, 26, "Hello ");
	SetConsoleTextAttribute(console, 11);
	cout << plName;
	Sleep(1000);
	Node* plPlay = findPlayer(plName, plHead);
	if (plPlay == NULL) {
		addheadPlayer(plName, plHead);
		savePlayersList(plHead);
		plPlay = plHead;
	}
	system("cls");
	// mode option box
	string options[3] = { "Back Home", "Easy mode", "Hard mode" };
	int color[3] = { 3, 11, 12 };
	int option = 1;
	while (true) {
		system("cls");
		option = makeOptions(options, color, 3, min(plPlay->info.stage + 2, 3), 20, 20, option);
		if (option == 0)
			break;
		startGame(plPlay, plHead, option, options, color);
	}
}
