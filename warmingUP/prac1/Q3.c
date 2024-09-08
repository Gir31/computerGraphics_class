#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct coordinate {
	int x, y, z;
	float length;
	struct coordinate* next;
}COORDINATE;

COORDINATE* head = NULL;

float findLength(int x, int y, int z);

int main() {

	printf("%lf", findLength(2,3,1));

	return 0;
}

float findLength(int x, int y, int z) {
	return (sqrt((pow(x, 2) + pow(y, 2) + pow(z, 2))));
}