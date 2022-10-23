#define _CRT_SECURE_NO_WARNINGS

#include "xl.h"
#include <stdio.h>
#include <windows.h>
#include <conio.h>
#include <iostream>
using namespace std;
#define maxmenu 4

#define colorcode_black			0
#define colorcode_darkblue		1
#define colorcode_darkgreen		2
#define colorcode_darkcyan		3
#define colorcode_darkred		4
#define colorcode_darkpink		5
#define colorcode_darkyellow	6
#define colorcode_darkwhite		7
#define colorcode_grey			8
#define colorcode_blue			9
#define colorcode_green			10
#define colorcode_cyan			11
#define colorcode_red			12
#define colorcode_pink			13
#define colorcode_yellow		14
#define colorcode_white			15



void about(){
	char s[1000];
	strcpy(s, "Ten: Vu Hong Phuc\n                       Truong DH KHTN (HCMUS)\n                       MSSV: 18120515\n                       LOP: 18CNTN\n                       Email: wasd742000@gmail.com");
	system("cls");
	printf("\n\n\n");
	TextColor(10);
	printf("                        ABOUT\n\n\n");
	TextColor(15);
	printf("                       %s\n\n", s);
	TextColor(11);
	printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n     ESC: back                                                                                    ");
	while (1){
		if (_kbhit()){
			int key = _getch();
			if (key == 27){
				break;
			}
		}
	}
}

// in phan huong dan
void help(){
	system("cls");
	printf("\n\n\n\n");
	TextColor(13);
	printf("                     HELP\n\n\n");
	TextColor(15);
	printf("             - IN menu: Using arrows key to scroll, Enter to select and ESC to go back.\n\n");
	printf("             - In game: \n                  + Using w, a, s, d to control snake.\n                  + enter to check.\n                  + Press L to save game, T to load game.\n                  + Press ESC to back to menu.");
	TextColor(11);
	printf("\n\n\n\n\n\n\n\n\n\n\n     ESC: back                                                                                    ");
	while (1){
		if (_kbhit()){
			int k = _getch();
			if (k == 27){
				break;
			}
		}
	}
}

// ham menu xu ly chinh
int MainMenu(){
	FixConsoleWindow();
	mystring menu[] = { "START NEW GAME", "LOAD SAVED GAME", "HELP", "ABOUT", "EXIT" };
	int pointer = 0;
	while (1){
		system("cls");
		TextColor(10);
		printf("\n\n");
		printf("\n\n");
		printf("			..######....##....##.......###........##....##..########\n");
		printf("			.##....##...###...##......##.##.......##...##...##......\n");
		printf("			.##.........####..##.....##...##......##..##....##......\n");
		printf("			..######....##.##.##....##.... ##.....#####.....######..\n");
		printf("			.......##...##..####...###########....##..##....##......\n");
		printf("			.##....##...##...###..##.........##...##...##...##......\n");
		printf("			..######....##....##.##...........##..##....##..########\n");

		printf("\n\n");
		TextColor(12);
		printf("                                                RAN SAN MOI       \n");
		TextColor(10);
		printf("\n               NOTICE: Tat Unikey khi choi.");
		printf("\n\n\n\n");
		for (int i = 0; i <= maxmenu; i++){
			if (pointer == i){
				TextColor(14);
				printf("                         -> %s <-\n", menu[i].str);
			}
			else{
				TextColor(15);
				printf("                         %s\n", menu[i].str);
			}
		}
		TextColor(11);
		printf("\n\n\n\n                                                                                                  Enter: select");
		while (1){
			if (_kbhit()){
				char key = _getch();
				//mui ten di len
				if (key == 72){
					if (pointer == 0){
						pointer = maxmenu;
					}
					else{
						pointer--;
					}
					break;
				}
				//mui ten di xuong
				if (key == 80){
					if (pointer == maxmenu){
						pointer = 0;
					}
					else{
						pointer++;
					}
					break;
				}
				//phim enter
				if (key == 13){
					switch (pointer){
					case 0:
						GameRun(0);
						break;
					case 1:
						LoadScr();
						GameRun(1);
						break;
					case 2:
						help();
						break;
					case 3:
						about();
						break;
					case 4:
						return 0;
						break;
					}
					break;
				}
			}
		}
	}
	return 0;
}

void main(){
	MainMenu();
}