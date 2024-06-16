#ifndef MY_PAGE_H
#define MY_PAGE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include "sign.h"
#include "basic_funcs.h"
#include "recipe_utils.h"
#include "global_vars.h"

void my_page();
void memo();
int handle_input();
int modify();
void replace_line(const char *filename, int line_num, const char *new_line);

#endif