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
void newCoordinate_lower(int x, int y, int z);
void deleteCoordinate_lower();
void printCoordinate();

float findLength(int x, int y, int z); //원점으로부터의 거리를 구하는 함수

int main() {
	createCoordinate();
	newCoordinate_upper(1, 2, 3);
	newCoordinate_upper(2, 2, 5);
	newCoordinate_upper(3, 8, 4);
	newCoordinate_upper(1, 2, 3);
	newCoordinate_upper(2, 2, 5);
	newCoordinate_upper(3, 8, 4);
	newCoordinate_upper(1, 2, 3);
	newCoordinate_upper(2, 2, 5);
	newCoordinate_upper(3, 8, 4);
	newCoordinate_upper(1, 2, 3);
	newCoordinate_upper(2, 2, 5);
	newCoordinate_upper(3, 8, 4);
	newCoordinate_upper(1, 2, 3);
	newCoordinate_upper(2, 2, 5);
	newCoordinate_upper(3, 8, 4);
	newCoordinate_upper(1, 2, 3);
	newCoordinate_upper(2, 2, 5);
	newCoordinate_upper(3, 8, 4);
	newCoordinate_upper(1, 2, 3);
	newCoordinate_upper(2, 2, 5);
	deleteCoordinate_lower();
	deleteCoordinate_lower();
	newCoordinate_upper(5, 5, 5);
	newCoordinate_upper(10,10,10);
	newCoordinate_upper(15, 15, 15);

	printCoordinate();
	
	
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
			if (!coor[i].fill) {
				number = i;
				break;
			}
			number += 2;
		}

		if (number > 19) printf("List가 가득 찼습니다.\n");
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

void newCoordinate_lower(int x, int y, int z) {
	int upCount = 0, downCount = 0;

	for (int i = 0; i < 20; i++) {
		if (coor[i].fill) {
			upCount = i;
			break;
		}
	}
	for (int i = 19; i >= 0; i--) {
		if (!coor[i].fill) {
			downCount = i;
			break;
		}
	}

	// 초기
	// 0번이 차있고 19번이 빌경우
	// 0번이 비고, 19번도 빌경우
	// 0번이 비고, 19번이 찰 경우
	// 0번이 차있고, 19번도 차있을 경우, 하지만 사이가 비어있을 경우
	// 전부 찼을 경우
}

void deleteCoordinate_lower() {
	for (int i = 0; i < 20; i++) {
		if (coor[i].fill) {
			coor[i].fill = 0;
			break;
		}
	}
}

void printCoordinate() {
	printf("|%6s | %3c %3c %3c | %5s|\n", "Number", 'X', 'Y', 'Z', "Length");
	for (int i = 9; i >= 0; i--) {
		if (coor[i].fill) {
			printf("|%6d | %3d %3d %3d | %6.2lf|\n", i, coor[i].x, coor[i].y, coor[i].z, coor[i].length);
		}
		else {
			printf("|%6d | %3c %3c %3c | %6c|\n", i, '-', '-', '-', '-');
		}
	}
}

float findLength(int x, int y, int z) {return (float)(sqrt((pow(x, 2) + pow(y, 2) + pow(z, 2))));}