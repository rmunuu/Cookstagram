#ifndef SIGN_H
#define SIGN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dirent.h"
#include "print.h"

char *signinup();
char *get_str();
char *signin();
void signup();
char **get_filenames(const char *directory, int *count);
int check_id(char *id);
int check_password(char *id, char *password);

#endif