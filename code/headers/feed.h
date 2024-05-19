#ifndef FEED_H
#define FEED_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h> 
#include <conio.h> 
#include <ctype.h>
#include <dirent.h>
#include <time.h>
#include "recipe_utils.h"
#include "global_vars.h"

// 주의 : public.txt에 없는 코드가 없을 경우 실행안될 수 있음 

int is_code_in_public_txt(const char *code);
char *get_code(const char *input);
void store_lines_feed(char **lines, int *lineCount, RECIPE *recipe);
void display_recipe_feed();

#endif