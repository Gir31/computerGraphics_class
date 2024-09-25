#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <Windows.h>

#define MAX_X 30
#define MAX_Y 30
#define MAX_OBSTACLE 30

typedef struct board {
	int direction, type;
}BOARD;

BOARD b[MAX_Y][MAX_X];
int me_x = 0, me_y = 0;

enum{way, background, obstacle, me};
enum { right, down, left, up };

void reset_board();
void create_obstacle();
void print_board();
void search_way();

_Bool possible_way(int x, int y);
_Bool in_range(int x, int y);
_Bool in_background(int x, int y);

int main() {
	srand(time(NULL));

	reset_board();
	create_obstacle();
	search_way();

	return 0;
}

void reset_board() {
	for (int y = 0; y < MAX_Y; y++) {
		for (int x = 0; x < MAX_X; x++) {
			if (b[y][x].type == way)	b[y][x].type = background;
		}
	}
	b[0][0].type = way;
	b[0][0].direction = rand() % 2;
}

void create_obstacle() {
	int obstacle_count = 0;
	int x, y;

	while (obstacle_count < MAX_OBSTACLE) {
		x = rand() % MAX_X;
		y = rand() % MAX_Y;

		if (b[y][x].type == background && (x != MAX_X - 1 && y != MAX_Y - 1)) {
			b[y][x].type = obstacle;
			obstacle_count++;
		}
	}
}

void print_board() {
	system("cls");
	for (int y = 0; y < MAX_Y; y++) {
		for (int x = 0; x < MAX_X; x++) {
			switch (b[y][x].type) {
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

_Bool possible_way(int x, int y) {
	if (in_range(x, y) && in_background(x, y)) return 1;
	else return 0;
}

_Bool in_range(int x, int y) {
	return ((-1 < x && x < MAX_X) && (-1 < y && y < MAX_Y));
}

_Bool in_background(int x, int y) {
	return (b[y][x].type == background);
}

void search_way() {
	int move;
	int direction;
	int x = 0, y = 0;

	while (1) {
		if (possible_way(x + 1, y)) {
			if (possible_way(x - 1, y)) {
				if(possible_way(x, y + 1)) {
					if (possible_way(x, y - 1)) {
						reset_board();
						x = 0;
						y = 0;
					}
				}
			}
		}

		move = (rand() % 4) + 1;

		switch (b[y][x].direction) {
		case right:
			for (int i = 0; i < move; i++) {
				if (possible_way(x + 1, y)) {
					x += 1;
					b[y][x].type = way;
				}
				else {
					break;
				}
			}
			b[y][x].direction = (rand() % 2) * 2 + 1;
			break;
		case down:
			for (int i = 0; i < move; i++) {
				if (possible_way(x, y + 1)) {
					y += 1;
					b[y][x].type = way;
				}
				else {
					
					break;
				}
			}
			b[y][x].direction = (rand() % 2) * 2;
			break;
		case left:
			for (int i = 0; i < move; i++) {
				if (possible_way(x - 1, y)) {
					x -= 1;
					b[y][x].type = way;
				}
				else {
					
					break;
				}
			}
			b[y][x].direction = (rand() % 2) * 2 + 1;
			break;
		case up:
			for (int i = 0; i < move; i++) {
				if (possible_way(x, y - 1)) {
					y -= 1;
					b[y][x].type = way;
				}
				else {
					
					break;
				}
			}
			b[y][x].direction = (rand() % 2) * 2;
			break;
		}

		if (x == MAX_X - 1 && y == MAX_Y - 1) break;

		print_board();
	}
}