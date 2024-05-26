#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./headers/sign.h"
#include "./headers/home.h"
#include "./headers/global_vars.h"
#include "./headers/basic_funcs.h"
#include "./headers/feed.h"
#include "./headers/recommend.h"

int main() {
    while (!(id=signinup())) {
        printf("\nreturn to menu page... press Enter");
        getchar();
    } // 성공적으로 sign in 할때까지 반복
    int ret;
    while (1) {
        ret = home();
        switch (ret) {
            case 1:
                display_recipe_feed();
                break;
            case 2:
                // search();
                break;
            case 3:
                // my_page();
                break;
            case 4:
                recommend();
                break;
            case 0:
                exit(0);
            default:
                printf("Invalid input");
        }
    }

    return 0;
}

