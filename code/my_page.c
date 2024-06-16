#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include "./headers/sign.h"
#include "./headers/basic_funcs.h"
#include "./headers/recipe_utils.h"
#include "./headers/global_vars.h"

#define MAX_LENGTH 2048
#define LINES_PER_PAGE 30

void my_page();
void memo();
int handle_input();
int modify();
void replace_line(const char *filename, int line_num, const char *new_line);

void my_page()
{
    while (1) {
        refresh_print("<MY PAGE>\nMEMOES\t\t  [1]\nMODIFY_INFOS\t  [2]\nADD_RECIPE\t  [3]\nHOME\t\t  [0]\nselect mode:");
        int mode;
        scanf("%d", &mode);
        while (getchar() != '\n');
        
        while (mode < 0 || mode > 3)
        {
            printf("Invalid mode\n");
            scanf("%d", &mode);
        }
        switch (mode)
        {
            case 1:
                memo();
                break;
            case 2:
                while (1)
                {
                    if (modify())
                    {
                        continue;
                    }
                    else
                    {
                        break;
                    }
                }
                break;
            case 3:
                create_recipe_file();
                break;
            case 0:
                break;
        }
        if (!mode) break;
    }
}

void memo()
{
    char filename[MAX_LENGTH];
    sprintf(filename, "../data/personal_info/%s.txt", id);

    FILE *file = fopen(filename, "r");
    if (!file)
    {
        perror("file open error");
        getchar();
        return;
    }

    char line[MAX_LENGTH];
    int line_count = 0;
    int total_lines = 0;

    // 파일의 총 라인 수 계산
    while (fgets(line, sizeof(line), file))
    {
        total_lines++;
    }

    rewind(file);

    // 파일의 첫 두 줄을 건너뜁니다.
    while (line_count < 2 && fgets(line, sizeof(line), file))
    {
        line_count++;
    }

    int lines_printed = 0;
    while (line_count < total_lines)
    {
        int lines_this_page = 0;
        
        // 현재 페이지에서 최대 LINES_PER_PAGE 줄을 출력합니다.
        while (lines_this_page < LINES_PER_PAGE && fgets(line, sizeof(line), file))
        {
            int number;
            char string[MAX_LENGTH];

            // 정수-문자열 형식으로 파싱합니다.
            if (sscanf(line, "%d - %[^\n]", &number, string) == 2)
            {
                printf("%d - %s\n", number, string);
            }
            else
            {
                fprintf(stderr, "line %d parcing failed: %s", line_count + 1, line);
            }

            lines_this_page++;
            line_count++;
            lines_printed++;
        }

        // 아직 파일의 끝에 도달하지 않았으면 사용자 입력 대기
        if (line_count < total_lines)
        {
            printf("\nPress below arrow to show more...\n");
            while (handle_input() != -1);
            printf("\033[H\033[J"); // 화면 지우기
        }
    }

    printf("Press Enter to exit...");
    getchar();

    fclose(file);
}

int handle_input()
{
    char c = getch();
    if (c == '\033')
    {
        getch(); 
        switch (getch())
        {
            case 'A':
                return 1; // 위쪽 화살표 키
            case 'B':
                return -1; // 아래쪽 화살표 키
        }
    }
    return 0;
}

int modify()
{
    char new_password[MAX_LENGTH];
    printf("New password: ");
    if (!fgets(new_password, sizeof(new_password), stdin))
    {
        printf("Wrong input");
        return 1;
    }
    new_password[strcspn(new_password, "\n")] = '\0'; // 개행 문자 제거

    char filename[MAX_LENGTH];
    sprintf(filename, "../data/personal_info/%s.txt", id);
    replace_line(filename, 2, new_password);
    printf("Password changed successfully");
    getchar();
    return 0;
}

void replace_line(const char *filename, int line_num, const char *new_line)
{
    FILE *file = fopen(filename, "r");
    if (!file)
    {
        perror("file open error");
        return;
    }

    char *lines[MAX_LINES];
    int num_lines = 0;
    char buffer[MAX_LENGTH];

    // 모든 줄을 메모리에 읽어들임
    while (fgets(buffer, sizeof(buffer), file))
    {
        lines[num_lines] = malloc(strlen(buffer) + 1);
        if (!lines[num_lines])
        {
            perror("memory allocation error");
            fclose(file);
            return;
        }
        strcpy(lines[num_lines], buffer);
        num_lines++;
    }
    fclose(file);

    // 지정된 줄을 교체함
    if (line_num < 1 || line_num > num_lines)
    {
        printf("Line number overflowed the range\n");
        for (int i = 0; i < num_lines; i++)
        {
            free(lines[i]);
        }
        return;
    }

    free(lines[line_num - 1]);
    lines[line_num - 1] = malloc(strlen(new_line) + 2); // +2는 개행 문자와 null 종료 문자
    if (!lines[line_num - 1])
    {
        perror("memory allocation error");
        for (int i = 0; i < num_lines; i++)
        {
            free(lines[i]);
        }
        return;
    }
    strcpy(lines[line_num - 1], new_line);
    strcat(lines[line_num - 1], "\n");

    // 모든 줄을 파일에 다시 씀
    file = fopen(filename, "w");
    if (!file)
    {
        perror("file open error");
        for (int i = 0; i < num_lines; i++)
        {
            free(lines[i]);
        }
        return;
    }

    for (int i = 0; i < num_lines; i++)
    {
        fputs(lines[i], file);
        free(lines[i]);
    }
    fclose(file);
}
