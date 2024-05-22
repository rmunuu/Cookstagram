#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h> 
#include <conio.h> 
#include <ctype.h>
#include <dirent.h>
#include "./headers/structs.h"
#include "./headers/basic_funcs.h"
#include "./headers/global_vars.h"

#define MAX_LENGTH 2048
#define MAX_LINES 30 

// 고유코드 및 메모와 id를 받아서 그 id를 가지는 파일의 마지막에 레시피의 고유코드와 메모를 저장하는 코드 
// 코드 추가 함수
void save_memo(const char *code, const char *memo);

// CSV 파일 내에 재료 이름이 존재하는지 확인하는 함수, 있으면 1 ,없으면 0 반환 
// 입력 값은 재료명
int is_ingredient_in_csv(const char *name);

// 고유 코드 파일 존재 여부 확인 함수 , 있으면 1, 없으면 0 반환
// 일단은 private / public 모든 파일 가능하게 함 
// 입력 값은 코드명
int check_unique_code_exists(const char *code);

// 텍스트 파일에서 Recipe 구조체로 데이터를 읽어오는 함수
// recipe가 가리키는 Recipe 구조체에 파일에서 읽어온 값을 저장함 
void read_recipe_from_file(const char *code, RECIPE *recipe);

// recipe에서 출력할 줄들을 저장하는 배열 만드는 함수, public에서 사용 
void store_lines_public(char **lines, int *line_count, RECIPE *recipe); 

// recipe에서 출력할 줄들을 저장하는 배열 만드는 함수, private에서 사용
void store_lines_private(char **lines, int *line_count, RECIPE *recipe);

// store_lines에서 만든 배열로부터 각 줄을 꺼내서 30줄씩 출력하는 함수 
// g를 누르면 좋아요 추가, c를 누르면 댓글 추가, u & d를 누르면 페이지 전환 , q를 누르면 셀이 모두 지워지면서 함수가 종료됨
// 검색 등
void display_recipe_public(RECIPE *recipe);  

// store_lines에서 만든 배열로부터 각 줄을 꺼내서 30줄씩 출력하는 함수 
// u & d를 누르면 페이지 전환 , q를 누르면 셀이 모두 지워지면서 함수가 종료됨 (댓글, 좋아요 기능 없음)
// 내 보관함
void display_recipe_private(RECIPE *recipe); 

// 레시피 파일에서 좋아요를 1 증가시키는 함수  
void increase_likes(char *code); 

// 레시피 파일에서 댓글을 추가하는 함수 
void add_comment(char *code, const char *new_comment);  

// cod2.txt 파일의 [Recipes Referencing This] 섹션에 code1을 추가하는 함수
void add_code_to_referencing_section(const char *code1, const char *code2); 

// unique_code.txt에서 고유 코드를 읽고 1 증가시켜 반환하는 함수
char *get_and_increment_unique_code();

// 레시피를 입력받아 파일에 저장하는 함수, code를 입력으로 받음 
// 입력받은 형식대로 레시피 파일에 저장함 
void create_recipe_file();


// int main() {
//     RECIPE recipe;
//     init_recipe(&recipe);
//     read_recipe_from_file("111112", &recipe); 

//     // 영양 성분 출력 
//     // for (int i = 0; i < 7; i++) {
//     //     printf("%f ",(((recipe.ingre_arr.arr[1])->nutrient).arr[i])[0]);

//     // printf("zzzzzzz%d",is_ingredient_in_csv("reee"));
//     display_recipe_private(&recipe); 
//     // create_recipe_file(); 

//     free_recipe(&recipe);

//     return 0;
// } 



// 고유코드 및 메모와 id를 받아서 그 id를 가지는 파일의 마지막에 레시피의 고유코드와 메모를 저장하는 코드 
// 코드 추가 함수
void save_memo(const char *code, const char *memo) {
    char filepath[MAX_LENGTH];
    snprintf(filepath, MAX_LENGTH, "../data/personal_info/%s.txt", id);
    FILE *file = fopen(filepath, "a");
    if (!file) {
        printf("File for ID %s does not exist.\n", id);
        return;
    }

    fprintf(file, "%s - %s\n", code, memo);
    fclose(file);
}

// CSV 파일 내에 재료 이름이 존재하는지 확인하는 함수, 있으면 1 ,없으면 0 반환 
// 입력 값은 재료명
int is_ingredient_in_csv(const char *name) {
    FILE *file = fopen("../data/nutrient_data.csv", "r");
    if (!file) {
        printf("Could not open file\n");
        return 0;  // 파일을 열 수 없으면 재료가 없다고 판단
    }

    char line[MAX_LENGTH];
    char *token;
    int found = 0;

    // 첫 번째 라인은 헤더이므로 읽고 넘깁니다.
    if (fgets(line, MAX_LENGTH, file) == NULL) {
        fclose(file);
        return 0;
    }

    // 각 줄을 읽어가며 처리합니다.
    while (fgets(line, MAX_LENGTH, file)) {
        token = strtok(line, ",");
        if (token && strcmp(token, name) == 0) {
            found = 1;
            break;
        }
    }

    fclose(file);
    return found;
}

// 고유 코드 파일 존재 여부 확인 함수 , 있으면 1, 없으면 0 반환
// 일단은 private / public 모든 파일 가능하게 함 
// 입력 값은 코드명
int check_unique_code_exists(const char *code) {
    char filepath[MAX_LENGTH];
    snprintf(filepath, MAX_LENGTH, "../data/archive/%s.txt", code);
    FILE *file = fopen(filepath, "r");
    if (file) {
        fclose(file);
        return 1;
    }
    return 0;
}

// 텍스트 파일에서 Recipe 구조체로 데이터를 읽어오는 함수
// recipe가 가리키는 Recipe 구조체에 파일에서 읽어온 값을 저장함 
void read_recipe_from_file(const char *code, RECIPE *recipe) {
    char filepath[MAX_LENGTH]; 
    snprintf(filepath, MAX_LENGTH, "../data/archive/%s.txt", code);
    FILE *file = fopen(filepath, "r");
    if (file == NULL) {
        perror("Failed to open file");
        exit(EXIT_FAILURE);
    }

    char buffer[256];

    // Title
    fgets(buffer, sizeof(buffer), file); // Skip [Title]
    fgets(buffer, sizeof(buffer), file);
    buffer[strcspn(buffer, "\n")] = 0; // Remove newline character
    recipe->title = malloc(strlen(buffer) + 1);
    if (recipe->title == NULL) {
        perror("Failed to allocate memory for title");
        exit(EXIT_FAILURE);
    }
    strcpy(recipe->title, buffer);

    // Username
    fgets(buffer, sizeof(buffer), file); // Skip empty line
    fgets(buffer, sizeof(buffer), file); // Skip [Author]
    fgets(buffer, sizeof(buffer), file);
    buffer[strcspn(buffer, "\n")] = 0; // Remove newline character
    recipe->username = malloc(strlen(buffer) + 1);
    if (recipe->username == NULL) {
        perror("Failed to allocate memory for username");
        exit(EXIT_FAILURE);
    }
    strcpy(recipe->username, buffer);

    // Likes
    fgets(buffer, sizeof(buffer), file); // Skip empty line
    fgets(buffer, sizeof(buffer), file); // Skip [Likes]
    fgets(buffer, sizeof(buffer), file);
    sscanf(buffer, "%d", &(recipe->likes));

    // Ingredients
    fgets(buffer, sizeof(buffer), file); // Skip empty line
    fgets(buffer, sizeof(buffer), file); // Skip [Ingredients]
    fgets(buffer, sizeof(buffer), file);

    char *token = strtok(buffer, ",");

    while (token != NULL) {
        char *temp = &(*token);
        struct ingre *ingredient = malloc(sizeof(struct ingre));
        if (ingredient == NULL) {
            perror("Failed to allocate memory for ingredient");
            exit(EXIT_FAILURE);
        }

        // Parse name, quantity and unit
        char *open_paren = strchr(token, '(');
        char *close_paren = strchr(token, ')');
        if (open_paren != NULL && close_paren != NULL) {
            *open_paren = '\0';
            *close_paren = '\0';
            ingredient->name = malloc(strlen(token) + 1);
            if (ingredient->name == NULL) {
                perror("Failed to allocate memory for ingredient name");
                exit(EXIT_FAILURE);
            }
            strcpy(ingredient->name, trim_whitespace(token)); 

            ingredient->nutrient = return_nutrient(trim_whitespace(token));

            float quantity;
            char unit[50];
            sscanf(open_paren + 1, "%f%s", &quantity, unit);

            ingredient->quantity = malloc(sizeof(float));
            if (ingredient->quantity == NULL) {
                perror("Failed to allocate memory for ingredient quantity");
                exit(EXIT_FAILURE);
            }
            *(ingredient->quantity) = quantity;

            ingredient->unit = malloc(strlen(unit) + 1);
            if (ingredient->unit == NULL) {
                perror("Failed to allocate memory for ingredient unit");
                exit(EXIT_FAILURE);
            }
            strcpy(ingredient->unit, unit);
        }

        // init_f_darray(&ingredient->nutrient); // Initialize neutrient

        addto_ingre_darray(&recipe->ingre_arr, ingredient);
        token = strtok(NULL, ",");
    }

    // Text
    fgets(buffer, sizeof(buffer), file); // Skip empty line
    fgets(buffer, sizeof(buffer), file); // Skip [Instructions]
    while (fgets(buffer, sizeof(buffer), file) != NULL && strcmp(buffer, "\n") != 0) {
        buffer[strcspn(buffer, "\n")] = 0; // Remove newline character
        addto_darray(&recipe->text, strdup(buffer));
    }

    // Tags
    fgets(buffer, sizeof(buffer), file); // Skip [Tags]
    fgets(buffer, sizeof(buffer), file); 

    int pass = 0;

    if (strcmp(buffer, "\n") == 0) {
        pass = 1;
    } 

    token = strtok(buffer, ",");
    if (pass == 0) {
        while (token != NULL) {
            token[strcspn(token, "\n")] = 0; // Remove newline character
            addto_darray(&recipe->tag, strdup(trim_whitespace(token)));
            token = strtok(NULL, ",");
        }
    }


    // Comments
    if (pass == 0) {
        fgets(buffer, sizeof(buffer), file); // Skip empty line
    }

    fgets(buffer, sizeof(buffer), file); // Skip [Comments]
    while (fgets(buffer, sizeof(buffer), file) != NULL && strcmp(buffer, "\n") != 0) {
        buffer[strcspn(buffer, "\n")] = 0; // Remove newline character
        addto_darray(&recipe->comments, strdup(buffer));
    }

    // Referenced recipes
    fgets(buffer, sizeof(buffer), file); // Skip [Referenced Recipes]
    fgets(buffer, sizeof(buffer), file);  

    pass = 0;

    if (strcmp(buffer, "\n") == 0) {
        pass = 1;
    } 

    if (pass == 0) {
        token = strtok(buffer, ", ");
        while (token != NULL) {
            token[strcspn(token, "\n")] = 0; // Remove newline character
            addto_darray(&recipe->refer, strdup(token));
            token = strtok(NULL, ", ");
        }
    } 

    // Referenced by recipes
    if (pass == 0) {
        fgets(buffer, sizeof(buffer), file); // Skip empty line
    } 
    fgets(buffer, sizeof(buffer), file); // Skip [Recipes Referencing This]
    fgets(buffer, sizeof(buffer), file);

    pass = 0;

    if (strcmp(buffer, "\n") == 0) {
        pass = 1;
    } 

    token = strtok(buffer, ", ");
    if (pass == 0) {
        while (token != NULL) {
            token[strcspn(token, "\n")] = 0; // Remove newline character
            addto_darray(&recipe->refered, strdup(token));
            token = strtok(NULL, ", ");
        }
    }

    // Code
    if (pass == 0) {
        fgets(buffer, sizeof(buffer), file); // Skip empty line
    }
    fgets(buffer, sizeof(buffer), file); // Skip [Unique Code]
    fgets(buffer, sizeof(buffer), file);
    buffer[strcspn(buffer, "\n")] = 0; // Remove newline character
    recipe->code = malloc(strlen(buffer) + 1);
    if (recipe->code == NULL) {
        perror("Failed to allocate memory for code");
        exit(EXIT_FAILURE);
    }
    strcpy(recipe->code, buffer);

    fclose(file);
}

// recipe에서 출력할 줄들을 저장하는 배열 만드는 함수, public에서 사용 
void store_lines_public(char **lines, int *line_count, RECIPE *recipe) {
    char buffer[MAX_LENGTH];

    // Title and Username
    snprintf(buffer, MAX_LENGTH, "Title: %s", recipe->title);
    lines[(*line_count)++] = strdup(buffer);
    snprintf(buffer, MAX_LENGTH, "Username: %s", recipe->username);
    lines[(*line_count)++] = strdup(buffer);

    // Required ingredients
    lines[(*line_count)++] = strdup("-----------------------------------------------");
    lines[(*line_count)++] = strdup("Required ingredients");
    lines[(*line_count)++] = strdup("");
    for (int i = 0; i < recipe->ingre_arr.count; i++) {
        struct ingre *ingredient = recipe->ingre_arr.arr[i];
        snprintf(buffer, MAX_LENGTH, "%-30s %10.2f%s", ingredient->name, *ingredient->quantity, ingredient->unit);
        lines[(*line_count)++] = strdup(buffer);
    }

    // Cooking instructions
    lines[(*line_count)++] = strdup("-------------------------------------------------");
    lines[(*line_count)++] = strdup("Cooking instruction");
    lines[(*line_count)++] = strdup("");
    for (int i = 0; i < recipe->text.count; i++) {
        snprintf(buffer, MAX_LENGTH, "%s", recipe->text.arr[i]);
        lines[(*line_count)++] = strdup(buffer);
    }

    // Likes
    lines[(*line_count)++] = strdup("---------------------------------------------------");
    snprintf(buffer, MAX_LENGTH, "%*s Likes: %d  [press 'g' to express like]", 15, "", recipe->likes);
    lines[(*line_count)++] = strdup(buffer);

    // Comments
    lines[(*line_count)++] = strdup("Comments"); 
    lines[(*line_count)++] = strdup("");
    if (recipe->comments.count == 0) {
        lines[(*line_count)++] = strdup("There are no comments yet. Would you like to be the first to leave?");
    } else {
        for (int i = 0; i < recipe->comments.count; i++) {
            snprintf(buffer, MAX_LENGTH, "%s", recipe->comments.arr[i]);
            lines[(*line_count)++] = strdup(buffer);
        }
    }
    

    // Footer with referenced recipes
    lines[(*line_count)++] = strdup("------------------------------------------------------------");
    snprintf(buffer, MAX_LENGTH, "Unique Code of Referenced Recipes: ");
    for (int i = 0; i < recipe->refer.count; i++) {
        strcat(buffer, recipe->refer.arr[i]);
        if (i < recipe->refer.count - 1) strcat(buffer, ", ");
    }
    lines[(*line_count)++] = strdup(buffer);

    snprintf(buffer, MAX_LENGTH, "Unique Code of Recipes Referencing This: ");
    for (int i = 0; i < recipe->refered.count; i++) {
        strcat(buffer, recipe->refered.arr[i]);
        if (i < recipe->refered.count - 1) strcat(buffer, ", ");
    }
    lines[(*line_count)++] = strdup(buffer);

    snprintf(buffer, MAX_LENGTH, "Unique Code of this recipe: %s", recipe->code);
    lines[(*line_count)++] = strdup(buffer); 

    // Tags
    lines[(*line_count)++] = strdup("------------------------------------------------------------");
    lines[(*line_count)++] = strdup("Tag");
    lines[(*line_count)++] = strdup("");
    snprintf(buffer, MAX_LENGTH, "");
    for (int i = 0; i < recipe->tag.count; i++) {
        strcat(buffer, "#");
        strcat(buffer, recipe->tag.arr[i]);
        if (i < recipe->tag.count - 1) strcat(buffer, " ");
    }
    lines[(*line_count)++] = strdup(buffer);
    lines[(*line_count)++] = strdup("[press s to save this recipe to your folder]");
}

// recipe에서 출력할 줄들을 저장하는 배열 만드는 함수, private에서 사용 
void store_lines_private(char **lines, int *line_count, RECIPE *recipe) {
    char buffer[MAX_LENGTH];

    // Title and Username
    snprintf(buffer, MAX_LENGTH, "Title: %s", recipe->title);
    lines[(*line_count)++] = strdup(buffer);
    snprintf(buffer, MAX_LENGTH, "Username: %s", recipe->username);
    lines[(*line_count)++] = strdup(buffer);

    // Required ingredients
    lines[(*line_count)++] = strdup("-----------------------------------------------");
    lines[(*line_count)++] = strdup("Required ingredients");
    lines[(*line_count)++] = strdup("");
    for (int i = 0; i < recipe->ingre_arr.count; i++) {
        struct ingre *ingredient = recipe->ingre_arr.arr[i];
        snprintf(buffer, MAX_LENGTH, "%-30s %10.2f%s", ingredient->name, *ingredient->quantity, ingredient->unit);
        lines[(*line_count)++] = strdup(buffer);
    }

    // Cooking instructions
    lines[(*line_count)++] = strdup("-------------------------------------------------");
    lines[(*line_count)++] = strdup("Cooking instruction");
    lines[(*line_count)++] = strdup("");
    for (int i = 0; i < recipe->text.count; i++) {
        snprintf(buffer, MAX_LENGTH, "%s", recipe->text.arr[i]);
        lines[(*line_count)++] = strdup(buffer);
    }

    // Likes
    lines[(*line_count)++] = strdup("---------------------------------------------------");
    snprintf(buffer, MAX_LENGTH, "%*s Likes: %d ", 15, "", recipe->likes);
    lines[(*line_count)++] = strdup(buffer);

    // Comments
    lines[(*line_count)++] = strdup("Comments"); 
    lines[(*line_count)++] = strdup("");
    if (recipe->comments.count == 0) {
        lines[(*line_count)++] = strdup("There are no comments");
    } else {
        for (int i = 0; i < recipe->comments.count; i++) {
            snprintf(buffer, MAX_LENGTH, "%s", recipe->comments.arr[i]);
            lines[(*line_count)++] = strdup(buffer);
        }
    }
    

    // Footer with referenced recipes
    lines[(*line_count)++] = strdup("------------------------------------------------------------");
    snprintf(buffer, MAX_LENGTH, "Unique Code of Referenced Recipes: ");
    for (int i = 0; i < recipe->refer.count; i++) {
        strcat(buffer, recipe->refer.arr[i]);
        if (i < recipe->refer.count - 1) strcat(buffer, ", ");
    }
    lines[(*line_count)++] = strdup(buffer);

    snprintf(buffer, MAX_LENGTH, "Unique Code of Recipes Referencing This: ");
    for (int i = 0; i < recipe->refered.count; i++) {
        strcat(buffer, recipe->refered.arr[i]);
        if (i < recipe->refered.count - 1) strcat(buffer, ", ");
    }
    lines[(*line_count)++] = strdup(buffer);

    snprintf(buffer, MAX_LENGTH, "Unique Code of this recipe: %s", recipe->code);
    lines[(*line_count)++] = strdup(buffer); 

    // Tags
    lines[(*line_count)++] = strdup("------------------------------------------------------------");
    lines[(*line_count)++] = strdup("Tag");
    lines[(*line_count)++] = strdup("");
    snprintf(buffer, MAX_LENGTH, "");
    for (int i = 0; i < recipe->tag.count; i++) {
        strcat(buffer, "#");
        strcat(buffer, recipe->tag.arr[i]);
        if (i < recipe->tag.count - 1) strcat(buffer, " ");
    }
    lines[(*line_count)++] = strdup(buffer);
}

// store_lines에서 만든 배열로부터 각 줄을 꺼내서 30줄씩 출력하는 함수 
// g를 누르면 좋아요 추가, c를 누르면 댓글 추가, u & d를 누르면 페이지 전환 , q를 누르면 셀이 모두 지워지면서 함수가 종료됨
void display_recipe_public(RECIPE *recipe) {
    int like_pressed = 0;
    int save_pressed = 0;
    int comment_index = recipe->comments.count; // New comment index
    int page_index = 0;

    char *lines[1024]; // Assuming the total number of lines will not exceed 1024
    int line_count = 0;

    store_lines_public(lines, &line_count, recipe);
    
    while (1) {
        refresh_print("");

        int start_line = page_index * MAX_LINES;
        int end_line = start_line + MAX_LINES;
        if (end_line > line_count) end_line = line_count;

        for (int i = start_line; i < end_line; i++) {
            printf("%s\n", lines[i]);
        }

        printf("[press q to exit and press u or d to change page and press c to leave a comment!]\n");

        char input = getch();
        if (input == 'g' && !like_pressed) {
            recipe->likes++;
            like_pressed = 1;
            increase_likes(recipe->code);
            line_count = 0;
            store_lines_public(lines, &line_count, recipe);
        } else if (input == 'c') {
            printf("Enter your comment: ");
            char comment[256];
            fgets(comment, 256, stdin);
            comment[strcspn(comment, "\n")] = 0; // Remove newline character
            addto_darray(&recipe->comments, strdup(comment));
            add_comment(recipe->code,comment);
            line_count = 0;
            store_lines_public(lines, &line_count, recipe);
        } else if (input == 'd' && end_line < line_count) {
            page_index++;
        } else if (input == 'u' && page_index > 0) {
            page_index--;
        } else if (input == 'q') { 
            refresh_print("");
            break;
        } else if (input == 's' && !save_pressed) {
            save_pressed = 1;
            printf("Enter your memo: ");
            char memo[256];
            fgets(memo, 256, stdin);
            memo[strcspn(memo, "\n")] = 0;
            save_memo(recipe->code, memo);
        }

        // Clear the screen before reprinting
        refresh_print("");
        fflush(stdin);
    }

    for (int i = 0; i < line_count; i++) {
        free(lines[i]);
    }
}

// store_lines에서 만든 배열로부터 각 줄을 꺼내서 30줄씩 출력하는 함수 
// u & d를 누르면 페이지 전환 , q를 누르면 셀이 모두 지워지면서 함수가 종료됨 (댓글, 좋아요 기능 없음)
void display_recipe_private(RECIPE *recipe) {
    int like_pressed = 0;
    int comment_index = recipe->comments.count; // New comment index
    int page_index = 0;

    char *lines[1024]; // Assuming the total number of lines will not exceed 1024
    int line_count = 0;

    store_lines_private(lines, &line_count, recipe);
    
    while (1) {
        refresh_print("");

        int start_line = page_index * MAX_LINES;
        int end_line = start_line + MAX_LINES;
        if (end_line > line_count) end_line = line_count;

        for (int i = start_line; i < end_line; i++) {
            printf("%s\n", lines[i]);
        }

        printf("[press q to exit and press u or d to change page]\n");

        char input = getch();
        if (input == 'd' && end_line < line_count) {
            page_index++;
        } else if (input == 'u' && page_index > 0) {
            page_index--;
        } else if (input == 'q') {
            refresh_print("");
            break;
        }

        // Clear the screen before reprinting
        refresh_print("");
        fflush(stdin);
    }

    for (int i = 0; i < line_count; i++) {
        free(lines[i]);
    }
}

// 레시피 파일에서 좋아요를 1 증가시키는 함수  
void increase_likes(char *code) { 
    char filename[128];
    snprintf(filename, sizeof(filename), "../data/archive/%s.txt",code);

    FILE *file = fopen(filename, "r");
    if (file == NULL) {  // 파일이 없을 때
        return;
    }

    // 파일 내용 읽기
    char buffer[MAX_LENGTH];
    fread(buffer, sizeof(char), MAX_LENGTH, file);
    fclose(file);

    // 파일 내용에서 likes 부분을 찾고 값 증가
    char *likes_pos = strstr(buffer, "[Likes]");
    if (likes_pos == NULL) {
        printf("Likes section not found.\n");  // likes 항목을 찾을 수 없을 때
        return;
    }

    // 줄 바꿈 포함하여 "[Likes]" 문자열 다음 부분 찾기
    likes_pos = strchr(likes_pos, '\n') + 1;

    int likes;
    sscanf(likes_pos, "%d", &likes);
    likes++;

    // likes 값을 업데이트
    char updated_buffer[MAX_LENGTH];
    char *next_line = strchr(likes_pos, '\n');

    if (next_line != NULL) {
        // 기존 내용의 앞부분 복사
        strncpy(updated_buffer, buffer, likes_pos - buffer);
        updated_buffer[likes_pos - buffer] = '\0';
        // 증가된 likes 값을 추가
        snprintf(updated_buffer + (likes_pos - buffer), MAX_LENGTH - (likes_pos - buffer), "%d\n%s", likes, next_line + 1);
    } else {
        // next_line이 NULL이면 파일의 끝이므로, 전체 내용을 복사
        strncpy(updated_buffer, buffer, likes_pos - buffer);
        updated_buffer[likes_pos - buffer] = '\0';
        snprintf(updated_buffer + (likes_pos - buffer), MAX_LENGTH - (likes_pos - buffer), "%d\n", likes);
    }

    // 파일에 업데이트된 내용 쓰기
    file = fopen(filename, "w");
    if (file == NULL) {
        printf("Could not open the file.\n");  // 파일을 열 수 없을 때
        return;
    }
    fwrite(updated_buffer, sizeof(char), strlen(updated_buffer), file);
    fclose(file);
}

// 레시피 파일에서 댓글을 추가하는 함수 
void add_comment(char *code, const char *new_comment) {
    char filename[128];
    snprintf(filename, sizeof(filename), "../data/archive/%s.txt", code);

    FILE *file = fopen(filename, "r");
    if (file == NULL) { // 파일이 없을 때는 무시 -> private과 public 때문에 만듦
        return;
    }

    // 파일 내용 읽기
    char buffer[MAX_LENGTH];
    size_t read_size = fread(buffer, sizeof(char), MAX_LENGTH, file);
    buffer[read_size] = '\0';  // Null terminate the buffer
    fclose(file);

    // 파일 내용에서 comments 부분을 찾기
    char *comments_pos = strstr(buffer, "[Comments]");
    if (comments_pos == NULL) {
        printf("Comments section not found.\n");  // comments 항목을 찾을 수 없을 때
        return;
    }

    // 줄 바꿈 포함하여 "[Comments]" 문자열 다음 부분 찾기
    comments_pos = strchr(comments_pos, '\n') + 1;

    // 다음 섹션 찾기
    char *next_section = strstr(comments_pos, "\n[");
    if (next_section == NULL) {
        next_section = buffer + strlen(buffer);
    }

    // 새로운 버퍼 준비
    char updated_buffer[MAX_LENGTH];
    strncpy(updated_buffer, buffer, next_section - buffer);
    updated_buffer[next_section - buffer] = '\0';

    // 새로운 댓글 추가
    snprintf(updated_buffer + (next_section - buffer), MAX_LENGTH - (next_section - buffer), "%s\n%s", new_comment, next_section);

    // 파일에 업데이트된 내용 쓰기
    file = fopen(filename, "w");
    if (file == NULL) {
        printf("Could not open the file.\n");  // 파일을 열 수 없을 때
        return;
    }
    fwrite(updated_buffer, sizeof(char), strlen(updated_buffer), file);
    fclose(file);
} 

// cod2.txt 파일의 [Recipes Referencing This] 섹션에 code1을 추가하는 함수
void add_code_to_referencing_section(const char *code1, const char *code2) {
    char filepath[MAX_LENGTH];
    snprintf(filepath, MAX_LENGTH, "../data/archive/%s.txt", code2);

    FILE *file = fopen(filepath, "r");
    if (!file) {
        printf("Could not open file for code %s\n", code2);
        return;
    }

    // 임시 파일에 내용 복사 및 수정
    char temp_filepath[MAX_LENGTH];
    snprintf(temp_filepath, MAX_LENGTH, "../data/archive/%s_temp.txt", code2);

    FILE *temp_file = fopen(temp_filepath, "w");
    if (!temp_file) {
        printf("Could not open temporary file for writing\n");
        fclose(file);
        return;
    }

    char line[MAX_LENGTH];
    int in_referencing_section = 0;
    int referencing_section_empty = 1;

    while (fgets(line, sizeof(line), file)) {
        if (strncmp(line, "[Recipes Referencing This]", 26) == 0) {
            in_referencing_section = 1;
            fprintf(temp_file, "%s", line);
            continue;
        }

        if (in_referencing_section) {
            if (line[0] == '\n' || strncmp(line, "[", 1) == 0) {
                if (referencing_section_empty) {
                    fprintf(temp_file, "%s\n", code1);
                } else {
                    fprintf(temp_file, ", %s\n", code1);
                }
                in_referencing_section = 0;
            } else {
                char *trimmed_line = trim_whitespace(line);
                if (strlen(trimmed_line) > 0) {
                    referencing_section_empty = 0;
                    fprintf(temp_file, "%s", line);
                }
            }
        }

        if (!in_referencing_section) {
            fprintf(temp_file, "%s", line);
        }
    }

    if (in_referencing_section) {
        if (referencing_section_empty) {
            fprintf(temp_file, "%s\n", code1);
        } else {
            fprintf(temp_file, ", %s\n", code1);
        }
    }

    fclose(file);
    fclose(temp_file);

    // 원본 파일을 대체
    remove(filepath);
    rename(temp_filepath, filepath);
} 

// 고유 코드를 읽고 1 증가시켜 반환하는 함수
char *get_and_increment_unique_code() {
    char *unique_code = malloc(MAX_LENGTH);
    FILE *file = fopen("../data/unique_code.txt", "r+");
    if (!file) {
        perror("Failed to open unique_code.txt");
        exit(EXIT_FAILURE);
    }

    if (fgets(unique_code, MAX_LENGTH, file) == NULL) {
        perror("Failed to read unique_code.txt");
        fclose(file);
        exit(EXIT_FAILURE);
    }

    int code = atoi(unique_code);
    code++;
    snprintf(unique_code, MAX_LENGTH, "%d", code);

    // 파일의 처음으로 돌아가서 새로운 값을 쓰기
    rewind(file);
    fprintf(file, "%s\n", unique_code);
    fclose(file);

    return unique_code;
}

// 레시피를 입력받아 파일에 저장하는 함수, code를 입력으로 받음 
// 입력받은 형식대로 레시피 파일에 저장함 
void create_recipe_file() { 
    char *code = get_and_increment_unique_code();

    char filename[MAX_LENGTH];
    snprintf(filename, MAX_LENGTH, "../data/archive/%s.txt", code);
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        perror("Failed to create file");
        exit(EXIT_FAILURE);
    }

    char title[MAX_LENGTH];
    char author[MAX_LENGTH];
    char ingredients[MAX_LENGTH] = {0};
    char instructions[MAX_LENGTH * 5] = {0};
    char tags[MAX_LENGTH] = {0};
    char referenced_recipes[MAX_LENGTH * 3] = {0};

    int stage = 0;

    while (1) {
        if (stage == 0) {
            // Title 입력
            printf("Enter the title of the recipe:\n");
            fgets(title, MAX_LENGTH, stdin);
            title[strcspn(title, "\n")] = '\0'; // 개행 문자 제거
            if (strcmp(title, "modify") == 0) {
                printf("Nothing to modify. Enter the title of the recipe:\n");
                continue;
            }
            if (strlen(title) == 0) {
                printf("Title cannot be empty. Please enter the title again.\n");
                continue;
            }
            stage++;
        }

        if (stage == 1) {
            // Ingredients 입력
            printf("Enter the ingredients (one per line in the format 'Egg Yolk(1)'). Enter 'finish' to complete:\n");
            char ingredient[MAX_LENGTH];
            while (1) {
                fgets(ingredient, MAX_LENGTH, stdin);
                ingredient[strcspn(ingredient, "\n")] = '\0'; // 개행 문자 제거

                if (strcmp(ingredient, "finish") == 0) {
                    break;
                } else if (strcmp(ingredient, "modify") == 0) {
                    if (strlen(ingredients) > 0) {
                        // 마지막 재료 삭제
                        char *last_comma = strrchr(ingredients, ',');
                        if (last_comma != NULL) {
                            *last_comma = '\0';
                        } else {
                            ingredients[0] = '\0';
                        }
                        printf("Last ingredient removed. Enter new ingredient:\n");
                    } else {
                        stage--;
                        break;
                    }
                    continue;
                }

                // 형식 검사
                char *open_paren = strchr(ingredient, '(');
                char *close_paren = strchr(ingredient, ')');
                if (open_paren == NULL || close_paren == NULL || open_paren > close_paren) {
                    printf("Invalid format. Please use 'Ingredient Name(Quantity)'.\n");
                    continue;
                }

                // 숫자 확인
                *open_paren = '\0';
                char *quantity = open_paren + 1;
                *close_paren = '\0';
                char *unit = quantity;
                while (*unit && (isdigit(*unit) || *unit == '.')) unit++;
                if (unit == quantity || *unit == '\0') {
                    printf("Invalid format. Quantity must be a number followed by a unit.\n");
                    continue;
                }

                to_lower_case(ingredient);

                if (!is_ingredient_in_csv(ingredient)) { // 재료 이름이 리스트에 없을 경우 문제 출력 
                    printf("Invalid format. Ingredient is not in list.\n");
                    continue;
                }

                if (strlen(ingredients) > 0) {
                    strcat(ingredients, ", ");
                } 
                
                strcat(ingredients, ingredient);
                strcat(ingredients, "(");
                strcat(ingredients, quantity);
                strcat(ingredients, ")");
            }
            if (stage == 1) {
                stage++;
            }
        }

        if (stage == 2) {
            // Instructions 입력
            printf("Enter the instructions (one per line). Enter 'finish' to complete:\n");
            int step = 1;
            char instruction[MAX_LENGTH];
            while (1) {
                fgets(instruction, MAX_LENGTH, stdin);
                instruction[strcspn(instruction, "\n")] = '\0'; // 개행 문자 제거

                if (strcmp(instruction, "finish") == 0) {
                    break;
                } else if (strcmp(instruction, "modify") == 0) {
                    if (step > 1) {
                        // 마지막 단계 삭제
                        char *last_newline = strrchr(instructions, '\n');
                        if (last_newline != NULL) {
                            *last_newline = '\0';
                        } else {
                            instructions[0] = '\0';
                        }
                        step--;
                        printf("Last instruction removed. Enter new instruction:\n");
                    } else {
                        stage--;
                        break;
                    }
                    continue;
                }

                char numbered_instruction[MAX_LENGTH];
                snprintf(numbered_instruction, MAX_LENGTH, "%d. %s", step++, instruction);
                if (strlen(instructions) > 0) {
                    strcat(instructions, "\n");
                }
                strcat(instructions, numbered_instruction);
            }
            if (stage == 2) {
                stage++;
            }
        }

        if (stage == 3) {
            // Tags 입력
            while (1) {
                printf("Enter the tags (a, b, c, ...). Enter 'No' to skip:\n");
                fgets(tags, MAX_LENGTH, stdin);
                tags[strcspn(tags, "\n")] = '\0'; // 개행 문자 제거

                if (strcmp(tags, "No") == 0) {
                    tags[0] = '\0'; // 빈 문자열로 설정
                    break;
                }

                if (strcmp(tags, "modify") == 0) {
                    stage--;
                    break;
                }

                int valid = 1;
                char buffer[MAX_LENGTH] = {0};
                char *tag = tags;
                while (*tag) {
                    // 각 태그를 소문자로 변환
                    if (isalpha(*tag)) {
                        *tag = tolower(*tag);
                    }
                    tag++;
                }

                // 입력된 태그들을 버퍼에 복사
                strcpy(buffer, tags);

                if (!valid) {
                    printf("Invalid format. Please use 'a, b, c, ...'.\n");
                    continue;
                }
                strcpy(tags, buffer);
                break;
            }
            if (stage == 3) {
                stage++;
            }
        }

        if (stage == 4) {
            // Referenced Recipes 입력
            while (1) {
                printf("Enter the referenced recipes (a, b, c, ...). Enter 'No' to skip:\n");
                fgets(referenced_recipes, MAX_LENGTH, stdin);
                referenced_recipes[strcspn(referenced_recipes, "\n")] = '\0'; // 개행 문자 제거

                if (strcmp(referenced_recipes, "No") == 0) {
                    referenced_recipes[0] = '\0'; // 빈 문자열로 설정
                    break;
                }

                if (strcmp(referenced_recipes, "modify") == 0) {
                    stage--;
                    break;
                }

                int valid = 1;
                char buffer[MAX_LENGTH] = {0};
                char *referenced_code = strtok(referenced_recipes, ", ");
                while (referenced_code != NULL) {
                    for (int i = 0; referenced_code[i]; i++) {
                        if (!isdigit(referenced_code[i])) {
                            valid = 0;
                            break;
                        }
                    }
                    if (!valid || !check_unique_code_exists(referenced_code)) {
                        valid = 0;
                        break;
                    }
                    if (strlen(buffer) > 0) {
                        strcat(buffer, ", ");
                    }
                    strcat(buffer, referenced_code);
                    add_code_to_referencing_section(code, referenced_code);
                    referenced_code = strtok(NULL, ", ");
                }

                if (!valid) {
                    printf("Invalid format or code does not exist. Please use '1111, 2222, ...'.\n");
                    continue;
                }
                strcpy(referenced_recipes, buffer);
                break;
            }
            if (stage == 4) {
                stage++;
            }
        }

        if (stage == 5) {
            break;
        }
    }

    // 텍스트 파일에 쓰기
    fprintf(file, "[Title]\n%s\n\n", title);
    fprintf(file, "[Author]\n%s\n\n", id);
    fprintf(file, "[Likes]\n0\n\n");
    fprintf(file, "[Ingredients]\n%s\n\n", ingredients);
    fprintf(file, "[Instructions]\n%s\n\n", instructions);
    if (tags[0] == '\0') {
        fprintf(file, "[Tags]\n\n");
    }
    else {
        fprintf(file, "[Tags]\n%s\n\n", tags);
    }
    fprintf(file, "[Comments]\n\n");
    if (referenced_recipes[0] == '\0') {
        fprintf(file, "[Referenced Recipes]\n\n");
    }
    else {
        fprintf(file, "[Referenced Recipes]\n%s\n\n", referenced_recipes);
    }
    fprintf(file, "[Recipes Referencing This]\n\n");
    fprintf(file, "[Unique Code]\n%s\n", code);

    fclose(file);
    printf("Recipe file '%s' created successfully.\n", filename);
}
