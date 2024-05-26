#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h> 
#include <conio.h> 
#include <ctype.h>
#include <dirent.h>
#include <time.h>
#include "./headers/recipe_utils.h"
#include "./headers/global_vars.h"

// 주의 : public.txt에 없는 코드가 없을 경우 실행안될 수 있음 

int is_code_in_public_txt(const char *code);
char *get_code(const char *input);
void store_lines_feed(char **lines, int *lineCount, RECIPE *recipe);
void display_recipe_feed();

// int main() {
//     display_recipe_feed();

//     return 0;
// }

// 주어진 코드가 public.txt에 있는지 확인
int is_code_in_public_txt(const char *code) {
    FILE *file = fopen("../data/archive/public.txt", "r");
    if (!file) {
        printf("Could not open public.txt\n");
        return 0;
    }

    char line[MAX_LENGTH];
    while (fgets(line, MAX_LENGTH, file)) {
        char *trimmed_line = trim_whitespace(line);
        if (strcmp(trimmed_line, code) == 0) {
            fclose(file);
            return 1;
        }
    }

    fclose(file);
    return 0;
} 

// 고유코드 처리 함수
char *get_code(const char *input) {
    FILE *file;
    char **codes = NULL;
    int code_count = 0;
    char *result_code = NULL;

    if (strcmp(input, "random") == 0) {
        file = fopen("../data/archive/public.txt", "r");
        if (!file) {
            printf("Could not open public.txt\n");
            return NULL;
        }

        char line[MAX_LENGTH];
        while (fgets(line, MAX_LENGTH, file)) {
            codes = realloc(codes, sizeof(char *) * (code_count + 1));
            codes[code_count] = strdup(trim_whitespace(line));
            code_count++;
        }

        fclose(file);

        if (code_count > 0) {
            srand(time(NULL));
            int random_index = rand() % code_count;
            result_code = strdup(codes[random_index]);

            for (int i = 0; i < code_count; i++) {
                free(codes[i]);
            }
            free(codes);
        }
    } else {
        if (!is_code_in_public_txt(input)) {
            printf("Code not found in public.txt.\n");
            return NULL;
        }

        result_code = strdup(input);
    }

    return result_code;
}

// recipe에서 출력할 줄들을 저장하는 배열 만드는 함수, public에서 사용 
void store_lines_feed(char **lines, int *lineCount, RECIPE *recipe) {
    char buffer[MAX_LENGTH];

    // Title and Username
    snprintf(buffer, MAX_LENGTH, "Title: %s", recipe->title);
    lines[(*lineCount)++] = strdup(buffer);
    snprintf(buffer, MAX_LENGTH, "Username: %s", recipe->username);
    lines[(*lineCount)++] = strdup(buffer);

    // Required ingredients
    lines[(*lineCount)++] = strdup("-----------------------------------------------");
    lines[(*lineCount)++] = strdup("Required ingredients");
    lines[(*lineCount)++] = strdup("");
    for (int i = 0; i < recipe->ingre_arr.count; i++) {
        struct ingre *ingredient = recipe->ingre_arr.arr[i];
        snprintf(buffer, MAX_LENGTH, "%-30s %10.2f%s", ingredient->name, *ingredient->quantity, ingredient->unit);
        lines[(*lineCount)++] = strdup(buffer);
    }

    // Cooking instructions
    lines[(*lineCount)++] = strdup("-------------------------------------------------");
    lines[(*lineCount)++] = strdup("Cooking instruction");
    lines[(*lineCount)++] = strdup("");
    for (int i = 0; i < recipe->text.count; i++) {
        snprintf(buffer, MAX_LENGTH, "%s", recipe->text.arr[i]);
        lines[(*lineCount)++] = strdup(buffer);
    }

    // Likes
    lines[(*lineCount)++] = strdup("---------------------------------------------------");
    snprintf(buffer, MAX_LENGTH, "%*s Likes: %d", 15, "", recipe->likes);
    lines[(*lineCount)++] = strdup(buffer);

    // Comments
    lines[(*lineCount)++] = strdup("Comments"); 
    lines[(*lineCount)++] = strdup("");
    if (recipe->comments.count == 0) {
        lines[(*lineCount)++] = strdup("There are no comments yet. Would you like to be the first to leave?");
    } else {
        for (int i = 0; i < recipe->comments.count; i++) {
            snprintf(buffer, MAX_LENGTH, "%s", recipe->comments.arr[i]);
            lines[(*lineCount)++] = strdup(buffer);
        }
    }
    

    // Footer with referenced recipes
    lines[(*lineCount)++] = strdup("------------------------------------------------------------");
    snprintf(buffer, MAX_LENGTH, "Unique Code of Referenced Recipes: ");
    for (int i = 0; i < recipe->refer.count; i++) {
        strcat(buffer, recipe->refer.arr[i]);
        if (i < recipe->refer.count - 1) strcat(buffer, ", ");
    }
    lines[(*lineCount)++] = strdup(buffer);

    snprintf(buffer, MAX_LENGTH, "Unique Code of Recipes Referencing This: ");
    for (int i = 0; i < recipe->refered.count; i++) {
        strcat(buffer, recipe->refered.arr[i]);
        if (i < recipe->refered.count - 1) strcat(buffer, ", ");
    }
    lines[(*lineCount)++] = strdup(buffer);

    snprintf(buffer, MAX_LENGTH, "Unique Code of this recipe: %s", recipe->code);
    lines[(*lineCount)++] = strdup(buffer); 

    // Tags
    lines[(*lineCount)++] = strdup("------------------------------------------------------------");
    lines[(*lineCount)++] = strdup("Tag");
    lines[(*lineCount)++] = strdup("");
    snprintf(buffer, MAX_LENGTH, "");
    for (int i = 0; i < recipe->tag.count; i++) {
        strcat(buffer, "#");
        strcat(buffer, recipe->tag.arr[i]);
        if (i < recipe->tag.count - 1) strcat(buffer, " ");
    }
    lines[(*lineCount)++] = strdup(buffer);
    lines[(*lineCount)++] = strdup("[press s to save this recipe to your folder]");
}

// store_lines에서 만든 배열로부터 각 줄을 꺼내서 30줄씩 출력하는 함수 
// g를 누르면 좋아요 추가, c를 누르면 댓글 추가, u & d를 누르면 페이지 전환 , q를 누르면 셀이 모두 지워지면서 함수가 종료됨, A를 누르면 다음 피드로 
// 전역변수인 id를 매개변수로 받아와야 할듯 
void display_recipe_feed() {
    char *code;
    char check_q = 'b';
    while (1) {
        if (check_q == 'q') {
            break;
        }
        RECIPE rec; 
        init_recipe(&rec); 
        RECIPE *recipe = &rec;
        code = get_code("random");
        read_recipe_from_file(code, recipe); 
        int likePressed = 0;
        int savePressed = 0;
        int commentIndex = recipe->comments.count; // New comment index
        int pageIndex = 0;

        char *lines[1024]; // 최대 한줄 길이 1024
        int lineCount = 0; 

        store_lines_feed(lines, &lineCount, recipe);
        
        while (1) {
            refresh_print("");

            int startLine = pageIndex * MAX_LINES;
            int endLine = startLine + MAX_LINES;
            if (endLine > lineCount) endLine = lineCount;

            for (int i = startLine; i < endLine; i++) {
                printf("%s\n", lines[i]);
            }

            printf("[q]uit, [u]p, [d]own, [c]omment, [r]andom\n");

            char input = getch();
            if (input == 'g' && !likePressed) {
                recipe->likes++;
                likePressed = 1;
                increase_likes(recipe->code);
                lineCount = 0; 
                store_lines_feed(lines, &lineCount, recipe);
            } else if (input == 'c') {
                printf("Enter your comment: ");
                char comment[256];
                fgets(comment, 256, stdin);
                comment[strcspn(comment, "\n")] = 0; // Remove newline character
                addto_darray(&recipe->comments, strdup(comment));
                add_comment(recipe->code,comment);
                lineCount = 0;
                store_lines_feed(lines, &lineCount, recipe);
            } else if (input == 'd' && endLine < lineCount) {
                pageIndex++;
            } else if (input == 'u' && pageIndex > 0) {
                pageIndex--;
            } else if (input == 'q') { // q를 누르면 함수 자체를 탈출
                check_q = 'q'; 
                refresh_print("");
                break;
            } else if (input == 'r') { // r를 누르면 다음 레시피로 
                refresh_print("");
                break;
            } else if (input == 's' && !savePressed) {
                savePressed = 1;
                printf("Enter your memo: ");
                char memo[256];
                fgets(memo, 256, stdin);
                memo[strcspn(memo, "\n")] = 0;
                save_memo(recipe->code, memo);
            }

            // Clear the screen before reprinting
            refresh_print("");
            //fflush(stdout);
        }

        for (int i = 0; i < lineCount; i++) {
            free(lines[i]);
        }
        free_recipe(&rec);
    }
    
}


