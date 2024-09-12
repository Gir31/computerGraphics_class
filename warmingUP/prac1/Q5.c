#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <Windows.h>

#define board_range 30

typedef struct board {

	int x, y; // 좌표값
	_Bool quad; // 0: 일반 보드 1 : 사각형 범위 

}BOARD;

void print_board();
void insert_quad_range();
void create_quad();
void move_right(); //오른쪽으로 이동

BOARD board[board_range][board_range];
int quad_range[2][2]; //[0]에는 시작 좌표 [1]에는 끝 좌표

int main() {
	insert_quad_range();

	while (1) {
		print_board();
		move_right();
		Sleep(500);
		system("cls");
	}


	return 0;
}

void print_board() {

	for (int y = 0; y < board_range; y++) {
		for (int x = 0; x < board_range; x++) {
			if (board[y][x].quad) printf("%2d", 0);
			else printf("%2c", '.');
		}
		printf("\n");
	}
	printf("\n");
}

// 나중에 어떤 좌표를 입력하더라도 사각형이 생성되게 고치기
void insert_quad_range() {
	printf("좌표 입력 : ");
	scanf("%d %d %d %d", &quad_range[0][0], &quad_range[0][1], &quad_range[1][0], &quad_range[1][1]);

	create_quad();

	printf("\n");
}

void create_quad() {

	for (int y = quad_range[0][1] - 1; y < quad_range[1][1]; y++) {
		for (int x = quad_range[0][0] - 1; x < quad_range[1][0]; x++) {
			board[y][x].quad = 0;
		}
	}

	for (int y = quad_range[0][1] - 1; y < quad_range[1][1]; y++) {
		for (int x = quad_range[0][0] - 1; x < quad_range[1][0]; x++) {
			board[y][x].quad = 1;
		}
	}
}

void move_right() {

	quad_range[0][0] -= 1;
	if (quad_range[0][0] < 0) quad_range[0][0] = board_range - 1;
	quad_range[1][0] -= 1;
	if (quad_range[1][0] < 0) quad_range[1][0] = board_range - 1;

	create_quad();
}