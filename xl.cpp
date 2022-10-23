#define _CRT_SECURE_NO_WARNINGS
#include <thread>
#include <iostream>
using namespace std;
#include <windows.h>
#include <time.h>
#include <conio.h>
#include "xl.h"
//#include <cwchar>



void GotoXY(int x, int y);
bool IsValid(int x, int y);
void GenerateFood();
void ResetData();
void DrawBoard(int x, int y, int width, int height, int flag, int curPosX, int curPosY);
void StartGame();
void ExitGame(HANDLE t);
void PauseGame(HANDLE t);
void ScoreBoard();
void Eat();
void DrawSnakeAndFood(char *str);
void MoveRight();
void MoveLeft();
void MoveUp();
void MoveDown();
// BASIC GAME FUNCTION


void FixConsoleWindow(){
	HWND consoleWindow = GetConsoleWindow();
	LONG style = GetWindowLong(consoleWindow, GWL_STYLE);
	style = style &~(WS_MAXIMIZEBOX)&~(WS_THICKFRAME);
	SetWindowLong(consoleWindow, GWL_STYLE, style);
}

void TextColor(int color)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

int random(int min, int max){
	return min + rand() % (max + 1 - min);
}

void GotoXY(int x, int y){
	COORD coord;
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}


// GAME VARIBLE
#define MAX_SIZE_SNAKE 10
#define MAX_SIZE_FOOD 4
#define MAX_SPEED 11

int mssv[] = { 1, 8, 1, 2, 0, 5, 1, 5, 1, 8, 1, 2, 0, 5, 1, 5 };
POINT snake[10];
POINT food[4];
int GateX, GateY;
int CHAR_LOCK;
int MOVING;
int SPEED;
int HEIGH_CONSOLE, WIDTH_CONSOLE;
int FOOD_INDEX;
int SIZE_SNAKE;
int STATE;
bool CHECK_ENTER_GATE; // kiem tra ran da vao cong chua 
int SCORE = 0, LEVEL = 0;


// Xu ly cong vao
void GenerateGate(){
	int x, y;
	srand(time(NULL));
	do{
		x = random(3, WIDTH_CONSOLE - 3);
		y = random(3, HEIGH_CONSOLE - 3);
	} while (!IsValid(x, y) || !IsValid(x - 1, y) || !IsValid(x + 1, y) || !IsValid(x - 1, y + 1) || !IsValid(x + 1, y + 1));
	GateX = x;
	GateY = y;
}

void DrawGate(int color){
	TextColor(color);
	GotoXY(GateX, GateY);
	cout << " ";
	GotoXY(GateX - 1, GateY);
	cout << " ";
	GotoXY(GateX + 1, GateY);
	cout << " ";
	GotoXY(GateX - 1, GateY + 1);
	cout << " ";
	GotoXY(GateX + 1, GateY + 1);
	cout << " ";
	TextColor(15);
}

// RESET VARIBLE

bool IsValid(int x, int y){
	for (int i = 0; i < SIZE_SNAKE; i++)
	if (snake[i].x == x && snake[i].y == y) return false;
	return true;
}

void GenerateFood(){
	int x, y;
	srand(time(NULL));
	for (int i = 0; i < MAX_SIZE_FOOD; i++){
		do{
			x = random(2, WIDTH_CONSOLE - 2);
			y = random(2, HEIGH_CONSOLE - 2);
		} while (IsValid(x, y) == false);
		food[i] = { x, y };
	}
}

void ResetData(){
	CHAR_LOCK = 'A', MOVING = 'D', SPEED = 6;
	FOOD_INDEX = 0, WIDTH_CONSOLE = 70, HEIGH_CONSOLE = 20, SIZE_SNAKE = 6;
	CHECK_ENTER_GATE = 0;
	for (int i = 0; i < 6; i++){
		snake[i] = { i + 10, 5 };
	}
	GenerateFood();
}

// Ve o cho nhieu muc dich
void DrawBoard(int x, int y, int width, int height, int flag, int curPosX = 0, int curPosY = 0){
	if (!flag){
		TextColor(55);
		GotoXY(x, y);
		cout << " ";
		for (int i = 1; i < width; i++) cout << " ";
		cout << " ";
		GotoXY(x, height + y);
		cout << " ";
		for (int i = 1; i < width; i++)cout << " ";
		cout << " ";
		for (int i = y + 1; i < height + y; i++){
			GotoXY(x, i);
			cout << " ";
			GotoXY(x + width, i);
			cout << " ";
		}
	}
	else{
		TextColor(75);
		GotoXY(x, y);
		cout << ":";
		for (int i = 1; i < width; i++) cout << ":";
		cout << ":";
		GotoXY(x, height + y);
		cout << ":";
		for (int i = 1; i < width; i++)cout << ":";
		cout << ":";
		for (int i = y + 1; i < height + y; i++){
			GotoXY(x, i);
			cout << ":";
			GotoXY(x + width, i);
			cout << ":";
		}
	}
	GotoXY(curPosX, curPosY);
	TextColor(15);
}
// xu ly game
void StartGame(){
	system("cls");
	ResetData();
	DrawBoard(0, 0, WIDTH_CONSOLE, HEIGH_CONSOLE, 0);
	ScoreBoard();
	STATE = 1;
}

void ExitGame(HANDLE t){
	system("cls");
	TerminateThread(t, 0);
}

void PauseGame(HANDLE t){
	SuspendThread(t);
}

// Hieu ung 
void SnakeText(int color){
	TextColor(color);
	GotoXY(15, 21); cout << "  _________              __        " << endl;
	GotoXY(15, 22); cout << " /   _____/ ____ _____  |  | __ ____  " << endl;
	GotoXY(15, 23); cout << " \\_____  \\ /    \\\\__  \\ |  |/ // __ \ " << endl;
	GotoXY(15, 24); cout << " /        \\   |  \\/ __ \\|    <\\  ___/ " << endl;
	GotoXY(15, 25); cout << "/_______  /___|  (____  /__|_ \\\\___  >" << endl;
	GotoXY(15, 26); cout << "        \\/     \\/     \\/     \\/    \\/ " << endl;
}

void LevelUp(){
	SnakeText(6);
	TextColor(6);
	GotoXY(10, 7); cout << "#   ### # # ### #       # # ##    # ";
	GotoXY(10, 8); cout << "#   #   # # #   #       # # # #   # ";
	GotoXY(10, 9); cout << "#   ##  # # ##  #       # # ##    # ";
	GotoXY(10, 10); cout << "#   #   # # #   #       # # #       ";
	GotoXY(10, 11); cout << "### ###  #  ### ###     ### #     O ";
	Sleep(1000);
}

// Ve o diem so
void ScoreBoard(){
	SnakeText(9);
	GotoXY(1, 28);
	TextColor(11);
	cout << "ESC: Exit to menu	P: Pause	T: Load		L: Save";
	DrawBoard(WIDTH_CONSOLE + 5, 1, 40, 9, 1);
	GotoXY(WIDTH_CONSOLE + 13, 4);
	TextColor(14);
	cout << "SCORE";
	GotoXY(WIDTH_CONSOLE + 15, 7);
	cout << SCORE;
	GotoXY(WIDTH_CONSOLE + 33, 4);
	cout << "LEVEL";
	GotoXY(WIDTH_CONSOLE + 35, 7);
	cout << LEVEL + 1;
	DrawBoard(WIDTH_CONSOLE + 25, 1, 1, 9, 1);
	DrawBoard(WIDTH_CONSOLE + 5, 10, 40, 17, 1);
	DrawBoard(WIDTH_CONSOLE + 14, 17, 2, 2, 0);
	GotoXY(WIDTH_CONSOLE + 15, 18);
	cout << "W";
	DrawBoard(WIDTH_CONSOLE + 11, 20, 2, 2, 0);
	GotoXY(WIDTH_CONSOLE + 12, 21);
	cout << "A";
	DrawBoard(WIDTH_CONSOLE + 14, 20, 2, 2, 0);
	GotoXY(WIDTH_CONSOLE + 15, 21);
	cout << "S";
	DrawBoard(WIDTH_CONSOLE + 17, 20, 2, 2, 0);
	GotoXY(WIDTH_CONSOLE + 18, 21);
	cout << "D";
	DrawBoard(WIDTH_CONSOLE + 25, HEIGH_CONSOLE / 2 + 1, 1, 15, 1);
	GotoXY(WIDTH_CONSOLE + 9, 13);
	cout << "SNAKE CONTROL";
	GotoXY(WIDTH_CONSOLE + 29, 13);
	cout << "ENTER THE GATE";
	GotoXY(WIDTH_CONSOLE + 30, 14);
	cout << "TO LEVEL UP";
	TextColor(35);
	GotoXY(WIDTH_CONSOLE + 35, 17);
	cout << " ";
	GotoXY(WIDTH_CONSOLE + 34, 17);
	cout << " ";
	GotoXY(WIDTH_CONSOLE + 36, 17);
	cout << " ";
	GotoXY(WIDTH_CONSOLE + 34, 18);
	cout << " ";
	GotoXY(WIDTH_CONSOLE + 36, 18);
	cout << " ";
	GotoXY(WIDTH_CONSOLE + 35, 20);
	TextColor(11);
	cout << "O";
	GotoXY(WIDTH_CONSOLE + 35, 21);
	TextColor(15);
	cout << "O";
	GotoXY(WIDTH_CONSOLE + 35, 22);
	TextColor(15);
	cout << "O";
	GotoXY(WIDTH_CONSOLE + 35, 23);
	TextColor(15);
	cout << "O";
}

// Xu ly khi an
void Eat(){
	SCORE++;
	ScoreBoard();
	snake[SIZE_SNAKE] = food[FOOD_INDEX];
	if (FOOD_INDEX == MAX_SIZE_FOOD - 1){
		GenerateGate();
		STATE = 2;
		DrawGate(35);
		if (SPEED == MAX_SPEED) {
			SIZE_SNAKE = 6;
			SPEED = 5;
		}
		else SPEED++;
		GenerateFood(); 
	}
	else{
		FOOD_INDEX++;
		SIZE_SNAKE++;
	}
}

// Xu ly khi chet
int ProcessDead(){
	DrawSnakeAndFood("G");
	Sleep(600);
	STATE = 0;
	SnakeText(12);
	DrawBoard(25, 7, 40, 10, 1);
	GotoXY(42, 9);
	TextColor(12);
	cout << "YOU DEAD";
	mystring menu[] = { "Replay", "Exit" };
	int pointer = 0;
	while (1){
		for (int i = 0; i < 2; i++){
			if (pointer == i){
				TextColor(12);
				if(pointer == 0) GotoXY(35, 13);
				else GotoXY(52, 13);
				cout << menu[i].str;
			}
			else{
				TextColor(15);
				if (pointer == 1) GotoXY(35, 13);
				else GotoXY(52, 13);
				cout << menu[i].str;
			}
		}
		while (1){
			if (_kbhit()){
				char key = _getch();
				if (key == 75){
					pointer = 0;
					break;
				}
				if (key == 77){
					pointer = 1;
					break;
				}
				//phim enter
				if (key == 13){
					switch (pointer){
					case 0:
						return 1;
					case 1:
						return 0;
					}
					break;
				}
			}
		}
	}
}


void DrawSnakeAndFood(char *str){
	if (STATE == 1){
		GotoXY(food[FOOD_INDEX].x, food[FOOD_INDEX].y);
		printf(str);
	}
	int temp = 0;
	for (int i = 0; i < SIZE_SNAKE; i++){
		GotoXY(snake[i].x, snake[i].y);
		if (*str == ' ') printf(str);
		else {
			if (temp == SIZE_SNAKE - 1 && CHECK_ENTER_GATE == 0) TextColor(11);
			if (str == "G") TextColor(12);
			cout << mssv[temp++];
			TextColor(15);
		}
	}
}

// Cac ham xu ly di chuyen
void MoveRight(){
	int headx = snake[SIZE_SNAKE - 1].x;
	int heady = snake[SIZE_SNAKE - 1].y;
	int x = GateX, y = GateY;
	if (STATE == 2){
		if ((headx == x - 2 && (heady == y || heady == y + 1)) || (headx == x && heady == y + 1))
			ProcessDead();
	}
	for (int i = 0; i < SIZE_SNAKE - 1; i++){
		if (headx + 1 == snake[i].x  && heady == snake[i].y){
			ProcessDead();
		}
	}
	if (headx + 1 == WIDTH_CONSOLE){
		ProcessDead();
	}
	else{
		if (headx + 1 == food[FOOD_INDEX].x && heady == food[FOOD_INDEX].y){
			Eat();
		}
		for (int i = 0; i < SIZE_SNAKE - 1; i++){
			snake[i].x = snake[i + 1].x;
			snake[i].y = snake[i + 1].y;
		}
		snake[SIZE_SNAKE - 1].x++;
	}
}

void MoveLeft(){
	int headx = snake[SIZE_SNAKE - 1].x;
	int heady = snake[SIZE_SNAKE - 1].y;
	int x = GateX, y = GateY;
	if (STATE == 2){
		if ((headx == x + 3 && (heady == y || heady == y + 1)) || (headx == x && heady == y + 1))
			ProcessDead();
	}
	for (int i = 0; i < SIZE_SNAKE - 1; i++){
		if (headx - 1 == snake[i].x  && heady == snake[i].y){
			ProcessDead();
		}
	}
	if (headx - 1 == 0){
		ProcessDead();
	}
	else{
		if (headx - 1 == food[FOOD_INDEX].x && heady == food[FOOD_INDEX].y){
			Eat();
		}
		for (int i = 0; i < SIZE_SNAKE - 1; i++){
			snake[i].x = snake[i + 1].x;
			snake[i].y = snake[i + 1].y;
		}
		snake[SIZE_SNAKE - 1].x--;
	}
}

void MoveDown(){
	int headx = snake[SIZE_SNAKE - 1].x;
	int heady = snake[SIZE_SNAKE - 1].y;
	int x = GateX, y = GateY;
	if (STATE == 2){
		if (heady == y - 1 && (headx == x || headx == x + 1 || headx == x - 1))
			ProcessDead();
	}
	for (int i = 0; i < SIZE_SNAKE - 1; i++){
		if (headx == snake[i].x  && heady + 1 == snake[i].y){
			ProcessDead();
		}
	}
	if (heady + 1 == HEIGH_CONSOLE){
		ProcessDead();
	}
	else{
		if (headx == food[FOOD_INDEX].x && heady + 1 == food[FOOD_INDEX].y){
			Eat();
		}
		for (int i = 0; i < SIZE_SNAKE - 1; i++){
			snake[i].x = snake[i + 1].x;
			snake[i].y = snake[i + 1].y;
		}
		snake[SIZE_SNAKE - 1].y++;
	}
}

void MoveUp(){
	int headx = snake[SIZE_SNAKE - 1].x;
	int heady = snake[SIZE_SNAKE - 1].y;
	int x = GateX, y = GateY;
	if (STATE == 2){
		if (heady == y + 2 && (headx == x - 1 || headx == x + 1))
			ProcessDead();
		else if (headx == x && heady == y + 2) CHECK_ENTER_GATE = 1;
	}
	for (int i = 0; i < SIZE_SNAKE - 1; i++){
		if (headx == snake[i].x  && heady - 1 == snake[i].y){
			ProcessDead();
		}
	}
	if (heady - 1 == 0){
		ProcessDead();
	}
	else{
		if (headx == food[FOOD_INDEX].x && heady - 1 == food[FOOD_INDEX].y){
			Eat();
		}
		for (int i = 0; i < SIZE_SNAKE - 1; i++){
			snake[i].x = snake[i + 1].x;
			snake[i].y = snake[i + 1].y;
		}
		snake[SIZE_SNAKE - 1].y--;
	}
}


// Ham da luong
void ThreadFunc(){
	while (1){
		if (STATE){
			if (CHECK_ENTER_GATE) {
				DrawGate(66);
				SIZE_SNAKE--;
			}
			if (SIZE_SNAKE == 0 && CHECK_ENTER_GATE) {
				LevelUp();
				LEVEL++;
				StartGame();
			}
			DrawSnakeAndFood(" ");
			switch (MOVING){
			case 'A':
				MoveLeft();
				break;
			case 'D':
				MoveRight();
				break;
			case 'W':
				MoveUp();
				break;
			case 'S':
				MoveDown();
				break;
			}
			if (STATE) DrawSnakeAndFood("O");
			Sleep(1000 / SPEED);
		}
	}
}


// Cac ham xu ly save va load game
bool SaveGame(char name[]){
	FILE *fSave = fopen(name, "wb");
	if (fSave == NULL) return 0;
	fwrite(&GateX, sizeof(int), 1, fSave);
	fwrite(&GateY, sizeof(int), 1, fSave);
	fwrite(&CHAR_LOCK, sizeof(int), 1, fSave);
	fwrite(&MOVING, sizeof(int), 1, fSave);
	fwrite(&SPEED, sizeof(int), 1, fSave);
	fwrite(&HEIGH_CONSOLE, sizeof(int), 1, fSave);
	fwrite(&WIDTH_CONSOLE, sizeof(int), 1, fSave);
	fwrite(&FOOD_INDEX, sizeof(int), 1, fSave);
	fwrite(&SIZE_SNAKE, sizeof(int), 1, fSave);
	fwrite(&STATE, sizeof(int), 1, fSave);
	fwrite(&SCORE, sizeof(int), 1, fSave);
	fwrite(&LEVEL, sizeof(int), 1, fSave);
	fwrite(&CHECK_ENTER_GATE, sizeof(bool), 1, fSave);
	for (int i = 0; i < SIZE_SNAKE; i++)
		fwrite(&snake[i], sizeof(POINT), 1, fSave);
	for (int i = 0; i < MAX_SIZE_FOOD; i++)
		fwrite(&food[i], sizeof(POINT), 1, fSave);
	fclose(fSave);
	return 1;
}

bool LoadGame(char name[]){
	FILE *fLoad = fopen(name, "rb");
	if (fLoad == NULL) return 0;
	fread(&GateX, sizeof(int), 1, fLoad);
	fread(&GateY, sizeof(int), 1, fLoad);
	fread(&CHAR_LOCK, sizeof(int), 1, fLoad);
	fread(&MOVING, sizeof(int), 1, fLoad);
	fread(&SPEED, sizeof(int), 1, fLoad);
	fread(&HEIGH_CONSOLE, sizeof(int), 1, fLoad);
	fread(&WIDTH_CONSOLE, sizeof(int), 1, fLoad);
	fread(&FOOD_INDEX, sizeof(int), 1, fLoad);
	fread(&SIZE_SNAKE, sizeof(int), 1, fLoad);
	fread(&STATE, sizeof(int), 1, fLoad);
	fread(&SCORE, sizeof(int), 1, fLoad);
	fread(&LEVEL, sizeof(int), 1, fLoad);
	fread(&CHECK_ENTER_GATE, sizeof(bool), 1, fLoad);
	for (int i = 0; i < SIZE_SNAKE; i++)
		fread(&snake[i], sizeof(POINT), 1, fLoad);
	for (int i = 0; i < MAX_SIZE_FOOD; i++)
		fread(&food[i], sizeof(POINT), 1, fLoad);
	fclose(fLoad);
	return 1;
}

void IncreaseFileNumber(){
	int n;
	FILE *out = fopen("number.txt", "r");
	fscanf(out, "%d", &n);
	fclose(out);
	n += 1;
	FILE *in1 = fopen("number.txt", "w");
	fprintf(in1, "%d\n", n);
	fclose(in1);

}



void SaveScr(){
	system("cls");
	cout << endl << endl << endl;
	TextColor(9);
	printf("\t\t\t\t..######....##....##.......###........##....##..########\n");
	printf("\t\t\t\t.##....##...###...##......##.##.......##...##...##......\n");
	printf("\t\t\t\t.##.........####..##.....##...##......##..##....##......\n");
	printf("\t\t\t\t..######....##.##.##....##.... ##.....#####.....######..\n");
	printf("\t\t\t\t.......##...##..####...###########....##..##....##......\n");
	printf("\t\t\t\t.##....##...##...###..##.........##...##...##...##......\n");
	printf("\t\t\t\t..######....##....##.##...........##..##....##..########\n");
	GotoXY(53, 12);
	cout << "SAVE GAME";
	DrawBoard(35, 16, 50, 5, 1);
	TextColor(15);
	GotoXY(40, 19);
	cout << "File name: ";
	GotoXY(13, 28);
	TextColor(11);
	cout << "ESC: back";
	while (1){
		if (_kbhit()){
			int k = _getch();
			if (k == 27){
				break;
			}
			else{
				IncreaseFileNumber();
				char filename[30];
				GotoXY(50, 19);
				//fflush(stdin);
				scanf("%s", &filename);
				FILE *in2 = fopen("list.txt", "a");
				fprintf(in2, "\n%s", filename);
				fclose(in2);
				SaveGame(filename);
			}
		}
	}
}

void LoadEffect(){
	system("cls");
	TextColor(8);
	printf("\n\n\n");
	printf("\t\t\t\t..######....##....##.......###........##....##..########\n");
	printf("\t\t\t\t.##....##...###...##......##.##.......##...##...##......\n");
	printf("\t\t\t\t.##.........####..##.....##...##......##..##....##......\n");
	printf("\t\t\t\t..######....##.##.##....##.... ##.....#####.....######..\n");
	printf("\t\t\t\t.......##...##..####...###########....##..##....##......\n");
	printf("\t\t\t\t.##....##...##...###..##.........##...##...##...##......\n");
	printf("\t\t\t\t..######....##....##.##...........##..##....##..########\n");
	printf("\n\n\n");
	TextColor(10);
	GotoXY(50, 13);
	cout << ".....LOADING....";
	DrawBoard(3, 15, 110, 2, 35);
	for (int i = 0; i < 4; i++){
		TextColor(30);
		GotoXY(4 + 27 * i, 16);
		printf("                            ");
		GotoXY(55, 19);
		TextColor(15);
		cout << (i * 100) / 3 << " % ";
		Sleep(400);
		GotoXY(55, 19);
		cout << "           ";
	}
	GotoXY(50, 13);
	TextColor(14);
	cout << "LOADING COMPLETED";
	Sleep(400);
}

void LoadScr(){
	int nlist, p = 1; // pointer
	mystring list[30];
	FILE *in1 = fopen("number.txt", "r");
	fscanf(in1, "%d", &nlist);
	fclose(in1);
	FILE *in2 = fopen("list.txt", "r");
	for (int i = 1; i <= nlist; i++){
		fscanf(in2, "%s", list[i].str);
	}
	fclose(in2);
	LoadEffect();
	system("cls");
	cout << endl << endl << endl;
	TextColor(9);
	printf("\t\t\t\t..######....##....##.......###........##....##..########\n");
	printf("\t\t\t\t.##....##...###...##......##.##.......##...##...##......\n");
	printf("\t\t\t\t.##.........####..##.....##...##......##..##....##......\n");
	printf("\t\t\t\t..######....##.##.##....##.... ##.....#####.....######..\n");
	printf("\t\t\t\t.......##...##..####...###########....##..##....##......\n");
	printf("\t\t\t\t.##....##...##...###..##.........##...##...##...##......\n");
	printf("\t\t\t\t..######....##....##.##...........##..##....##..########\n");
	printf("\n\n\n");
	TextColor(11);
	GotoXY(13, 28);
	cout << "ESC: back";
	GotoXY(65, 28);
	cout << "Enter: select";
	if (nlist != 0){
		while (1){
			GotoXY(53, 12);
			TextColor(15);
			printf("LOAD GAME       \n\n\n\n");
			for (int i = 1; i <= nlist; i++){
				if (p == i){
					TextColor(14);
					printf("\n                      >> GAME SAVE %d: %s", i, list[i].str);
				}
				else{
					TextColor(15);
					printf("\n                      Game save %d:  %s  ", i, list[i].str);
				}
			}
			while (1){
				if (_kbhit()){
					int key = _getch();
					// di len
					if (key == 72){
						if (p == 1){
							p = nlist;
						}
						else{
							p--;
						}
						break;
					}
					// di xuong
					if (key == 80){
						if (p == nlist){
							p = 1;
						}
						else{
							p++;
						}
						break;
					}
					if (key == 27){
						return;
					}
					// Phim enter
					if (key == 13){
						LoadGame(list[p].str);
						return;
					}
				}
			}
		}

	}
	else{
		GotoXY(15, 15);
		printf("       !!!! CHUA CO GAME DUOC LUU !!!!");
		while (1){
			if (_kbhit()){
				int key = _getch();
				if (key == 27){
					return;
				}
			}
		}
	}
}

void Rebuild(){
	system("cls");
	DrawBoard(0, 0, WIDTH_CONSOLE, HEIGH_CONSOLE, 0);
	ScoreBoard();
	STATE = 1;
	if (STATE == 2) DrawGate(35);
}

void GameRun(int loadflag){
	int temp;
	FixConsoleWindow();
	if (!loadflag) StartGame();
	else Rebuild();
	thread t1(ThreadFunc);
	HANDLE handle_t1 = t1.native_handle();
	while (1){
		temp = toupper(_getch());
		if (STATE) { 
			if (temp == 'L'){
				PauseGame(handle_t1);
				SaveScr();
				Rebuild();
				ResumeThread(handle_t1);
			}
			if (temp == 'T'){
				PauseGame(handle_t1);
				LoadScr();
				Rebuild();
				ResumeThread(handle_t1);
			}
			if (temp == 'P'){
				PauseGame(handle_t1);
			}
			else if (temp == 27) {
				STATE = 0;
				ExitGame(handle_t1);
				t1.join();
				return;
			}
			else{
				ResumeThread(handle_t1);
				if ((temp != CHAR_LOCK) && (temp == 'D' || temp == 'A' || temp == 'W' || temp == 'S')){
					if (temp == 'D') CHAR_LOCK = 'A';
					else if (temp == 'W') CHAR_LOCK = 'S';
					else if (temp == 'S') CHAR_LOCK = 'W';
					else CHAR_LOCK = 'D';
					MOVING = temp;
				}
			}
		}
		else{
			int flag = ProcessDead();
			if (flag == 1)
				StartGame();
			else{
				ExitGame(handle_t1);
				t1.join();
				return;
			}
		}
	}
}


