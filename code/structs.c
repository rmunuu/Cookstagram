// ingre & Recipe 구조체 및 동적 배열 구조체 및 함수 정의 파일 

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

/*
// 테스트용 메인 함수
int main() {
    char *ingredient_name = "pork";  // 테스트할 재료 이름
    f_darray nutrients = return_nutrient(ingredient_name);

    // 결과 출력
    printf("Nutrients for %s:\n", ingredient_name);
    for (int i = 0; i < nutrients.count; i++) {
        printf("%f ", nutrients.arr[i][0]);
    }
    printf("\n");

    free_f_darray(&nutrients);

    return 0;
}
*/


// darray

// 동적 배열 초기화 함수
void init_darray(darray *array) {
    array->arr = NULL;
    array->count = 0;
} 

// 동적 배열에 항목 추가 함수
void addto_darray(darray *array, char *item) {
    array->count++;
    array->arr = realloc(array->arr, array->count * sizeof(char *));
    array->arr[array->count - 1] = item;
}

// 동적 배열 해제 함수
void free_darray(darray *array) {
    for (int i = 0; i < array->count; i++) {
        free(array->arr[i]);
    }
    free(array->arr);
    array->arr = NULL;
    array->count = 0;
} 

//f_darray

// 동적 배열 초기화 함수
void init_f_darray(f_darray *array) {
    array->arr = NULL;
    array->count = 0;
} 

// 동적 배열에 항목 추가 함수
void addto_f_darray(f_darray *array, float *item) {
    array->count++;
    array->arr = realloc(array->arr, array->count * sizeof(float *));
    array->arr[array->count - 1] = item;
} 

// 동적 배열 해제 함수
void free_f_darray(f_darray *array) {
    for (int i = 0; i < array->count; i++) {
        free(array->arr[i]);
    }
    free(array->arr);
    array->arr = NULL;
    array->count = 0;
}  

// ingre 구초체 메모리 해제 함수  
void free_ingre(struct ingre *ingre) {
    free(ingre->name);
    free_f_darray(&(ingre->nutrient));
    free(ingre->quantity);
    free(ingre->unit);
    ingre->name = NULL;
    ingre->quantity = NULL;
    ingre->unit = NULL;
}

//ingre_darray

// 동적 배열 초기화 함수
void init_ingre_darray(ingre_darray *array) {
    array->arr = NULL;
    array->count = 0;
} 

// 동적 배열에 항목 추가 함수
void addto_ingre_darray(ingre_darray *array, struct ingre *item) {
    array->count++;
    array->arr = realloc(array->arr, array->count * sizeof(struct ingre *));
    array->arr[array->count - 1] = item;
} 

// 동적 배열 해제 함수
void free_ingre_darray(ingre_darray *array) {
    for (int i = 0; i < array->count; i++) {
        free_ingre(array->arr[i]);
    }
    free(array->arr);
    array->arr = NULL;
    array->count = 0;
}  

// 동적 배열 초기화 및 데이터 할당을 위한 Recipe 초기화 함수
void init_recipe(RECIPE *recipe) {
    init_darray(&recipe->tag);
    init_ingre_darray(&recipe->ingre_arr);
    init_darray(&recipe->text);
    init_darray(&recipe->comments);
    init_darray(&recipe->refer);
    init_darray(&recipe->refered);
    recipe->title = NULL;
    recipe->username = NULL;
    recipe->code = NULL;
}

// Recipe 구조체 메모리 해제 함수
void free_recipe(RECIPE *recipe) {
    free_darray(&recipe->tag);
    free_ingre_darray(&recipe->ingre_arr);
    free_darray(&recipe->text);
    free_darray(&recipe->comments);
    free_darray(&recipe->refer);
    free_darray(&recipe->refered);
} 

// 재료 이름으로부터 영양소 반환하는 함수 
f_darray return_nutrient(char* name) {
    const int MAX_LINE_LENGTH = 256; 
    const int NUM_NUTRIENTS = 7;
    FILE *file = fopen("../data/nutrient_data.csv", "r");
    f_darray result;
    result.arr = NULL;
    result.count = NUM_NUTRIENTS;

    if (!file) {
        printf("Could not open file\n");
        return result;
    }

    char line[MAX_LINE_LENGTH];
    int found = 0;
    result.arr = (float **)malloc(NUM_NUTRIENTS * sizeof(float *));
    for (int i = 0; i < NUM_NUTRIENTS; i++) {
        result.arr[i] = (float *)malloc(sizeof(float));
        result.arr[i][0] = 0.0;
    }

    // 첫 번째 라인은 헤더
    if (fgets(line, MAX_LINE_LENGTH, file) == NULL) {
        fclose(file);
        return result;
    }

    // 각 줄을 읽으면서 처리
    while (fgets(line, MAX_LINE_LENGTH, file)) {
        char *temp_line = strdup(line);
        char *pos = strchr(temp_line, ',');
        if (pos != NULL) {
            *pos = '\0';
            if (strcmp(temp_line, name) == 0) {
                found = 1;
                temp_line = pos + 1;
                for (int i = 0; i < NUM_NUTRIENTS; i++) {
                    pos = strchr(temp_line, ',');
                    if (pos != NULL) {
                        *pos = '\0';
                        result.arr[i][0] = atof(temp_line) / 100.0;  // 값을 100으로 나누어 저장
                        temp_line = pos + 1;
                    } else {
                        result.arr[i][0] = atof(temp_line) / 100.0;  // 마지막 값 처리
                        break;
                    }
                }
                break;
            }
        }
    }

    fclose(file);
    
    if (!found) {
        for (int i = 0; i < NUM_NUTRIENTS; i++) {
            result.arr[i][0] = 0.0;
        }
    }

    return result;
}



