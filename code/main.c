#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./headers/sign.h"
#include "./headers/home.h"
#include "./headers/global_vars.h"
#include "./headers/basic_funcs.h"
#include "./headers/feed.h"

int main() {
    while (!(id=signinup())) {} // 성공적으로 sign in 할때까지 반복

    int ret;
    ret = home();
    // printf("%d\n", ret);
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
            // recommend();
            break;
    }
    

    return 0;
}

