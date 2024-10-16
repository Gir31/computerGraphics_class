#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <time.h>
#include <math.h>

int matrix1[4][4], matrix2[4][4];
void createMatrix();
void multipleMatrix();
void addMatrix();
void minusMatrix();
void printMatrix();
void transposedMatrix();
void evenPrint();
void oddPrint();
void scalarMatrix(int scalar);

int determinant_2x2(int(*matrix)[2]);
int determinant_3x3(int(*matrix)[3]);
int determinant_4x4(int (*matrix)[4]);


int main() {
	char command;
	int changeCount = 0;

	printf("Q1. ��� �ٷ��\n\n");
	printf("--------------------------------------------\n");
	createMatrix();

	while (1) {
		printf("\nm : ����\na : ����\nd : ����\nr : ��Ľ��� ��\nt : ��ġ\ne : ¦��/Ȧ��\ns : ���� ����\n1~9 : ��Į���\nq : ����\n");
		printf("���ɾ� �Է� : ");
		scanf("%c", &command);
		printf("\n");
		switch (command) {
		case 'm' : multipleMatrix(); break;
		case 'a': addMatrix();  break;
		case 'd': minusMatrix(); break;
		case 'r': 
			printMatrix();
			printf("\n�� ����� ��Ľ�\nMATRIX 1 : %d / MATRIX 2 : %d\n", determinant_4x4(matrix1), determinant_4x4(matrix2));
			break;
		case 't': transposedMatrix(); break;
		case 'e':
			if (changeCount == 0) {
				oddPrint();
				changeCount++;
			}
			else if (changeCount == 1) {
				evenPrint();
				changeCount++;
			}
			else {
				printMatrix();
				changeCount = 0;
			}

			break;
		case 's': createMatrix();  break;
		case 'q': return 0;
		default : 
			if (command - 48 > 0 && command - 48 < 10)
				scalarMatrix(command - 48);
			break;
		}
		fseek(stdin, 0, SEEK_END);
		printf("--------------------------------------------\n");
	}
}

void createMatrix() {
	int count_two = 0, count_two2 = 0;
	srand(time(NULL));

	while (count_two != 2) {
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				matrix1[i][j] = rand() % 3;
				if (matrix1[i][j] == 2 && count_two < 2) count_two++;
				else if (matrix1[i][j] == 2 && count_two == 2) j--;
			}
		}
		if (count_two != 2) count_two = 0;
	}
	while (count_two2 != 2) {
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				matrix2[i][j] = rand() % 3;
				if (matrix2[i][j] == 2 && count_two2 < 2) count_two2++;
				else if (matrix2[i][j] == 2 && count_two2 == 2) j--;
			}
		}
		if (count_two2 != 2) count_two2 = 0;
	}

	printMatrix();
}

void multipleMatrix() {
	int matrix_ex[4][4] = { 0 };
	for (int k = 0; k < 4; k++) {
		printf("|");
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				matrix_ex[k][i] += matrix1[k][j] * matrix2[j][i];
			}
			printf("%5d", matrix_ex[k][i]);
		}
		printf("|\n");
	}
}

void addMatrix() {
	int matrix_ex[4][4];
	for (int i = 0; i < 4; i++) {
		printf("|");
		for (int j = 0; j < 4; j++) {
			matrix_ex[i][j] = matrix1[i][j] + matrix2[i][j];
			printf("%5d", matrix_ex[i][j]);
		}
		printf("|\n");
	}
}

void minusMatrix() {
	int matrix_ex[4][4];
	for (int i = 0; i < 4; i++) {
		printf("|");
		for (int j = 0; j < 4; j++) {
			matrix_ex[i][j] = matrix1[i][j] - matrix2[i][j];
			printf("%5d", matrix_ex[i][j]);
		}
		printf("|\n");
	}
}

void printMatrix() {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 2; j++) {
			printf("|");
			for (int k = 0; k < 4; k++) {
				if (j == 0) {
					printf("%5d", matrix1[i][k]);
				}
				else {
					printf("%5d", matrix2[i][k]);
				}
			}
			printf("|");
		}
		printf("\n");
	}
}

void transposedMatrix() {
	int item = 0;

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (i < j) {
				item = matrix1[i][j];
				matrix1[i][j] = matrix1[j][i];
				matrix1[j][i] = item;

				item = matrix2[i][j];
				matrix2[i][j] = matrix2[j][i];
				matrix2[j][i] = item;
			}
		}
	}

	printMatrix();
}

void evenPrint() {
	char dot = '.';
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 2; j++) {
			printf("|");
			for (int k = 0; k < 4; k++) {
				if (j == 0) {
					if (!(matrix1[i][k] % 2))
						printf("%5d", matrix1[i][k]);
					else
						printf("%5c", dot);
				}
				else {
					if (!(matrix2[i][k] % 2))
						printf("%5d", matrix2[i][k]);
					else
						printf("%5c", dot);
				}
			}
			printf("|");
		}
		printf("\n");
	}
}

void oddPrint() {
	char dot = '.';
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 2; j++) {
			printf("|");
			for (int k = 0; k < 4; k++) {
				if (j == 0) {
					if (matrix1[i][k] % 2)
						printf("%5d", matrix1[i][k]);
					else
						printf("%5c", dot);
				}
				else {
					if (matrix2[i][k] % 2)
						printf("%5d", matrix2[i][k]);
					else
						printf("%5c", dot);
				}
			}
			printf("|");
		}
		printf("\n");
	}
}

void scalarMatrix(int scalar) {
	
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			matrix1[i][j] *= scalar;
			matrix2[i][j] *= scalar;
		}
	}
	
	printMatrix();
}

int determinant_2x2(int(*matrix)[2]) {
	return ((matrix[0][0] * matrix[1][1]) - (matrix[0][1] * matrix[1][0]));
}

int determinant_3x3(int(* matrix)[3]) {
	int x = 0, y = 0;
	int matrix_2x2[2][2];
	int sum = 0;

	for (int i = 0; i < 3; i++) {
		for (int j = 1; j < 3; j++) {
			for (int k = 0; k < 3; k++) {
				if (k != i) {
					if(i == 0) matrix_2x2[j - 1][k - 1] = matrix[j][k];
					else if(k > i) matrix_2x2[j - 1][k - i] = matrix[j][k];
					else  matrix_2x2[j - 1][k] = matrix[j][k];
				}
			}
		}

		if (i % 2) {
			sum += matrix[0][i] * -1 * determinant_2x2(matrix_2x2);
		}
		else {
			sum += matrix[0][i] * determinant_2x2(matrix_2x2);
		}
	}

	return sum;
}

int determinant_4x4(int (*matrix)[4]) {
	int x = 0, y = 0;
	int matrix_3x3[3][3];
	int sum = 0;

	for (int i = 0; i < 4; i++) {
		for (int j = 1; j < 4; j++) {
			for (int k = 0; k < 4; k++) {
				if (k != i) {
					if(i == 0) matrix_3x3[j - 1][k - 1] = matrix[j][k];
					else if (k > i) matrix_3x3[j - 1][k - i] = matrix[j][k];
					else  matrix_3x3[j - 1][k] = matrix[j][k];
				}
			}
		}

		if (i % 2) {
			sum += matrix[0][i] * -1 * determinant_3x3(matrix_3x3);
		}
		else {
			sum += matrix[0][i] * determinant_3x3(matrix_3x3);
		}
	}

	return sum;
}