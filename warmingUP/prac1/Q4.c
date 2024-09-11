#define _CRT_SECURE_NO_WARINGS

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <windows.h>

#define MAX_X 5
#define MAX_Y 5

//카드 고유번호
#define A 0
#define B 1
#define C 2
#define D 3
#define E 4
#define F 5
#define G 6
#define H 7
#define I 8
#define J 9
#define K 10
#define L 11
#define JOKER 12

typedef struct card {
	int x, y;
	int card_character;
	int color;
	_Bool open, fill;
}CARD;

CARD card[MAX_Y][MAX_Y];
int first_card[2]; // first open card의 x, y 좌표
int second_card[2]; // second open card의 x, y 좌표

void put_card(int x, int y, int card_number);
void mix_card();
void print_board();
void first_select_card();
void second_select_card();
void compare_card();
void find_another_card(int number);

int main() {
	mix_card();
	print_board();

	while (1) {
		first_select_card();
	}
	return 0;
}

void put_card(int x, int y, int card_number, int* card_count, int* board_count) {
	if (*card_count > 0) {
		int c_count = *card_count;
		int b_count = *board_count;

		card[y][x].x = x;
		card[y][x].y = y;
		card[y][x].fill = 1;

		switch (card_number) {
		case A:
			card[y][x].card_character = 'A';
			break;
		case B:
			card[y][x].card_character = 'B';
			break;
		case C:
			card[y][x].card_character = 'C';
			break;
		case D:
			card[y][x].card_character = 'D';
			break;
		case E:
			card[y][x].card_character = 'E';
			break;
		case F:
			card[y][x].card_character = 'F';
			break;
		case G:
			card[y][x].card_character = 'G';
			break;
		case H:
			card[y][x].card_character = 'H';
			break;
		case I:
			card[y][x].card_character = 'I';
			break;
		case J:
			card[y][x].card_character = 'J';
			break;
		case K:
			card[y][x].card_character = 'K';
			break;
		case L:
			card[y][x].card_character = 'L';
			break;
		case JOKER:
			card[y][x].card_character = '@';
			break;
		}
		
		c_count--;
		b_count--;

		*card_count = c_count;
		*board_count = b_count;
	}
}

void mix_card() {
	srand(time(NULL));
	int card_count[13] = { 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1};
	int board_count = 25;

	while (board_count != 0) {
		int card_number = rand()%13;
		int board_x = rand() % 5, board_y = rand() % 5;

		if (!card[board_y][board_x].fill) {
			switch (card_number) {
			case A:
				put_card(board_x, board_y, card_number, &card_count[A], &board_count);
				break;
			case B:
				put_card(board_x, board_y, card_number, &card_count[B], &board_count);
				break;
			case C:
				put_card(board_x, board_y, card_number, &card_count[C], &board_count);
				break;
			case D:
				put_card(board_x, board_y, card_number, &card_count[D], &board_count);
				break;
			case E:
				put_card(board_x, board_y, card_number, &card_count[E], &board_count);
				break;
			case F:
				put_card(board_x, board_y, card_number, &card_count[F], &board_count);
				break;
			case G:
				put_card(board_x, board_y, card_number, &card_count[G], &board_count);
				break;
			case H:
				put_card(board_x, board_y, card_number, &card_count[H], &board_count);
				break;
			case I:
				put_card(board_x, board_y, card_number, &card_count[I], &board_count);
				break;
			case J:
				put_card(board_x, board_y, card_number, &card_count[J], &board_count);
				break;
			case K:
				put_card(board_x, board_y, card_number, &card_count[K], &board_count);
				break;
			case L:
				put_card(board_x, board_y, card_number, &card_count[L], &board_count);
				break;
			case JOKER:
				put_card(board_x, board_y, card_number, &card_count[JOKER], &board_count);
				break;
			}
		}
	}
}

void print_board() {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 3);
	printf("%2c|%3c%3c%3c%3c%3c\n", ' ', 'a', 'b', 'c', 'd', 'e');
	printf("-------------------\n");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);

	for (int i = 0; i < 5; i++) {
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 3);
		printf("%2d|", i + 1);
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);

		for (int j = 0; j < 5; j++) {
			if (card[i][j].open) {
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), card[i][j].color);
				printf("%3c", card[i][j].card_character);
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
			}
			else {
				printf("%3c", '*');
			}
		}
		printf("\n");
	}
}

void first_select_card() {
	int i = 0;

	printf("FIRST OPEN CARD : ");
	while (i < 2) {
		first_card[i++] = getchar();
	}
	fseek(stdin, 0, SEEK_END);
	first_card[0] -= 97;
	first_card[1] -= 49;

	if (first_card[0] > -1 && first_card[0] < 5 && first_card[1] > -1 && first_card[1] < 5) {
		card[first_card[1]][first_card[0]].open = 1;
		card[first_card[1]][first_card[0]].color = 1;
		print_board();
		second_select_card();
	}
	else { 
		printf("Wrong card\n"); 
		print_board();
	}
}

void second_select_card() {
	int i = 0;

	while (1) {
		printf("SECOND OPEN CARD : ");
		while (i < 2) {
			second_card[i++] = getchar();
		}
		fseek(stdin, 0, SEEK_END);
		second_card[0] -= 97;
		second_card[1] -= 49;
		//첫번째 좌표와 두번째 좌표가 같으면 다시 코드 짜기
		if (second_card[0] > -1 && second_card[0] < 5 && second_card[1] > -1 && second_card[1] < 5) {
			card[second_card[1]][second_card[0]].open = 1;
			card[second_card[1]][second_card[0]].color = 5;
			break;
		}
		else { printf("Wrong locate\n"); }
	}

	print_board();

	compare_card();
}

void compare_card() {
	if (card[first_card[1]][first_card[0]].card_character == '@' || card[first_card[1]][first_card[0]].card_character == '@') {
		if (card[first_card[1]][first_card[0]].card_character == '@')
			find_another_card(0);
		else
			find_another_card(1);
		
		printf("Correct!\n");
		printf("Another Card Open\n");
	}
	else if (card[first_card[1]][first_card[0]].card_character != card[second_card[1]][second_card[0]].card_character) {
		card[first_card[1]][first_card[0]].open = 0;
		card[second_card[1]][second_card[0]].open = 0;

		printf("Wrong Card\n");
	}
	else {
		card[first_card[1]][first_card[0]].color = 8;
		card[second_card[1]][second_card[0]].color = 8;
		printf("Correct!\n");
	}

	print_board();
}

void find_another_card(int number) {
	switch (number) {
	case 0:
		for (int i = 0; i < 5; i++) {
			for (int j = 0; j < 5; j++) {
				if (card[i][j].card_character == card[first_card[1]][first_card[0]].card_character) {
					if (i != first_card[1] && j != first_card[0]) {
						card[i][j].open = 1;
						card[i][j].color = 14;
						return;
					}
				}
			}
		}
	case 1:
		for (int i = 0; i < 5; i++) {
			for (int j = 0; j < 5; j++) {
				if (card[i][j].card_character == card[second_card[1]][second_card[0]].card_character) {
					if (i != second_card[1] && j != second_card[0]) {
						card[i][j].open = 1;
						card[i][j].color = 2;
						return;
					}
				}
			}
		}
	default: break;
	}
}