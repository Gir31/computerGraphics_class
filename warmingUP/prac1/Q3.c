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

COORDINATE coor[20];
COORDINATE newCoor[20];

void menu();

void createCoordinate();
void newCoordinate_upper(int x, int y, int z);
void newCoordinate_lower(int x, int y, int z);
void deleteCoordinate_lower();
void deleteCooridnate_upper();
void printCoordinate(COORDINATE* coo);
void countCoordinate();
void furthestCoordinate();
void closestCoordinate();
void uppertList();
void lowerList();

void swapList(COORDINATE** a, COORDINATE** b);

float findLength(int x, int y, int z); //�������κ����� �Ÿ��� ���ϴ� �Լ�

int main() {
	createCoordinate();

	printf("Q3. ���� ����Ʈ �����");
	
	menu();
	
	return 0;
}

void menu() {
	char command;
	int x, y, z;
	int upperCount = 0, lowerCount = 0;
	COORDINATE list[20];



	while (1) {
		printf("\n\n+ (�� ���� �Է�) / - (�� ������ ����) / e (�� �Ʒ��� �Է�) / d (�� �Ʒ����� ����\n");
		printf("l (�� ���� ���) / c (����Ʈ ����) / m (���� �� ��ǥ) / n (���� ����� ��ǥ)\n");
		printf("a (�Ÿ� ���� ��������) / s (�Ÿ� ���� ��������) / q (���α׷� ����)\n ��ɾ� : ");

		scanf("%c", &command);
		fseek(stdin, 0, SEEK_END);

		switch (command) {
		case '+' :
			printf("��ǥ �Է� (x, y, z) : ");
			scanf("%d %d %d", &x, &y, &z);
			fseek(stdin, 0, SEEK_END);
			newCoordinate_upper(x, y, z);
			printCoordinate(coor);
			break;
		case '-' :
			deleteCooridnate_upper();
			printCoordinate(coor);
			break;
		case 'e' :
			printf("��ǥ �Է� (x, y, z) : ");
			scanf("%d %d %d", &x, &y, &z);
			fseek(stdin, 0, SEEK_END);
			newCoordinate_lower(x, y, z);
			printCoordinate(coor);
			break;
		case 'd' :
			deleteCoordinate_lower();
			printCoordinate(coor);
			break;
		case 'l' :
			countCoordinate(coor);
			break;
		case 'c' : 
			createCoordinate();
			printCoordinate(coor);
			break;
		case 'm' :
			furthestCoordinate();
			break;
		case 'n' :
			closestCoordinate();
			break;
		case 'a' :
			if (lowerCount) {
				for (int i = 0; i < 20; i++) {
					list[i] = newCoor[i];
					newCoor[i] = coor[i];
					coor[i] = list[i];
				}
				lowerCount = 0;
				uppertList();
				upperCount = 1;
			}
			else if (!upperCount) {
				uppertList();
				upperCount = 1;
			}
			else {
				for (int i = 0; i < 20; i++) {
					list[i] = newCoor[i];
					newCoor[i] = coor[i];
					coor[i] = list[i];
					upperCount = 0;
				}
			}
			printCoordinate(coor);
			break;
		case 's' :
			if (upperCount) {
				for (int i = 0; i < 20; i++) {
					list[i] = newCoor[i];
					newCoor[i] = coor[i];
					coor[i] = list[i];
				}
				upperCount = 0;
				lowerList();
				lowerCount = 1;
			}
			else if (!lowerCount) {
				lowerList();
				lowerCount = 1;
			}
			else {
				for (int i = 0; i < 20; i++) {
					list[i] = newCoor[i];
					newCoor[i] = coor[i];
					coor[i] = list[i];
					lowerCount = 0;
				}
			}
			printCoordinate(coor);
			break;
		case 'q' :
			return 0;
		default :
			printf("�߸��� ��ɾ�!\n\n");
			break;
		}
	}
}

void createCoordinate() {
	for (int i = 0; i < 20; i++) {
		coor[i].fill = 0;
		newCoor[i].fill = 0;
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

		if (number > 19) printf("List�� ���� á���ϴ�.\n");
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
	int count = 20;

	for (int i = 0; i < 20; i++) {
		if (!coor[i].fill) {
			count = i;
			break;
		}
	}

	if (count != 20) {
		for (int i = count; i > -1; i--) {
			if (coor[i].fill) {
				coor[i+1] = coor[i];
				coor[i+1].number += 1;
			}
		}

		coor[0].x = x;
		coor[0].y = y;
		coor[0].z = z;
		coor[0].number = 0;
		coor[0].length = findLength(x, y, z);
		coor[0].fill = 1;
	}
	else {
		printf("List�� ���� á���ϴ�.\n");
	}
}

void deleteCoordinate_lower() {
	for (int i = 0; i < 20; i++) {
		if (coor[i].fill) {
			coor[i].fill = 0;
			break;
		}
	}
}

void deleteCooridnate_upper() {
	for (int i = 19; i >= 0; i--) {
		if (coor[i].fill) {
			coor[i].fill = 0;
			break;
		}
	}
}

void printCoordinate(COORDINATE* coo) {
	printf("\n\n|%6s | %3c %3c %3c | %5s|\n", "Number", 'X', 'Y', 'Z', "Length");
	for (int i = 9; i >= 0; i--) {
		if (coor[i].fill) {
			printf("|%6d | %3d %3d %3d | %6.2lf|\n", i, coo[i].x, coo[i].y, coo[i].z, coo[i].length);
		}
		else {
			printf("|%6d | %3c %3c %3c | %6c|\n", i, '-', '-', '-', '-');
		}
	}
}

void countCoordinate() {
	int count = 0;
	for (int i = 0; i < 19; i++) { if(coor[i].fill) count++; }

	printf("���� ����Ʈ�� ����� ���� ���� : %d", count);
}

void furthestCoordinate() {
	float longestLength = 0;

	for (int i = 0; i < 20; i++) {if (coor[i].fill && longestLength < coor[i].length) longestLength = coor[i].length;}
	for (int i = 0; i < 20; i++) { if (coor[i].fill && longestLength == coor[i].length) printf("�������κ��� ���� �ָ� �������ִ� ��ǥ : (%d, %d, %d) %lf\n", coor[i].x, coor[i].y, coor[i].z, coor[i].length); }
}

void closestCoordinate() {
	float shortestLength = coor[0].length;

	for (int i = 0; i < 20; i++) { 
		if (coor[i].fill && shortestLength > coor[i].length) shortestLength = coor[i].length;
	}
	for (int i = 0; i < 20; i++) { if (coor[i].fill && shortestLength == coor[i].length) printf("�������κ��� ���� �������ִ� ��ǥ : (%d, %d, %d) %lf\n", coor[i].x, coor[i].y, coor[i].z, coor[i].length); }
}

void uppertList() {
	float shortestLength = -1;
	int count = 0;
	COORDINATE list[20];

	for (int i = 0; i < 20; i++) {
		list[i] = coor[i];
	}

	for (int i = 0; i < 20; i++) {
		for (int k = 0; k < 20; k++) {
			if (coor[k].fill && coor[k].length > shortestLength) {
				shortestLength = coor[k].length;
				count = k;
			}
		}
		for (int j = 0; j < 20; j++) {
			if (coor[j].fill && shortestLength > coor[j].length) {
				count = coor[j].number;
				shortestLength = coor[j].length;
			}
		}
		newCoor[i] = coor[count];
		coor[count].fill = 0;
	}

	for (int i = 0; i < 20; i++) {
		coor[i] = newCoor[i];
		newCoor[i] = list[i];
	}
}

void lowerList() {
	float longestLength = 0;
	int count = 0;
	COORDINATE list[20];

	for (int i = 0; i < 20; i++) {
		list[i] = coor[i];
	}

	for (int i = 0; i < 20; i++) {
		for (int j = 0; j < 20; j++) {
			if (coor[j].fill && coor[j].length > longestLength) {
				count = coor[j].number;
				longestLength = coor[j].length;
			}
		}
		newCoor[i] = coor[count];
		coor[count].fill = 0;
		longestLength = 0;
	}

	for (int i = 0; i < 20; i++) {
		coor[i] = newCoor[i];
		newCoor[i] = list[i];
	}
}

float findLength(int x, int y, int z) {return (float)(sqrt((pow(x, 2) + pow(y, 2) + pow(z, 2))));}