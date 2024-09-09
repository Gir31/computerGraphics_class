#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <windows.h>

#define MAX_LINES 10
#define MAX_LINE_LEN 256

char lines[MAX_LINES][MAX_LINE_LEN];
char original_lines[MAX_LINES][MAX_LINE_LEN];

void stringMenu();

void readFile(char* filename);
void swapOrigin();
void printWords();
void findUppercaseWords();
void reverseLine();
void insertSign();
void reverseWords();
void replaceCharacter(char oldChar, char newChar);
void printWordCount();
void sortLinesWordCount(int command);
void findWord(char* word);

int main() {
    stringMenu();
}

void stringMenu() {
    printf("Q2. 파일에서 문자열 읽기\n\n");

    char command;
    char fileName[MAX_LINE_LEN];
    char oldChar, newChar;
    char searchWord[20];
    int c = 1, d = 1, e = 1, f = 1, g = 1, r = 1, s = 1;

    printf("데이터 파일 이름을 입력하시오. : ");
    scanf("%s", fileName);
    fseek(stdin, 0, SEEK_END);

    readFile(fileName);

    while (1) {
        printf("\nc: (대문자 찾기) d : (문장 거꾸로 출력) e : (@문자 삽입)\nf : (단어 거꾸로 출력) g : (문자 바꾸기) h : (단어 개수)\nr : (오름차순 / 내림차순) s : (단어 찾기) q : (프로그램종료)\n");
        printf("명령어 입력 : ");
        scanf("%c", &command);
        fseek(stdin, 0, SEEK_END);
        printf("\n");
        switch (command) {
        case 'c':
            if (c) {
                swapOrigin();
                findUppercaseWords();
                c = 0;
            }
            else {
                swapOrigin();
                printWords();
                c = 1;
            }
            break;
        case 'd':
            if (d) {
                swapOrigin();
                reverseLine();
                d = 0;
            }
            else {
                swapOrigin();
                printWords();
                d = 1;
            }
            break;
        case 'e':
            if (e) {
                swapOrigin();
                insertSign();
                e = 0;
            }
            else {
                swapOrigin();
                printWords();
                e = 1;
            }
            break;
        case 'f':
            if (f) {
                swapOrigin();
                reverseWords();
                f = 0;
            }
            else {
                swapOrigin();
                printWords();
                f = 1;
            }
            break;
        case 'g':
            if (g) {
                swapOrigin();
                printf("바꿀 문자를 입력하시오 (바꿀 문자, 새롭게 입력받을 문자) : ");
                scanf("%c %c", &oldChar, &newChar);
                fseek(stdin, 0, SEEK_END);
                replaceCharacter(oldChar, newChar);
                g = 0;
            }
            else {
                swapOrigin();
                printWords();
                g = 1;
            }
            break;
        case 'h':
            swapOrigin();
            printWordCount();
            break;
        case 'r':
            switch (r) {
            case 1: 
                swapOrigin();
                sortLinesWordCount(r);
                r = 2;
                break;
            case 2:
                swapOrigin();
                sortLinesWordCount(r);
                r = 0;
                break;
            default:
                swapOrigin();
                printWords();
                r = 1;
                break;
            }
            break;
        case 's':
            swapOrigin();
            printf("찾을 단어를 입력하시오 : ");
            scanf("%s", searchWord);
            fseek(stdin, 0, SEEK_END);
            findWord(searchWord);
            break;
        case 'q': return 0;
        default:
            printf("잘못된 명령어!\n\n");
            break;
        }
        printf("\n\n");
    }
}

void readFile(char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("파일을 열 수 없습니다.\n");
        exit(1);
    }

    for (int i = 0; i < MAX_LINES; i++) {
        if (fgets(lines[i], MAX_LINE_LEN, file) == NULL) break;
        strcpy(original_lines[i], lines[i]); // 원본도 저장
    }

    fclose(file);
}

void swapOrigin() {
    for (int i = 0; i < MAX_LINES; i++) {
        strcpy(lines[i], original_lines[i]);
    }
}

void printWords() {
    for (int i = 0; i < MAX_LINES; i++) {
        char* token = strtok(lines[i], " \t\n"); //" \t\n" 대로 문자열을 나눠주는 함수 strtok()
        while (token != NULL) {
            printf("%s ", token);
            token = strtok(NULL, " \t\n");
        }
        printf("\n");
        strcpy(lines[i], original_lines[i]);
    }
}

void findUppercaseWords() {
    int count = 0;
    for (int i = 0; i < MAX_LINES; i++) {
        char* token = strtok(lines[i], " \t\n");
        while (token != NULL) {
            if (isupper(token[0])) { //대문자를 찾아주는 함수 isupper()
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 3);
                printf("%s ", token);
                count++;
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
            }
            else {
                printf("%s ", token);
            }
            token = strtok(NULL, " \t\n");
        }
        printf("\n");
    }
    printf("대문자로 시작하는 단어의 개수: %d\n", count);
}

void reverseLine() {
    for (int i = 0; i < MAX_LINES; i++) {
        for (int j = strlen(lines[i]) - 1; j >= 0; j--) {
            printf("%c", lines[i][j]);
        }
    }
    printf("\n");
}

void insertSign() {
    for (int i = 0; i < MAX_LINES; i++) {
        for (int j = 0; j < strlen(lines[i]); j++) {
            printf("%c", lines[i][j]);
            if ((j + 1) % 3 == 0) {
                printf("@@");
            }
        }
    }
    printf("\n");
}

void reverseWords() {
    for (int i = 0; i < MAX_LINES; i++) {
        char* token = strtok(lines[i], " \t\n");

        while (token != NULL) {
            for (int j = strlen(token); j >= 0; j--) printf("%c", token[j]);
            printf(" ");
            token = strtok(NULL, " \t\n");
        }
        printf("\n");
    }
}

void replaceCharacter(char oldChar, char newChar) {
    for (int i = 0; i < MAX_LINES; i++) {
        for (int j = 0; j < strlen(lines[i]); j++) {
            if (lines[i][j] == oldChar) {
                lines[i][j] = newChar;
            }
        }
        printf("%s", lines[i]);
    }
}

void printWordCount() {
    for (int i = 0; i < MAX_LINES; i++) {
        int count = 0;
        char* token = strtok(lines[i], " \t\n");
        while (token != NULL) {
            count++;
            token = strtok(NULL, " \t\n");
        }
        printf("%3d줄 : % 3d개 단어\n", i + 1, count);

        strcpy(lines[i], original_lines[i]);
    }
}

void sortLinesWordCount(int command) {
    int wordCountList[MAX_LINES];
    int count = 0;

    for (int i = 0; i < MAX_LINES; i++) {
        int wordCount = 0;
        char* token = strtok(lines[i], " \t\n");
        while (token != NULL) {
            wordCount++;
            token = strtok(NULL, " \t\n");
        }
        wordCountList[i] = wordCount;
        strcpy(lines[i], original_lines[i]);
    }

    switch (command) {
    case 1: // 오름차순
        while (count != 9) {
        count = 0;
        for (int i = 0; i < MAX_LINES - 1; i++) {
            if (wordCountList[i] > wordCountList[i + 1]) {
                int tempCount = wordCountList[i];
                wordCountList[i] = wordCountList[i + 1];
                wordCountList[i + 1] = tempCount;

                char tempString[MAX_LINE_LEN];
                strcpy(tempString, lines[i]);
                strcpy(lines[i], lines[i + 1]);
                strcpy(lines[i + 1], tempString);
            }
            else count++;
        }
    }
        break;
    case 2: //내림차순
        while (count != 9) {
            count = 0;
            for (int i = 0; i < MAX_LINES - 1; i++) {
                if (wordCountList[i] < wordCountList[i + 1]) {
                    int tempCount = wordCountList[i];
                    wordCountList[i] = wordCountList[i + 1];
                    wordCountList[i + 1] = tempCount;

                    char tempString[MAX_LINE_LEN];
                    strcpy(tempString, lines[i]);
                    strcpy(lines[i], lines[i + 1]);
                    strcpy(lines[i + 1], tempString);
                }
                else count++;
            }
        }
        break;
    default: break;
    }

    for (int i = 0; i < MAX_LINES; i++) {
        printf("%4d : %s",wordCountList[i], lines[i]);
    }
}

void findWord(char* word) {
    int count = 0;

    for (int i = 0; i < MAX_LINES; i++) {
        char* token = strtok(lines[i], " \t\n");
        while (token != NULL) {
            if (!strcmp(token, word)) {
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 3);
                printf("%s ", token);
                count++;
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
            }
            else if (strlen(word) == strlen(token)) {
                int rightCount = 0;
                for (int i = 0; i < strlen(word); i++) {
                    if (word[i] == token[i]) rightCount++;
                    else if (64 < word[i] && word[i] < 91) {
                        if (word[i] + 32 == token[i]) rightCount++;
                        else break;
                    }
                    else if (97 < word[i] && word[i] < 123) {
                        if (word[i] - 32 == token[i]) rightCount++;
                        else break;
                    }
                }

                if (rightCount == strlen(word)) {
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 3);
                    printf("%s ", token);
                    count++;
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
                }
                else {
                    printf("%s ", token);
                }
            }
            else {
                printf("%s ", token);
            }
            token = strtok(NULL, " \t\n");
        }
        printf("\n");
    }
    printf("단어 '%s'의 개수: %d\n", word, count);
}