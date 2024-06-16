#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "./headers/structs.h"
#include "./headers/recipe_utils.h"

// 검색 쿼리 구조체
typedef struct 
{
    char *text; 
    darray *tags;
    darray *ingres;
    char *code;
    int orderby;
} QUERY;

// utils
// 두 darray의 공통 원소의 개수 반환
int count_matches(darray *arr, darray *subarr);

// darray codes를 key에 따라 내림차순으로 정렬
void sort_codes(darray *codes, int *keys);

// public의 모든 레시피의 고유코드를 darray codes에 저장
void get_full_code(darray *codes); 

// codes에 저장된 레시피들의 좋아요 수를 int형 배열로 반환
int *get_likes(darray *codes);

// ingre_darray ingredients의 ingre들의 이름을 darray *ingre_names에 저장
void get_ingre_name(ingre_darray ingredients, darray *ingre_names);

// string을 입력받아 key를 기준으로 분할하여 darray *array에 저장
void split_string(darray *array, const char *string, char key); 


// input to query
// 입력된 텍스트 args를 받아 변환한 쿼리를 QUERY query에 저장
void split_args(QUERY *query, const char *args); 


// search
// full_codes에 저장된 레시피 중 제목에 text를 포함하는 레시피의 고유코드를 darray codes에 저장
void search_with_text(darray *codes, darray *full_codes, const char *text);

// full_codes에 저장된 레시피 중 tags의 원소 중 하나 이상을 포함하는 레시피의 고유코드를 darray codes에 저장
// 각 레시피에 포함된 tags의 원소의 개수를 int형 배열로 반환
int *search_with_tag(darray *codes, darray *full_codes, darray *tags); 

// full_codes에 저장된 레시피 중 ingres의 원소 중 하나 이상을 포함하는 레시피의 고유코드를 darray codes에 저장
// 각 레시피에 포함된 ingres의 원소의 개수를 int형 배열로 반환
int *search_with_ingredient(darray *codes, darray *full_codes, darray *ingres);


// console
// codes에 저장된 레시피를 30개씩 출력한다. u, d키를 이용해 커서를 이동하고 s키를 이용해 레시피를 선택할 수 있다.
void select_recipe(darray *codes);

// 텍스트 입력을 받아 검색 결과를 출력한다.
void search();

/*
함수 설명: 
ingre_darray ingredients의 ingre들의 이름을 darray *ingre_names에 저장합니다.

매개변수:
ingre_darray ingredients - 재료 구조체 ingre들의 동적배열
darray *ingre_names - 재료의 이름을 저장할 동적배열의 주소
*/
void get_ingre_name(ingre_darray ingredients, darray *ingre_names) 
{
    int i;
    for (i=0; i<ingredients.count; i++) 
    {
        addto_darray(ingre_names, ingredients.arr[i]->name);
    }
}

/*
함수 설명: 
두 darray의 공통 원소의 개수를 반환한다.

매개변수:
darray *arr - 첫번째 darray의 주소
darray *subarr - 두번째 darray의 주소

반환값:
arr의 원소와 subarr의 원소 중 일치하는 쌍의 개수
*/
int count_matches(darray *arr, darray *subarr) 
{
    int count = 0;
    int i, j;
    for (i=0; i<arr->count; i++) 
    {
        for (j=0; j<subarr->count; j++) 
        {
            if (strcmp(arr->arr[i], subarr->arr[j]) == 0) 
            {
                count++;
            }
        }
    }
    return count;
}

/*
함수 설명: 
고유코드를 key에 따라 내림차순으로 정렬한다.

매개변수:
darray *codes - 고유코드들의 darray의 주소
int *keys - 각 고유코드에 대응되는 가중치.
*/
void sort_codes(darray *codes, int *keys) 
{
    int i, j;
    char *temp;
    int temp_key;
    for (i=0; i<codes->count; i++) 
    {
        for (j=i+1; j<codes->count; j++) 
        {
            if (keys[j-1] < keys[j]) 
            {
                temp = codes->arr[j];
                codes->arr[j] = codes->arr[j-1];
                codes->arr[j-1] = temp;

                temp_key = keys[j];
                keys[j] = keys[j-1];
                keys[j-1] = temp_key;
            }
        }
    }
}

/*
함수 설명: 
string을 입력받아 key를 기준으로 분할하여 darray *array에 저장한다. 파이썬의 .split(key)와 유사하다.

매개변수:
darray *array - 분할한 문자열을 저장하는 darray의 주소
const char *string - 분할하는 문자열.
char key - 문자열을 분할하는 기준이 되는 문자.
*/
void split_string(darray *array, const char *string, char key) 
{
    int length = strlen(string);
    int i, j;
    int index = 0;
    char buffer[100] = "";
    int buffer_length;
    char *copy;

    for (i=0; i<length; i++) 
    {
        if (string[i] != key) 
        {
            buffer[index] = string[i];
            index++;
        }
        else 
        {
            copy = (char *)malloc(sizeof(char) * (strlen(buffer)+1));
            strcpy(copy, buffer);
            addto_darray(array, copy);
            index = 0;
            buffer_length = strlen(buffer);
            for(j=0; j<buffer_length; j++) {
                buffer[j] = '\0';
            }
        }
    }
    copy = (char *)malloc(sizeof(char) * (strlen(buffer)+1));
    strcpy(copy, buffer);
    addto_darray(array, copy);
}

/*
함수 설명: 
public의 모든 레시피의 고유코드를 darray codes에 저장한다.

매개변수:
darray *codes - public의 모든 레시피의 고유코드를 저장할 darray의 주소.
*/
void get_full_code(darray *codes) 
{
    FILE *file;
    init_darray(codes);
    file = fopen("../data/archive/public.txt", "r");
    char *temp;
    if (file) 
    {
        char line[MAX_LENGTH];
        while (fgets(line, MAX_LENGTH, file)) 
        {
            temp = strdup(trim_whitespace(line));
            addto_darray(codes, temp);
        }
    }
    else 
    {
        printf("Could not open public.txt\n");
    }
    fclose(file);
}

/*
함수 설명: 
codes에 저장된 레시피들의 좋아요 수를 int형 배열로 반환한다.

매개변수:
darray *codes - 레시피의 고유코드를 저장한 darray의 주소.

반환값:
레시피들의 좋아요 수를 담은 int형 배열의 첫번쨰 원소의 주소.
*/
int *get_likes(darray *codes) 
{
    int i;
    int *likes = (int *)malloc(sizeof(int) * codes->count);
    RECIPE r;
    RECIPE *recipe = &r;

    for (i=0; i<codes->count; i++) 
    {
        init_recipe(recipe);
        read_recipe_from_file(codes->arr[i], recipe);
        likes[i] = recipe->likes;
    }

    return likes;  
}


/*
함수 설명: 
입력된 텍스트 args를 받아 변환한 쿼리를 QUERY query에 저장한다.

args는 다음의 형식으로 입력된다.
text=[]/tag=[]&[]/code=[]/ingredient=[]&[]/option=[like/tag/ingredient]

예시는 다음과 같다.
text=hello/tag=egg/code=/ingredient=beef&milk/option=like
text=hello/tag=apple/code=/ingredient=milk/option=tag
text=/tag=egg&apple/code=/ingredient=beef&milk/option=ingredient
text=/tag=/code=10002/ingredient=/option=

매개변수:
QUERY *query - args의 정보를 저장할 QUERY의 주소
args - QUERY로 변환할 텍스트
*/
void split_args(QUERY *query, const char *args) 
{
    int i, j;
    char *copy;

    init_darray(query->tags);
    init_darray(query->ingres);

    darray option;
    darray *options = &option;
    init_darray(options);
    split_string(options, args, '/');

    for (i=0; i<(options->count); i++) 
    {
        if (options->arr[i][0] == 't') 
        {
            if (options->arr[i][1] == 'e') // text
            {
                if (strlen(options->arr[i]) > 5)
                {
                    copy = (char *)malloc(sizeof(char) * (strlen(options->arr[i]+5)+1));
                    strcpy(copy, options->arr[i]+5);
                    query->text = copy;
                }
                else 
                {
                    query->text = NULL;
                }
            }
            else // tag
            {
                if (strlen(options->arr[i]) > 4) {
                    split_string(query->tags, options->arr[i]+4, '&');
                    for (j=0; j<query->tags->count; j++) {
                        printf("%s ", query->tags->arr[j]);
                    }
                    printf("\n");
                }
                else 
                {
                    query->tags = NULL;
                }
            }
        }
        else if (options->arr[i][0] == 'i') // ingredient
        {
            if (strlen(options->arr[i]) > 11) 
            {
                split_string(query->ingres, options->arr[i]+11, '&');
            }
            else 
            {
                query->ingres = NULL;
            }
        }
        else if (options->arr[i][0] == 'c') // code
        {
            if (strlen(options->arr[i]) > 5) 
            {
                copy = (char *)malloc(sizeof(char) * (strlen(options->arr[i]+5)+1));
                strcpy(copy, options->arr[i]+5);
                query->code = copy;
            }
            else 
            {
                query->code = NULL;
            }
        }
        else // option
        {
            /*
            0 - like
            1 - tag
            2 - ingredient
            */
            query->orderby = 0;
            if (strlen(options->arr[i]) > 7) 
            {
                if (options->arr[i][7] == 't') // tag
                {
                    query->orderby = 1;
                }
                else if (options->arr[i][7] == 'i') // ingredient
                {
                    query->orderby = 2;
                }
            }
        }
    }
}

/*
함수 설명: 
full_codes에 저장된 레시피 중 제목에 text를 포함하는 레시피의 고유코드를 darray codes에 저장한다.

매개변수:
darray *codes - 검색된 레시피의 고유코드를 저장할 darray의 주소
darray *full_codes - 검색 대상이 되는 레시피의 고유코드를 저장한 darray의 주소
const char *text - 검색하고자 하는 제목 (또는 그 제목의 일부)
*/
void search_with_text(darray *codes, darray *full_codes, const char *text) 
{
    int i;
    RECIPE r;
    RECIPE *recipe = &r;
    init_recipe(recipe);

    for (i=0; i<full_codes->count; i++) 
    {
        read_recipe_from_file(full_codes->arr[i], recipe);
        if (strstr(recipe->title, text)) 
        {
            addto_darray(codes, recipe->code);
        }
    }
}

/*
함수 설명: 
full_codes에 저장된 레시피 중 tags의 원소 중 하나 이상을 포함하는 레시피의 고유코드를 darray codes에 저장한다.

매개변수:
darray *codes - 검색된 레시피의 고유코드를 저장할 darray의 주소
darray *full_codes - 검색 대상이 되는 레시피의 고유코드를 저장한 darray의 주소
darray *tags - 검색하고자 하는 태그를 저장한 darray의 주소

반환값:
각 레시피에 포함된 tags의 원소의 개수를 담은 int형 배열의 첫번째 원소의 주소
*/
int *search_with_tag(darray *codes, darray *full_codes, darray *tags) 
{
    int i;
    RECIPE r;
    RECIPE *recipe = &r;
    int match;
    int *matches = (int *)malloc(sizeof(int) * tags->count);
    int index = 0;
    
    for (i=0; i<full_codes->count; i++) 
    {
        init_recipe(recipe);
        read_recipe_from_file(full_codes->arr[i], recipe);
        
        match = count_matches(&(recipe->tag), tags);
        if (match > 0) 
        {
            matches[index] = match;
            index++;
            addto_darray(codes, recipe->code);
        } 
    }

    return matches;
}

/*
함수 설명: 
full_codes에 저장된 레시피 중 ingres의 원소 중 하나 이상을 포함하는 레시피의 고유코드를 darray codes에 저장한다.

매개변수:
darray *codes - 검색된 레시피의 고유코드를 저장할 darray의 주소
darray *full_codes - 검색 대상이 되는 레시피의 고유코드를 저장한 darray의 주소
darray *ingres - 검색하고자 하는 재료를 저장한 darray의 주소

반환값:
각 레시피에 포함된 ingres의 원소의 개수를 담은 int형 배열의 첫번째 원소의 주소
*/
int *search_with_ingredient(darray *codes, darray *full_codes, darray *ingres) 
{
    int i;
    RECIPE r;
    RECIPE *recipe = &r;
    darray name;
    darray *ingre_names = &name;
    int match;
    int *matches = (int *)malloc(sizeof(int) * ingres->count);
    int index = 0;
    
    for (i=0; i<full_codes->count; i++) 
    {
        init_recipe(recipe);
        read_recipe_from_file(full_codes->arr[i], recipe);

        init_darray(ingre_names);
        get_ingre_name(recipe->ingre_arr, ingre_names);

        match = count_matches(ingre_names, ingres);
        if (match > 0) 
        {
            matches[index] = match;
            index++;
            addto_darray(codes, recipe->code);
        } 
    }

    return matches;
}
 
/*
함수 설명: 
codes에 저장된 레시피를의 고유코드와 제목을 30개씩 출력한다. u, d키를 이용해 커서를 이동하고 s키를 이용해 레시피를 선택할 수 있다.

매개변수:
darray *codes - 출력할 레시피의 고유코드를 저장한 darray의 주소
*/
void select_recipe(darray *codes) 
{
    int i;
    char input;
    int cursor = 0;
    int pageindex = 0;
    int len = codes->count;
    int start = 0;
    int end = 30;
    RECIPE r;
    RECIPE *recipe = &r;
    init_recipe(recipe);
    
    while(1) 
    {
        refresh_print("[q]uit, [u]p, [d]own, [s]elect\n");
        start = pageindex * 30;
        if (start + 30 < len) 
        {
            end = start +30;
        }
        else 
        {
            end = len;
        }

        if (len == 0) 
        {
            printf("no recipes searched\n");
            printf("press any key to continue\n");
            input = getch();
            break;
        }

        for (i=start; i<end; i++) 
        {
            init_recipe(recipe);
            read_recipe_from_file(codes->arr[i], recipe);

            if (i == cursor) 
            {
                printf("> ");
            }
            printf("%s %s\n", codes->arr[i], recipe->title);
        }
        input = getch();

        if (input == 'd') 
        {
            if (cursor < len -1) 
            {
                cursor++;
            }
        }
        if (input == 'u') 
        {
            if (cursor > 0) 
            {
                cursor--;
            }
        }
        if (input == 'q') 
        {
            break;
        }
        if (input == 's') 
        {
            init_recipe(recipe);
            read_recipe_from_file(codes->arr[cursor], recipe);
            display_recipe_public(recipe);
        }
        pageindex = cursor / 30;
    }
}

// target으로 original 복사
void copy_darray(darray *target, darray *original) {
    init_darray(target);
    int i;

    for (i=0; i<original->count; i++) 
    {
        addto_darray(target, original->arr[i]);
    }
}

/*
함수 설명: 
텍스트 입력을 받아 검색 결과를 출력한다. q를 입력하면 홈 화면으로 복귀한다.
*/
void search() {
    getchar();

    while(1) 
    {
        char buffer[300] = "";
        QUERY q;
        QUERY *query = &q;

        darray tag;
        query->tags = &tag;

        darray ingre;
        query->ingres = &ingre;

        darray code;
        darray *codes = &code;

        darray full_code;
        darray *full_codes = &full_code;

        int *likes = NULL;
        int *tag_matches = NULL;
        int *ingredient_matches = NULL;
    
        init_darray(codes);
        refresh_print("search query should be written in the format:\n");
        printf("text=[]/tag=[]&[]/code=[]/ingredient=[]&[]/option=[like/tag/ingredient].\n\n");
        printf("---example---\n");
        printf("text=hello/tag=egg/code=/ingredient=beef&milk/option=like\n\n");
        printf("enter q to quit.\n\n");
        printf("enter your search query: ");
        fgets(buffer, sizeof(buffer), stdin);

        if (strcmp(buffer, "q\n") == 0) 
        {
            break;
        }
        split_args(query, buffer);
        get_full_code(full_codes);
        
        if (query->code) 
        {
            addto_darray(codes, query->code);
            select_recipe(codes);
            continue;
        }

        if (query->text) 
        {
            search_with_text(codes, full_codes, query->text);
            copy_darray(full_codes, codes);
        }        

        if (query->tags) 
        {
            init_darray(codes);
            tag_matches = search_with_tag(codes, full_codes, query->tags);
            copy_darray(full_codes, codes);

            if (query->ingres) 
            {
                init_darray(codes);
                ingredient_matches = search_with_ingredient(codes, full_codes, query->ingres);
                full_codes = codes;
                tag_matches = search_with_tag(codes, full_codes, query->tags);
            }
        }

        else if (query->ingres) 
        {
            init_darray(codes);
            ingredient_matches = search_with_ingredient(codes, full_codes, query->ingres);
            copy_darray(full_codes, codes);
        }

        if (query->orderby == 0) 
        {
            likes = get_likes(codes);
            sort_codes(codes, likes);
        }
        
        else if (query->orderby == 1) 
        {
            sort_codes(codes, tag_matches);
        }
        else 
        {
            sort_codes(codes, ingredient_matches);
        }

        select_recipe(codes);       
    }
}