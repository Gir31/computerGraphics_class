#define _CRT_SECURE_NO_WARINGS

#include <stdio.h>
#include <time.h>
#include <math.h>

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
	int card_number;
	_Bool open, fill; 
}CARD;

CARD card[MAX_Y][MAX_Y];

void put_card(int x, int y, int card_number);
void mix_card();
void create_board();

int main() {
	printf("a\n");
	mix_card();
	create_board();
	return 0;
}

void put_card(int x, int y, int card_number, int* card_count, int* board_count) {
	printf("%d\n", card_count);
	if (card_count) {
		card[y][x].x = x;
		card[y][x].y = y;
		card[y][x].card_number = card_number;
		card[y][x].fill = 1;
		
		card_count--;
		board_count--;
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
				put_card(board_x, board_y, card_number, card_count[card_number], &board_count);
				break;
			case B:
				put_card(board_x, board_y, card_number, card_count[card_number], &board_count);
				break;
			case C:
				put_card(board_x, board_y, card_number, card_count[card_number], &board_count);
				break;
			case D:
				put_card(board_x, board_y, card_number, card_count[card_number], &board_count);
				break;
			case E:
				put_card(board_x, board_y, card_number, card_count[card_number], &board_count);
				break;
			case F:
				put_card(board_x, board_y, card_number, card_count[card_number], &board_count);
				break;
			case G:
				put_card(board_x, board_y, card_number, card_count[card_number], &board_count);
				break;
			case H:
				put_card(board_x, board_y, card_number, card_count[card_number], &board_count);
				break;
			case I:
				put_card(board_x, board_y, card_number, card_count[card_number], &board_count);
				break;
			case J:
				put_card(board_x, board_y, card_number, card_count[card_number], &board_count);
				break;
			case K:
				put_card(board_x, board_y, card_number, card_count[card_number], &board_count);
				break;
			case L:
				put_card(board_x, board_y, card_number, card_count[card_number], &board_count);
				break;
			case JOKER:
				put_card(board_x, board_y, card_number, card_count[card_number], &board_count);
				break;
			}
		}
	}
}

void create_board() {
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			printf("%3d ", card[i][j].card_number);
		}
		printf("\n");
	}
}