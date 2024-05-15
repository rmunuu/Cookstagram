#include <stdio.h>
#include "print.c"

int home() {
    int mode = 0;
    while (1) {
        refresh_print("<HOME>\nFEED[1]\nSEARCH[2]\nMY PAGE[3]\nRECOMMEND[4]\nselect mode: ");
        scanf("%d", &mode); // 입력 이상한거 하는 case 예외 처리?
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






