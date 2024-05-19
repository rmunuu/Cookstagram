#ifndef RECIPE_UTILS_H
#define RECIPE_UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h> 
#include <conio.h> 
#include <ctype.h>
#include <dirent.h>
#include "structs.h"
#include "basic_funcs.h"
#include "global_vars.h"

#define MAX_LENGTH 2048
#define MAX_LINES 30

// 고유코드 및 메모와 id를 받아서 그 id를 가지는 파일의 마지막에 레시피의 고유코드와 메모를 저장하는 코드 
// 코드 추가 함수
void save_memo(const char *code, const char *memo);

// CSV 파일 내에 재료 이름이 존재하는지 확인하는 함수, 있으면 1 ,없으면 0 반환 
// 입력 값은 재료명
int is_ingredient_in_csv(const char *name);

// 고유 코드 파일 존재 여부 확인 함수 , 있으면 1, 없으면 0 반환
// 일단은 private / public 모든 파일 가능하게 함 
// 입력 값은 코드명
int check_unique_code_exists(const char *code);

// 텍스트 파일에서 Recipe 구조체로 데이터를 읽어오는 함수
// recipe가 가리키는 Recipe 구조체에 파일에서 읽어온 값을 저장함 
void read_recipe_from_file(const char *code, RECIPE *recipe);

// recipe에서 출력할 줄들을 저장하는 배열 만드는 함수, public에서 사용 
void store_lines_public(char **lines, int *line_count, RECIPE *recipe); 

// recipe에서 출력할 줄들을 저장하는 배열 만드는 함수, private에서 사용
void store_lines_private(char **lines, int *line_count, RECIPE *recipe);

// store_lines에서 만든 배열로부터 각 줄을 꺼내서 30줄씩 출력하는 함수 
// g를 누르면 좋아요 추가, c를 누르면 댓글 추가, u & d를 누르면 페이지 전환 , q를 누르면 셀이 모두 지워지면서 함수가 종료됨
// 검색 등
void display_recipe_public(RECIPE *recipe);  

// store_lines에서 만든 배열로부터 각 줄을 꺼내서 30줄씩 출력하는 함수 
// u & d를 누르면 페이지 전환 , q를 누르면 셀이 모두 지워지면서 함수가 종료됨 (댓글, 좋아요 기능 없음)
// 내 보관함
void display_recipe_private(RECIPE *recipe); 

// 레시피 파일에서 좋아요를 1 증가시키는 함수  
void increase_likes(char *code); 

// 레시피 파일에서 댓글을 추가하는 함수 
void add_comment(char *code, const char *new_comment);  

// cod2.txt 파일의 [Recipes Referencing This] 섹션에 code1을 추가하는 함수
void add_code_to_referencing_section(const char *code1, const char *code2); 

// unique_code.txt에서 고유 코드를 읽고 1 증가시켜 반환하는 함수
char *get_and_increment_unique_code();

// 레시피를 입력받아 파일에 저장하는 함수, code를 입력으로 받음 
// 입력받은 형식대로 레시피 파일에 저장함 
void create_recipe_file();

#endif