#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <Windows.h>

#define MAX_X 7
#define MAX_Y 7
#define MAX_OBSTACLE 7

enum{way, background, obstacle, me};
enum{right, down, left, up};

int room[MAX_Y][MAX_X];
int move_count[4] = { 0 };
int me_x = 0, me_y = 0;

void menu();
void reset_board();
void create_obstacle();
void print_board();
void search_way();
void move_me(int command);

_Bool available_move_me(int x, int y);
_Bool in_range(int x, int y);
_Bool available_move(int x, int y);

int main() {
	srand(time(NULL));

	menu();
	return 0;
}

void menu() {
	reset_board();
	create_obstacle();

	search_way();
	print_board();

	while (1) {

		char command;

		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
		printf("r : 새로운 경로 만들기\nt : 객체 나타내기\nwasd : 객체 이동\nq : 프로그램 종료\n명령어 : ");
		command = getchar();
		fseek(stdin, 0, SEEK_END);

		switch (command) {
		case 'r':
			search_way();
			print_board();
			break;
		case 't':
			room[me_y][me_x] = way;
			me_x = 0; 
			me_y = 0;
			room[me_y][me_x] = me;
			break;
		case 'w':
			move_me(up);
			break;
		case 'a':
			move_me(left);
			break;
		case 's':
			move_me(down);
			break;
		case 'd':
			move_me(right);
			break;
		case 'q': return 0;
		default: break;
		}
	}
}

void reset_board(){
	for (int y = 0; y < MAX_Y; y++) {
		for (int x = 0; x < MAX_X; x++) {
			if(room[y][x] == way)	room[y][x] = background;
		}
	}
	room[0][0] = way;
}

void create_obstacle() {
	int obstacle_count = 0;
	int x, y;

	while (obstacle_count < MAX_OBSTACLE) {
		x = rand() % MAX_X;
		y = rand() % MAX_Y;

		if (room[y][x] == background && (x != MAX_X - 1 && y != MAX_Y - 1)) {
			room[y][x] = obstacle;
			obstacle_count++;
		}
	}
}

void print_board() {
	system("cls");
	for (int y = 0; y < MAX_Y; y++) {
		for (int x = 0; x < MAX_X; x++) {
			switch (room[y][x]) {
			case way: 
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
				printf("%3c", '0'); 
				break;
			case background: 
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
				printf("%3c", '1');
				break;
			case obstacle: 
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 3);
				printf("%3c", 'X'); 
				break;
			case me:
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 5);
				printf("%3c", '*');
				break;
			}
		}
		printf("\n");
	}
}

void search_way() {
	int x, y;
	int random_move;
	x = 0;
	y = 0;

	while (x != MAX_X - 1 || y != MAX_Y - 1) {
		random_move = rand() % 4;

		if (!(in_range(x + 1, y) && available_move(x + 1, y))) {
			if (!(in_range(x, y + 1) && available_move(x, y + 1))) {
				if (!(in_range(x - 1, y) && available_move(x - 1, y))) {
					if (!(in_range(x, y - 1) && available_move(x, y - 1))) {
						reset_board();
						x = 0;
						y = 0;
					}
				}
			}
		}

		switch (random_move) {
		case right:
			if (in_range(x + 1, y) && available_move(x + 1, y)) {
				if (move_count[0] < 4) {
					x += 1;
					room[y][x] = way;
					move_count[0]++;
					move_count[1] = 0;
					move_count[2] = 0;
					move_count[3] = 0;
				}
			}
			break;
		case down:
			if (in_range(x, y + 1) && available_move(x, y + 1)) {
				if (move_count[1] < 4) {
					y += 1;
					room[y][x] = way;
					move_count[0] = 0;
					move_count[1]++;
					move_count[2] = 0;
					move_count[3] = 0;
				}
			}
			break;
		case left:
			if (in_range(x - 1, y) && available_move(x - 1, y)) {
				if (move_count[2] < 4) {
					x -= 1;
					room[y][x] = way;
					move_count[0] = 0;
					move_count[1] = 0;
					move_count[2]++;
					move_count[3] = 0;
				}
			}
			break;
		case up:
			if (in_range(x, y - 1) && available_move(x, y - 1)) {
				if (move_count[3] < 4) {
					y -= 1;
					room[y][x] = way;
					move_count[0] = 0;
					move_count[1] = 0;
					move_count[2] = 0;
					move_count[3]++;
				}
			}
			break;
		}

		print_board();
	}
	
}

void move_me(int command) {
	switch (command) {
	case right:
		if (available_move_me(me_x + 1, me_y)) {
			room[me_y][me_x] = way;
			me_x += 1;
			room[me_y][me_x] = me;
		}
		break;
	case down:
		if (available_move_me(me_x, me_y + 1)) {
			room[me_y][me_x] = way;
			me_y += 1;
			room[me_y][me_x] = me;
		}
		break;
	case left:
		if (available_move_me(me_x - 1, me_y)) {
			room[me_y][me_x] = way;
			me_x -= 1;
			room[me_y][me_x] = me;
		}
		break;
	case up:
		if (available_move_me(me_x, me_y - 1)) {
			room[me_y][me_x] = way;
			me_y -= 1;
			room[me_y][me_x] = me;
		}
		break;
	}
}

_Bool in_range(int x, int y) {
	return ((-1 < x && x < MAX_X) && (-1 < y && y < MAX_Y));
}

_Bool available_move(int x, int y) {
	return (room[y][x] == background);
}

_Bool available_move_me(int x, int y) {
	return (room[y][x] == way);
}