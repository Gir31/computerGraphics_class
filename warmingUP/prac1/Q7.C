#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <math.h>
#include <Windows.h>

#define board_range 30

enum{background, quad_1, quad_2, quad_stack};

int board_type[board_range][board_range];

int quad_range1[2][2];
int quad_range2[2][2];

void menu();

void print_board();
void reset_board();
void insert_quad_range();

void create_quad1();
void create_quad2();

void move_right1();
void move_left1();
void move_up1();
void move_down1();

void move_right2();
void move_left2();
void move_up2();
void move_down2();

void expand_x1();
void reduce_x1();
void expand_y1();
void reduce_y1();

void expand_x2();
void reduce_x2();
void expand_y2();
void reduce_y2();

int main() {
	menu();
	return 0;
}

void menu() {
	char command;

	print_board();
	insert_quad_range();

	while (1) {

		print_board();

		printf("첫번째 도형\n");
		printf("----이동----\n%3c%3c%3c\n%3c%3c%3c\n--크기조정--\n-%4c|+%4c\n\n", ' ', 'w', ' ', 'a', 's', 'd', 't', 'y');
		printf("두번째 도형\n");
		printf("----이동----\n%3c%3c%3c\n%3c%3c%3c\n--크기조정--\n-%4c|+%4c\n\n", ' ', '8', ' ', '4', '5', '6', '7', '9');
		printf("리셋 : r / 종료 : q\n");
		printf("command : ");

		command = getchar();
		fseek(stdin, 0, SEEK_END);

		switch (command) {
		case 'w':move_up1(); break;
		case 'a':move_left1(); break;
		case 's':move_down1(); break;
		case 'd':move_right1(); break;
		case 't':reduce_x1(); reduce_y1(); break;
		case 'y':expand_x1(); expand_y1(); break;
		case '8':move_up2(); break;
		case '4':move_left2(); break;
		case '5':move_down2(); break;
		case '6':move_right2(); break;
		case '7':reduce_x2(); reduce_y2(); break;
		case '9':expand_x2(); expand_y2(); break;
		case 'r': 
			reset_board(); 
			Sleep(100);
			system("cls");
			print_board(); 
			insert_quad_range(); 
			break;
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
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
			switch (board_type[y][x]) {
			case background:
				printf("%2c", '.');
				break;
			case quad_1:
				printf("%2c", 'O');
				break;
			case quad_2:
				printf("%2c", 'X');
				break;
			case quad_stack:
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 3);
				printf("%2c", '#');
				break;
			}
		}
		printf("\n");
	}
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
	printf("\n");
}

void reset_board() {
	for (int y = 0; y < board_range; y++) {
		for (int x = 0; x < board_range; x++) {
			board_type[y][x] = 0;
		}
	}
}

void create_quad1() {
	// 모든 보드 초기화
	reset_board();

	switch (quad_range1[0][1] <= quad_range1[1][1]) {
	case 0:
		switch (quad_range1[0][0] <= quad_range1[1][0]) {
		case 0:
			for (int y = 0; y < quad_range1[1][1]; y++) {
				for (int x = 0; x < quad_range1[1][0]; x++) {
					board_type[y][x]++;
				}

				for (int x = quad_range1[0][0] - 1; x < board_range; x++) {
					board_type[y][x]++;
				}
			}

			for (int y = quad_range1[0][1] - 1; y < board_range; y++) {
				for (int x = 0; x < quad_range1[1][0]; x++) {
					board_type[y][x]++;
				}

				for (int x = quad_range1[0][0] - 1; x < board_range; x++) {
					board_type[y][x]++;
				}
			}
			break;
		case 1:
			for (int y = 0; y < quad_range1[1][1]; y++) {
				for (int x = quad_range1[0][0] - 1; x < quad_range1[1][0]; x++) {
					board_type[y][x]++;
				}
			}

			for (int y = quad_range1[0][1] - 1; y < board_range; y++) {
				for (int x = quad_range1[0][0] - 1; x < quad_range1[1][0]; x++) {
					board_type[y][x]++;
				}
			}
			break;
		}
		break;
	case 1:
		switch (quad_range1[0][0] <= quad_range1[1][0]) {
		case 0:
			for (int y = quad_range1[0][1] - 1; y < quad_range1[1][1]; y++) {
				for (int x = 0; x < quad_range1[1][0]; x++) {
					board_type[y][x]++;
				}

				for (int x = quad_range1[0][0] - 1; x < board_range; x++) {
					board_type[y][x]++;
				}
			}
			break;
		case 1:
			for (int y = quad_range1[0][1] - 1; y < quad_range1[1][1]; y++) {
				for (int x = quad_range1[0][0] - 1; x < quad_range1[1][0]; x++) {
					board_type[y][x]++;
				}
			}
			break;
		}
		break;
	}

	create_quad2();
}

void create_quad2() {

	switch (quad_range2[0][1] <= quad_range2[1][1]) {
	case 0:
		switch (quad_range2[0][0] <= quad_range2[1][0]) {
		case 0:
			for (int y = 0; y < quad_range2[1][1]; y++) {
				for (int x = 0; x < quad_range2[1][0]; x++) {
					board_type[y][x] += 2;
				}

				for (int x = quad_range2[0][0] - 1; x < board_range; x++) {
					board_type[y][x] += 2;
				}
			}

			for (int y = quad_range2[0][1] - 1; y < board_range; y++) {
				for (int x = 0; x < quad_range2[1][0]; x++) {
					board_type[y][x] += 2;
				}

				for (int x = quad_range2[0][0] - 1; x < board_range; x++) {
					board_type[y][x] += 2;
				}
			}
			break;
		case 1:
			for (int y = 0; y < quad_range2[1][1]; y++) {
				for (int x = quad_range2[0][0] - 1; x < quad_range2[1][0]; x++) {
					board_type[y][x] += 2;
				}
			}

			for (int y = quad_range2[0][1] - 1; y < board_range; y++) {
				for (int x = quad_range2[0][0] - 1; x < quad_range2[1][0]; x++) {
					board_type[y][x] += 2;
				}
			}
			break;
		}
		break;
	case 1:
		switch (quad_range2[0][0] <= quad_range2[1][0]) {
		case 0:
			for (int y = quad_range2[0][1] - 1; y < quad_range2[1][1]; y++) {
				for (int x = 0; x < quad_range2[1][0]; x++) {
					board_type[y][x] += 2;
				}

				for (int x = quad_range2[0][0] - 1; x < board_range; x++) {
					board_type[y][x] += 2;
				}
			}
			break;
		case 1:
			for (int y = quad_range2[0][1] - 1; y < quad_range2[1][1]; y++) {
				for (int x = quad_range2[0][0] - 1; x < quad_range2[1][0]; x++) {
					board_type[y][x] += 2;
				}
			}
			break;
		}
		break;
	}
}

void insert_quad_range() {
	printf("input coord value1 : ");
	scanf("%d %d %d %d", &quad_range1[0][0], &quad_range1[0][1], &quad_range1[1][0], &quad_range1[1][1]);

	printf("input coord value2 : ");
	scanf("%d %d %d %d", &quad_range2[0][0], &quad_range2[0][1], &quad_range2[1][0], &quad_range2[1][1]);

	create_quad1();

	printf("\n");
}

void move_right1() {
	quad_range1[0][0] += 1;
	if (quad_range1[0][0] == board_range + 1) quad_range1[0][0] = 1;
	quad_range1[1][0] += 1;
	if (quad_range1[1][0] == board_range + 1) quad_range1[1][0] = 1;

	create_quad1();
}

void move_left1() {
	quad_range1[0][0] -= 1;
	if (quad_range1[0][0] == 0) quad_range1[0][0] = board_range;
	quad_range1[1][0] -= 1;
	if (quad_range1[1][0] == 0) quad_range1[1][0] = board_range;

	create_quad1();
}

void move_up1() {

	quad_range1[0][1] -= 1;
	if (quad_range1[0][1] == 0) quad_range1[0][1] = board_range;
	quad_range1[1][1] -= 1;
	if (quad_range1[1][1] == 0) quad_range1[1][1] = board_range;

	create_quad1();
}

void move_down1() {

	quad_range1[0][1] += 1;
	if (quad_range1[0][1] == board_range + 1) quad_range1[0][1] = 1;
	quad_range1[1][1] += 1;
	if (quad_range1[1][1] == board_range + 1) quad_range1[1][1] = 1;

	create_quad1();
}

void move_right2() {
	quad_range2[0][0] += 1;
	if (quad_range2[0][0] == board_range + 1) quad_range2[0][0] = 1;
	quad_range2[1][0] += 1;
	if (quad_range2[1][0] == board_range + 1) quad_range2[1][0] = 1;


	create_quad1();
}

void move_left2() {
	quad_range2[0][0] -= 1;
	if (quad_range2[0][0] == 0) quad_range2[0][0] = board_range;
	quad_range2[1][0] -= 1;
	if (quad_range2[1][0] == 0) quad_range2[1][0] = board_range;

	create_quad1();
}

void move_up2() {

	quad_range2[0][1] -= 1;
	if (quad_range2[0][1] == 0) quad_range2[0][1] = board_range;
	quad_range2[1][1] -= 1;
	if (quad_range2[1][1] == 0) quad_range2[1][1] = board_range;

	create_quad1();
}

void move_down2() {

	quad_range2[0][1] += 1;
	if (quad_range2[0][1] == board_range + 1) quad_range2[0][1] = 1;
	quad_range2[1][1] += 1;
	if (quad_range2[1][1] == board_range + 1) quad_range2[1][1] = 1;

	create_quad1();
}

void expand_x1() {
	if (quad_range1[1][0] != quad_range1[0][0] - 1) quad_range1[1][0] += 1;
	create_quad1();
}

void reduce_x1() {
	if (quad_range1[1][0] != quad_range1[0][0]) quad_range1[1][0] -= 1;
	create_quad1();
}

void expand_y1() {
	if (quad_range1[1][1] != quad_range1[0][1] - 1) quad_range1[1][1] += 1;
	create_quad1();
}

void reduce_y1() {
	if (quad_range1[1][1] != quad_range1[0][1]) quad_range1[1][1] -= 1;
	create_quad1();
}

void expand_x2() {
	if (quad_range2[1][0] != quad_range2[0][0] - 1) quad_range2[1][0] += 1;
	create_quad1();
}

void reduce_x2() {
	if (quad_range2[1][0] != quad_range2[0][0]) quad_range2[1][0] -= 1;
	create_quad1();
}

void expand_y2() {
	if (quad_range2[1][1] != quad_range2[0][1] - 1) quad_range2[1][1] += 1;
	create_quad1();
}

void reduce_y2() {
	if (quad_range2[1][1] != quad_range2[0][1]) quad_range2[1][1] -= 1;
	create_quad1();
}