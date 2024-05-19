#include <stdio.h>

int home();

int home() {
    int mode = 0;
    while (1) {
        refresh_print("<HOME>\nFEED\t[1]\nSEARCH\t[2]\nMY PAGE\t[3]\nRECOMMEND\t[4]\nselect mode: ");
        scanf("%d", &mode); // 입력 이상한거 하는 case 예외 처리?
        if (mode < 1 || mode > 4) {
            // printf("Invalid mode\n");
            getchar();
            continue;
        }
        
        switch (mode) {
            case 1:
                return 1;
            case 2:
                return 2;
            case 3:
                return 3;
            case 4:
                return 4;
        }
    }
}