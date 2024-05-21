#ifndef BASIC_FUNCS_H
#define BASIC_FUNCS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h> 
#include <conio.h> 
#include <ctype.h>
#include <dirent.h>
#include "global_vars.h"

void to_lower_case(char *str);
char *trim_whitespace(char *str);
void refresh_print(const char *format, ...);
void get_command(char *str);

#endif