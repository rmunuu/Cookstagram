#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sign.h"
#include "home.h"
#include "global_var.h"

int main() {
    while (!(id=signinup())) {} // 성공적으로 sign in 할때까지 반복

    int ret;
    ret = home();
    printf("%d\n", ret);
    

    return 0;
}

