#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h> 
#include <conio.h> 
#include <ctype.h>
#include <dirent.h>
#include "./headers/global_vars.h"

void to_lower_case(char *str);
char *trim_whitespace(char *str);
void refresh_print(const char *format, ...);

// 소문자로 변환하는 함수
void to_lower_case(char *str) {
    for (int i = 0; str[i]; i++) {
        str[i] = tolower((unsigned char)str[i]);
    }
}  

// 공백 문자 제거 함수
char *trim_whitespace(char *str) {
    char *end;

    // 선행 공백 제거
    while (isspace((unsigned char)*str)) str++;

    if (*str == 0)  // 문자열이 모두 공백인 경우
        return str;

    // 후행 공백 제거
    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end)) end--;

    // 새로운 종료 문자 삽입
    *(end + 1) = 0;

    return str;
}

// 화면 지우고 출력
void refresh_print(const char *format, ...) {
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