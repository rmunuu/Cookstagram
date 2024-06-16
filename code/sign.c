#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include "./headers/basic_funcs.h"

char *signinup();
char *get_str();
char *signin();
void signup();
char **get_filenames(const char *directory, int *count);
int check_id(char *id);
int check_password(char *id, char *password);
void refresh_print(const char *format, ...);


// 각 input 에서 예외상황 check 필요

// sign in 성공 시 id 반환, 실패 or sign up 시 NULL 반환
char *signinup()
{
    int choice = 0;
    while (!choice)
    {
        refresh_print("sign in\t[1]\nsign up\t[2]\nexit\t[-1]\n");
        scanf("%d", &choice);
        int c;
        while ((c = getchar()) != '\n' && c != EOF); // 입력에서 뒤에 \n 같은거 제거
        if (choice == -1)
        {
            printf("exit program\n"); // 프로그램 종료
            exit(0);
        }
        else if (choice == 1)
        { // sign in
            printf("=== sign in ===\n");
            return signin();
        }
        else if (choice == 2)
        { // sign up
            printf("=== sign up ===\n");
            signup();
            return NULL;
        }
        else
        {
            printf("invalid choice\n");
            return NULL;
        }
    }
    return NULL;
}

// 최대 길이 30, \n 제거해서 입력 문자열 반환
char *get_str()
{
    char str[31];

    if (!fgets(str, 31, stdin))
    {
        perror("fgets failed");
        return NULL;
    }
    
    // 없어도 됨
    // if (strlen(str) > 30) {
    //     printf("input length should be less than 31\n");
    //     return NULL;
    // }

    int len = strlen(str);

    if (str[len - 1] == '\n')
    { // fgets 는 \n 까지 읽어들이므로 제거
        str[len - 1] = '\0';
        len --;
    }

    char *buffer = malloc(len + 1);
    if (!buffer)
    {
        perror("malloc failed");
        return NULL;
    }

    strcpy(buffer, str);

    return buffer;
}

// sign in 성공 시 id 반환, 실패 시 NULL 반환
char *signin()
{
    char *id;
    char *password;
    printf("id: ");
    if (!(id = get_str()))
    {
        return NULL; // id 입력 실패 시 NULL 반환
    }
    printf("password: ");
    if (!(password = get_str()))
    {
        free(id);
        return NULL; // password 입력 실패 시 NULL 반환
    }

    if (check_id(id))
    {
        if (check_password(id, password))
        {
            return id; // id, password 일치 시 id 반환
        }
        else
        {
            printf("incorrect password\n");
            free(id);
            free(password);
            return NULL; // password 틀릴 시 NULL 반환
        }
    }
    else
    {
        printf("id not found\n");
        free(id);
        free(password);
        return NULL; // id 없을 시 NULL 반환
    }
}

// id 중복체크, id, password 입력받아 파일에 저장
void signup()
{
    char *id;
    char *password;
    FILE *stream;

    printf("id: ");
    if (!(id = get_str()))
    {
        return;
    }

    if (check_id(id))
    { // id 중복체크
        printf("id already exists\n");
        free(id);
        return;
    }

    printf("password: "); // password 입력
    if (!(password = get_str()))
    {
        free(id);
        return;
    }

    char filename[100];

    sprintf(filename, "../data/personal_info/%s.txt", id); // 파일 경로 만들기
    if (!(stream = fopen(filename, "a")))
    { // 파일 열기 성공 여부
        perror("fopen failed");
        return;
    }

    fprintf(stream, "%s\n", id);
    fprintf(stream, "%s\n", password);

    free(id);
    free(password);
    fclose(stream);
}

// 디렉토리 내 파일 이름들 배열, 배열 길이 반환 GPT 도움
char **get_filenames(const char *directory, int *count)
{
    DIR *dir;
    struct dirent *entry;
    char **filenames = NULL;
    int capacity = 10; // Initial capacity of the array
    *count = 0; // Number of filenames

    dir = opendir(directory);
    if (dir == NULL)
    {
        perror("Failed to open directory");
        return NULL;
    }

    filenames = malloc(capacity * sizeof(char *));
    if (filenames == NULL)
    {
        perror("Failed to allocate memory");
        closedir(dir);
        return NULL;
    }

    while ((entry = readdir(dir)) != NULL)
    {
        if (entry->d_type == DT_REG)
        { // Check if it's a regular file
            if (*count >= capacity)
            {
                // Increase capacity
                capacity *= 2;
                char **new_space = realloc(filenames, capacity * sizeof(char *));
                if (new_space == NULL)
                {
                    perror("Failed to reallocate memory");
                    for (int i = 0; i < *count; i++)
                    {
                        free(filenames[i]);
                    }
                    free(filenames);
                    closedir(dir);
                    return NULL;
                }
                filenames = new_space;
            }
            filenames[*count] = strdup(entry->d_name);
            if (filenames[*count] == NULL)
            {
                perror("Failed to duplicate filename");
                for (int i = 0; i < *count; i++)
                {
                    free(filenames[i]);
                }
                free(filenames);
                closedir(dir);
                return NULL;
            }
            (*count)++;
        }
    }

    closedir(dir);
    return filenames;
}

// id 존재하는지 체크(있으면 1, 없으면 0 return)
int check_id(char *id)
{
    int id_num = 0;
    char **id_list = get_filenames("../data/personal_info", &id_num); // id 파일 이름들 배열

    char *temp_id = malloc(strlen(id) + 5);
    sprintf(temp_id, "%s.txt", id);

    for (int i = 0; i < id_num; i++)
    {
        if (strcmp(id_list[i], temp_id) == 0)
        {
            return 1;
        }
    }

    return 0;
}

// id 파일 찾아서 password 일치하는지 체크(맞으면 1, 틀리면 0 return)
int check_password(char *id, char *password)
{
    char filename[100];
    sprintf(filename, "../data/personal_info/%s.txt", id);

    FILE *stream;
    if (!(stream = fopen(filename, "r")))
    { // id password 저장된 파일 열기
        perror("fopen failed");
        return 0;
    }

    char *temp = malloc(31);
    char *pwd = malloc(31);

    fgets(temp, 31, stream);
    free(temp);

    fgets(pwd, 31, stream);

    int len = strlen(pwd);
    if (pwd[len - 1] == '\n')
    {
        pwd[len - 1] = '\0';
    }

    if (strcmp(pwd, password) == 0)
    { // password 입력값과 비교
        free(pwd);
        fclose(stream);
        return 1;
    }

    free(pwd);
    fclose(stream);
    return 0;
}

// 화면 지우고 출력,  GPT4 도움
void refresh_print(const char *format, ...)\
{
    // Clear the screen and move the cursor to the top-left corner
    printf("\033[2J\033[H");

    // Start variadic argument processing
    va_list args;
    va_start(args, format);
    
    // Print the new content using vprintf which accepts a va_list
    vprintf(format, args);

    // End variadic argument processing
    va_end(args);

    // Optionally, flush the output to ensure it appears immediately
    fflush(stdout);
}