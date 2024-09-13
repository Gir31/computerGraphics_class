#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <Windows.h>

#define board_range 30

typedef struct board {

	int x, y; // ��ǥ��
	_Bool quad; // 0: �Ϲ� ���� 1 : �簢�� ���� 

}BOARD;

void print_board();
void insert_quad_range();
void reset_board();
void create_quad();
void move_right();
void move_left();
void move_up();
void move_down();

BOARD board[board_range][board_range];
int quad_range[2][2]; //[0]���� ���� ��ǥ [1]���� �� ��ǥ

int main() {
	insert_quad_range();

	while (1) {
		print_board();
		move_down();
		Sleep(200);
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
	printf("\n");
}

// ���߿� � ��ǥ�� �Է��ϴ��� �簢���� �����ǰ� ��ġ��
void insert_quad_range() {
	printf("��ǥ �Է� : ");
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
	// ��� ���� �ʱ�ȭ
	reset_board();

	switch (quad_range[0][1] <= quad_range[1][1]) {
	case 0:
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
	if (quad_range[0][0] == board_range) quad_range[0][0] = 1;
	quad_range[1][0] += 1;
	if (quad_range[1][0] == board_range) quad_range[1][0] = 1;

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
	if (quad_range[0][1] == 0) quad_range[0][1] = 1;
	quad_range[1][1] += 1;
	if (quad_range[1][1] == 0) quad_range[1][1] = 1;

	create_quad();
}