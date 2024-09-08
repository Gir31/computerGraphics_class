#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct coordinate {
	int number; //����
	float length; //�������κ����� �Ÿ�
	_Bool fill; //��ǥ ���� �����ϴ°�
	int x, y, z; //��ǥ ��
}COORDINATE;

static COORDINATE coor[20];

void createCoordinate();
void newCoordinate_upper(int x, int y, int z);

float findLength(int x, int y, int z); //�������κ����� �Ÿ��� ���ϴ� �Լ�

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

		if (number == -1) printf("List�� ���� á���ϴ�.\n");
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