#include <stdio.h>
#include <string.h>
#include <conio.h>

void get_command(char *str);

int main() {
    char str[100];

    printf("hello world\n");
    get_command(str);
    printf("Returned str: %s", str);
    
    return 0;
}

void get_command(char *str) {
    while (1) {
        if (getch() != ':') {
            printf("\r\033[K%s", "");
            fflush(stdout);
            continue;
        } else {
            printf(":");
            scanf("%s", str);
            break;
        }
    }
}