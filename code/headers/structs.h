#ifndef STRUCTS_H
#define STRUCTS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char **arr; 
    int count;
} darray;

typedef struct {
    float **arr; 
    int count;
} f_darray;

struct ingre { // 재료 구조체 // 재료 구조체의 경우 모든 속성이 동적할당 
    char *name;
    f_darray nutrient;
    float *quantity;
    char *unit;
};

typedef struct {
    struct ingre **arr;
    int count; 
} ingre_darray;

typedef struct { // 레시피 구조체 // 레시피 구조체의 경우 darray 꼴 속성만 동적할당
    char *title;
    char *username;
    darray tag; 
    ingre_darray ingre_arr;
    darray text;
    int likes;
    darray comments; 
    darray refer;
    darray refered; 
    char *code;
} RECIPE;

// 동적 배열 초기화 함수
// 동적 배열에 항목 추가 함수
// 동적 배열 해제 함수

// darray
void init_darray(darray *array);
void addto_darray(darray *array, char *item);
void free_darray(darray *array);

//f_darray
void init_f_darray(f_darray *array);
void addto_f_darray(f_darray *array, float *item);
void free_f_darray(f_darray *array);

// ingre 구초체 메모리 해제 함수
void free_ingre(struct ingre *ingre);

//ingre_darray
void init_ingre_darray(ingre_darray *array);
void addto_ingre_darray(ingre_darray *array, struct ingre *item);
void free_ingre_darray(ingre_darray *array);

// 동적 배열 초기화 및 데이터 할당을 위한 Recipe 초기화 함수
void init_recipe(RECIPE *recipe);

// Recipe 구조체 메모리 해제 함수
void free_recipe(RECIPE *recipe);

// 재료 이름으로부터 영양소 f_darray로 반환하는 함수 
f_darray return_nutrient(char* name);

#endif