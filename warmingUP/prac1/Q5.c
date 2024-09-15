#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <math.h>
#include <Windows.h>

#define board_range 30

typedef struct board {

	int x, y; // 좌표값
	_Bool quad; // 0: 일반 보드 1 : 사각형 범위 

}BOARD;

void print_board();
void insert_quad_range();

void reset_board();
void create_quad();

void move_right();
void move_left();
void move_up();
void move_down();

void expand_x();
void reduce_x();
void expand_y();
void reduce_y();

int quad_area();
double quad_ratio();

BOARD board[board_range][board_range];
int quad_range[2][2]; //[0]에는 시작 좌표 [1]에는 끝 좌표

int main() {
	char command;

	print_board();
	insert_quad_range();

	while (1) {

		print_board();

		printf("이동 : w a s d / 사각형 축소, 확대 : [ ]\n");
		printf("x 값 축소, 확대 : o p / y 값 축소, 확대 : u i\n");
		printf("x 값 확대, y값 축소 : t / x 값 축소, y 값 확대 : y\n");
		printf("사각형 면적 : m / 사각형 비율 : n / 리셋 : r / 종료 : q\n");
		printf("command : ");

		command = getchar();
		fseek(stdin, 0, SEEK_END);

		switch (command) {
		case 'w': move_up();  break;
		case 'a': move_right(); break;
		case 's': move_down(); break;
		case 'd': move_left(); break;
		case '[':
			reduce_x();
			reduce_y();
			break;
		case ']':
			expand_x();
			expand_y();
			break;
		case 'o':
			reduce_x();
			break;
		case 'p':
			expand_x();
			break;
		case 'u':
			reduce_y();
			break;
		case 'i':
			expand_y();
			break;
		case 't':
			expand_x();
			reduce_y();
			break;
		case 'y':
			reduce_x();
			expand_y();
			break;
		case 'm':
			printf("현재 사각형의 면적 : %d\n\n", quad_area());
			Sleep(1000);
			break;
		case 'n':
			printf("현재 사각형의 면적 비율 : %0.2lf %%\n\n", quad_ratio());
			Sleep(1000);
			break;
		case 'r': insert_quad_range(); break;
		case 'q': return 0;
		default:  break;
		}

		Sleep(100);
		system("cls");
	}


	return 0;
}

void print_board() {

	for (int y = 0; y < board_range; y++) {
		for (int x = 0; x < board_range; x++) {
			if (board[y][x].quad) {
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 3);
				printf("%2d", 0);
			}
			else {
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
				printf("%2c", '.');
			}
		}
		printf("\n");
	}
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
	printf("\n");
}

void insert_quad_range() {
	printf("좌표 입력 : ");
	scanf("%d %d %d %d", &quad_range[0][0], &quad_range[0][1], &quad_range[1][0], &quad_range[1][1]);

	create_quad();

	printf("\n");
}

void reset_board() {
	for (int y = 0; y < board_range; y++) {
		for (int x = 0; x < board_range; x++) {
			board[y][x].quad = 0;
		}
	}
}

void create_quad() {
	// 모든 보드 초기화
	reset_board();

	switch (quad_range[0][1] <= quad_range[1][1]) {
	case 0:
		switch (quad_range[0][0] <= quad_range[1][0]) {
		case 0:
			for (int y = 0; y < quad_range[1][1]; y++) {
				for (int x = 0; x < quad_range[1][0]; x++) {
					board[y][x].quad = 1;
				}

				for (int x = quad_range[0][0] - 1; x < board_range; x++) {
					board[y][x].quad = 1;
				}
			}

			for (int y = quad_range[0][1] - 1; y < board_range; y++) {
				for (int x = 0; x < quad_range[1][0]; x++) {
					board[y][x].quad = 1;
				}

				for (int x = quad_range[0][0] - 1; x < board_range; x++) {
					board[y][x].quad = 1;
				}
			}
			break;
		case 1:
			for (int y = 0; y < quad_range[1][1]; y++) {
				for (int x = quad_range[0][0] - 1; x < quad_range[1][0]; x++) {
					board[y][x].quad = 1;
				}
			}

			for (int y = quad_range[0][1] - 1; y < board_range; y++) {
				for (int x = quad_range[0][0] - 1; x < quad_range[1][0]; x++) {
					board[y][x].quad = 1;
				}
			}
			break;
		}
		break;
	case 1:
		switch (quad_range[0][0] <= quad_range[1][0]) {
		case 0:
			for (int y = quad_range[0][1] - 1; y < quad_range[1][1]; y++) {
				for (int x = 0; x < quad_range[1][0]; x++) {
					board[y][x].quad = 1;
				}

				for (int x = quad_range[0][0] - 1; x < board_range; x++) {
					board[y][x].quad = 1;
				}
			}
			break;
		case 1:
			for (int y = quad_range[0][1] - 1; y < quad_range[1][1]; y++) {
				for (int x = quad_range[0][0] - 1; x < quad_range[1][0]; x++) {
					board[y][x].quad = 1;
				}
			}
			break;
		}
		break;
	}
}

void move_right() {

	quad_range[0][0] -= 1;
	if (quad_range[0][0] == 0) quad_range[0][0] = board_range;
	quad_range[1][0] -= 1;
	if (quad_range[1][0] == 0) quad_range[1][0] = board_range;

	create_quad();
}

void move_left() {

	quad_range[0][0] += 1;
	if (quad_range[0][0] == board_range + 1) quad_range[0][0] = 1;
	quad_range[1][0] += 1;
	if (quad_range[1][0] == board_range + 1) quad_range[1][0] = 1;

	create_quad();
}

void move_up() {

	quad_range[0][1] -= 1;
	if (quad_range[0][1] == 0) quad_range[0][1] = board_range;
	quad_range[1][1] -= 1;
	if (quad_range[1][1] == 0) quad_range[1][1] = board_range;

	create_quad();
}

void move_down() {

	quad_range[0][1] += 1;
	if (quad_range[0][1] == board_range + 1) quad_range[0][1] = 1;
	quad_range[1][1] += 1;
	if (quad_range[1][1] == board_range + 1) quad_range[1][1] = 1;

	create_quad();
}

void expand_x() {
	if(quad_range[1][0] != quad_range[0][0] - 1) quad_range[1][0] += 1;
	create_quad();
}

void reduce_x() {
	if (quad_range[1][0] != quad_range[0][0]) quad_range[1][0] -= 1;
	create_quad();
}

void expand_y() {
	if (quad_range[1][1] != quad_range[0][1] - 1) quad_range[1][1] += 1;
	create_quad();
}

void reduce_y() {
	if (quad_range[1][1] != quad_range[0][1]) quad_range[1][1] -= 1;
	create_quad();
}

int quad_area() {
	int sum = 0;

	switch (quad_range[0][1] <= quad_range[1][1]) {
	case 0:
		switch (quad_range[0][0] <= quad_range[1][0]) {
		case 0:
			sum = quad_range[1][0] * quad_range[1][1];
			sum += (board_range - quad_range[0][0] + 1) * quad_range[1][1];
			sum += quad_range[1][0] * (board_range - quad_range[0][1] + 1);
			sum += (board_range - quad_range[0][0] + 1) * (board_range - quad_range[0][1] + 1);
			break;
		case 1:
			sum = (quad_range[1][0] - quad_range[0][0] + 1) * quad_range[1][1];
			sum += (quad_range[1][0] - quad_range[0][0] + 1) * (board_range - quad_range[0][1] + 1);
			break;
		}
		break;
	case 1:
		switch (quad_range[0][0] <= quad_range[1][0]) {
		case 0:
			sum = quad_range[1][0] * (quad_range[1][1] - quad_range[0][1] + 1);
			sum += (board_range - quad_range[0][0] + 1) * (quad_range[1][1] - quad_range[0][1] + 1);
			break;
		case 1:
			sum = (quad_range[1][0] - quad_range[0][0] + 1) * (quad_range[1][1] - quad_range[0][1] + 1);
			break;
		}
		break;
	}

	return sum;
}

double quad_ratio() {
	return ((double)quad_area() / (pow(board_range, 2))) * 100;
}
