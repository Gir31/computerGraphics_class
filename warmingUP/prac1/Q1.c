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

int main() {
	createMatrix();
	printMatrix();
	oddPrint();
	evenPrint();
	return 0;
}

void createMatrix() {
	int count_two = 0, count_two2 = 0;
	srand(time(NULL));

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			matrix1[i][j] = rand() % 3;
			if (matrix1[i][j] == 2 && count_two < 2) count_two++;
			else if (matrix1[i][j] == 2 && count_two == 2) j--;

			matrix2[i][j] = rand() % 3;
			if (matrix2[i][j] == 2 && count_two2 < 2) count_two2++;
			else if (matrix2[i][j] == 2 && count_two2 == 2) j--;
		}
	}
	if (count_two != 2) matrix1[3][3] = 2;
	if (count_two2 != 2) matrix2[3][3] = 2;
}

void multipleMatrix() {
	int matrix_ex[4][4] = { 0 };
	for (int k = 0; k < 4; k++) {
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				matrix_ex[k][i] += matrix1[k][j] * matrix2[j][i];
			}
			printf("%4d", matrix_ex[k][i]);
		}
		printf("\n");
	}
}

void addMatrix() {
	int matrix_ex[4][4];
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			matrix_ex[i][j] = matrix1[i][j] + matrix2[i][j];
			printf("%4d", matrix_ex[i][j]);
		}
		printf("\n");
	}
}

void minusMatrix() {
	int matrix_ex[4][4];
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			matrix_ex[i][j] = matrix1[i][j] - matrix2[i][j];
			printf("%4d", matrix_ex[i][j]);
		}
		printf("\n");
	}
}

void printMatrix() {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 2; j++) {
			printf("|");
			for (int k = 0; k < 4; k++) {
				if (j == 0) {
					printf("%4d", matrix1[i][k]);
				}
				else {
					printf("%4d", matrix2[i][k]);
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
}

void evenPrint() {
	char dot = '.';
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 2; j++) {
			printf("|");
			for (int k = 0; k < 4; k++) {
				if (j == 0) {
					if (!(matrix1[i][k] % 2))
						printf("%4d", matrix1[i][k]);
					else
						printf("%4c", dot);
				}
				else {
					if (!(matrix2[i][k] % 2))
						printf("%4d", matrix2[i][k]);
					else
						printf("%4c", dot);
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
						printf("%4d", matrix1[i][k]);
					else
						printf("%4c", dot);
				}
				else {
					if (matrix2[i][k] % 2)
						printf("%4d", matrix2[i][k]);
					else
						printf("%4c", dot);
				}
			}
			printf("|");
		}
		printf("\n");
	}
}