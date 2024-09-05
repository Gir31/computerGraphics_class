#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

typedef struct word {
	char length[15];
	struct word next;
	struct word prev;
}Word;

int main() {
	FILE* p_file;
	char fileName[20] = { 0 };
	char str[10000] = {0};

	printf("input data file name : ");
	scanf("%s", fileName);
	fseek(stdin, 0, SEEK_END);
	
	p_file = fopen(fileName,"r");

	if (NULL != p_file) {
		fread(str, 1, 999, p_file);
		printf("%s", str);
	}
	else {
		printf("fail");
	}

	fclose(p_file);

	return 0;
}

void newWord(Word* prevWord) {
	Word* new;

	new = (Word*)malloc(sizeof(Word));


}