#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <Windows.h>

#define MAX_X 30
#define MAX_Y 30
#define MAX_OBSTACLE 30
#define MAX_MOVE 5

enum{way, background, obstacle};
enum{right, down, left, up};

typedef struct room {
	int type;
	int distance;
}ROOM;

ROOM room[MAX_Y][MAX_X];
int move_count = 0;

void reset_board();
void create_obstacle();
void find_distance();
void print_board();
void search_way(int x, int y, int direction);

_Bool in_range(int x, int y);
_Bool available_move(int x, int y);

int main() {
	srand(time(NULL));

	reset_board();
	create_obstacle();
	find_distance();
	search_way(0, 0, rand()%2);
	print_board();
	return 0;
}

void reset_board(){
	for (int y = 0; y < MAX_Y; y++) {
		for (int x = 0; x < MAX_X; x++) {
			if(room[y][x].type != obstacle)	room[y][x].type = background;
			room[y][x].distance = 0;
		}
	}
	room[0][0].type = way;
}

void create_obstacle() {
	int obstacle_count = 0;
	int x, y;

	while (obstacle_count < MAX_OBSTACLE) {
		x = rand() % MAX_X;
		y = rand() % MAX_Y;

		if (room[y][x].type == background) {
			room[y][x].type = obstacle;
			obstacle_count++;
		}
	}
}

void find_distance() {
	for (int y = 0; y < MAX_Y; y++) {
		for (int x = 0; x < MAX_X; x++) {
			if (room[y][x].type != obstacle) {
				room[y][x].distance = (MAX_X - x - 1) + (MAX_Y - y - 1);
			}
		}
	}
}

void print_board() {
	for (int y = 0; y < MAX_Y; y++) {
		for (int x = 0; x < MAX_X; x++) {
			switch (room[y][x].type) {
			case way: 
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
				printf("%3c", '0'); 
				break;
			case background: 
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
				printf("%3d", room[y][x].distance); 
				break;
			case obstacle: 
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 3);
				printf("%3c", 'X'); 
				break;
			}
		}
		printf("\n");
	}
}

void search_way(int x, int y, int direction) {

}

_Bool in_range(int x, int y) {
	return ((-1 < x && x < MAX_X) && (-1 < y && y < MAX_Y));
}

_Bool available_move(int x, int y) {
	return (room[y][x].type == background);
}