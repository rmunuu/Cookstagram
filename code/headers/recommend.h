#ifndef RECOMMEND_H
#define RECOMMEND_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <errno.h> 
#include "sign.h"
#include "basic_funcs.h"
#include "recipe_utils.h"
#include "global_vars.h"
#include "structs.h"

void recommend();

void help();

void sort_calories();
void calc_calories(RECIPE*, float*);
void sort_protein();
void calc_protein(RECIPE*, float*);
void sort_fat();
void calc_fat(RECIPE*, float*);
void lcar_hfat();
void calc_car(RECIPE*,float*);

void today_menu();
int weighted_random_selection(int *,float *,int);

void return_all_code(darray* );
void return_public_code(darray* );
void get_file_names(const char*, darray*);
void remove_txt_extension(char *);


#endif