#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct coordinate {
	int number; //순서
	float length; //원점으로부터의 거리
	_Bool fill; //좌표 값이 존재하는가
	int x, y, z; //좌표 값
}COORDINATE;

static COORDINATE coor[20];

void createCoordinate();
void newCoordinate_upper(int x, int y, int z);

float findLength(int x, int y, int z); //원점으로부터의 거리를 구하는 함수

int main() {
	createCoordinate();
	newCoordinate_upper(1, 2, 3);
	newCoordinate_upper(1, 2, 3);
	newCoordinate_upper(1, 2, 3);
	newCoordinate_upper(1, 2, 3);
	newCoordinate_upper(1, 2, 3);
	newCoordinate_upper(1, 2, 3);
	newCoordinate_upper(1, 2, 3);
	newCoordinate_upper(1, 2, 3);
	newCoordinate_upper(1, 2, 3);
	newCoordinate_upper(1, 2, 3);
	newCoordinate_upper(1, 2, 3);
	newCoordinate_upper(1, 2, 3);
	newCoordinate_upper(1, 2, 3);
	newCoordinate_upper(1, 2, 3);
	newCoordinate_upper(1, 2, 3);
	newCoordinate_upper(1, 2, 3);
	newCoordinate_upper(1, 2, 3);
	newCoordinate_upper(1, 2, 3);
	newCoordinate_upper(1, 2, 3);
	newCoordinate_upper(1, 2, 3);
	newCoordinate_upper(1, 2, 3);
	newCoordinate_upper(1, 2, 3);
	newCoordinate_upper(1, 2, 3);
	newCoordinate_upper(1, 2, 3);
	newCoordinate_upper(1, 2, 3);

	for (int i = 19; i > -1; i--) {
		printf("%d : %lf\n", coor[i].number, coor[i].length);
	}
	
	
	return 0;
}

void createCoordinate() {
	for (int i = 0; i < 20; i++) {
		coor[i].fill = 0;
	}
}

void newCoordinate_upper(int x, int y, int z) {
	int number = 0;

	for (int i = 19; i > -1; i--) {
		if (coor[i].fill) {
			number = i + 1;
			break;
		}
	}

	if (number == 20) {
		for (int i = 0; i < 20; i++) {
			if (coor[i].fill) {
				number = i - 1;
				break;
			}
		}

		if (number == -1) printf("List가 가득 찼습니다.\n");
		else {
			coor[number].x = x;
			coor[number].y = y;
			coor[number].z = z;
			coor[number].number = number;
			coor[number].length = findLength(x, y, z);
			coor[number].fill = 1;
		}
	}
	else {
		coor[number].x = x;
		coor[number].y = y;
		coor[number].z = z;
		coor[number].number = number;
		coor[number].length = findLength(x, y, z);
		coor[number].fill = 1;
	}
}

float findLength(int x, int y, int z) {return (sqrt((pow(x, 2) + pow(y, 2) + pow(z, 2))));}