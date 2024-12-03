#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_LINE_LENGTH 256

typedef struct {
    float x, y, z;
} Vertex;

typedef struct {
    unsigned int v1, v2, v3;
} Face;

typedef struct {
    float x, y, z;
} Normal;

typedef struct {
    Vertex* vertices;
    size_t vertex_count;
    Normal* normals;
    size_t normal_count;
    Face* faces;
    size_t face_count;
} Model;

void read_newline(char* str) {
    char* pos;
    if ((pos = strchr(str, '\n')) != NULL)
        *pos = '\0';
}

void read_obj_file(const char* filename, Model* model) {
    FILE* file;
    fopen_s(&file, filename, "r");
    if (!file) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    char line[MAX_LINE_LENGTH];
    model->vertex_count = 0;
    model->normal_count = 0;
    model->face_count = 0;

    // 첫 번째 패스: 요소 개수 확인
    while (fgets(line, sizeof(line), file)) {
        read_newline(line);
        if (line[0] == 'v' && line[1] == ' ') {
            model->vertex_count++;
        }
        else if (line[0] == 'v' && line[1] == 'n') {
            model->normal_count++;
        }
        else if (line[0] == 'f' && line[1] == ' ') {
            model->face_count++;
        }
    }

    // 메모리 할당
    fseek(file, 0, SEEK_SET);
    model->vertices = (Vertex*)malloc(model->vertex_count * sizeof(Vertex));
    model->normals = (Normal*)malloc(model->normal_count * sizeof(Normal));
    model->faces = (Face*)malloc(model->face_count * sizeof(Face));

    size_t vertex_index = 0;
    size_t normal_index = 0;
    size_t face_index = 0;

    // 두 번째 패스: 데이터 읽기
    while (fgets(line, sizeof(line), file)) {
        read_newline(line);
        if (line[0] == 'v' && line[1] == ' ') {
            sscanf_s(line + 2, "%f %f %f",
                &model->vertices[vertex_index].x,
                &model->vertices[vertex_index].y,
                &model->vertices[vertex_index].z);
            vertex_index++;
        }
        else if (line[0] == 'v' && line[1] == 'n') {
            sscanf_s(line + 3, "%f %f %f",
                &model->normals[normal_index].x,
                &model->normals[normal_index].y,
                &model->normals[normal_index].z);
            normal_index++;
        }
        else if (line[0] == 'f' && line[1] == ' ') {
            unsigned int v1, v2, v3;
            sscanf_s(line + 2, "%u//%*u %u//%*u %u//%*u", &v1, &v2, &v3);
            model->faces[face_index].v1 = v1 - 1; // OBJ 인덱스는 1부터 시작
            model->faces[face_index].v2 = v2 - 1;
            model->faces[face_index].v3 = v3 - 1;
            face_index++;
        }
    }

    fclose(file);
}