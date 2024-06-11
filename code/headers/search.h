#ifdef SEARCH_H
#define SEARCH_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "./headers/structs.h"
#include "./headers/recipe_utils.h"

int count_matches(darray *arr, darray *subarr);
void sort_codes(darray *codes, int *keys);
void get_full_code(darray *codes); 
int *get_likes(darray *codes);
void get_ingre_name(ingre_darray ingredients, darray *ingre_names);
void split_string(darray *array, const char *string, char key); 
void split_args(QUERY *query, const char *args); 
void search_with_text(darray *codes, darray *full_codes, const char *text);
int *search_with_tag(darray *codes, darray *full_codes, darray *tags); 
int *search_with_ingredient(darray *codes, darray *full_codes, darray *ingres);
void select_recipe(darray *codes);
void search();

#endif