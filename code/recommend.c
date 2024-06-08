#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include "./headers/sign.h"
#include "./headers/basic_funcs.h"
#include "./headers/recipe_utils.h"
#include "./headers/global_vars.h"
#include "./headers/structs.h"

//recommend에서 입력으로 어떤 종류의 추천으로 입력받을지 확인한다.
//recommend 내에서 입력받고, 이후 다른 종류의 함수로 이동하여 추천하게 된다.
//recommend 할 수 있는 옵션을 무엇이 있을까?
// 명령어 도움 - "help" 라고 입력하면 각각의 옵션에 대해 도움을 준다. 

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


void recommend()
{
    char command[MAX_LENGTH];
    while(1)
    {   
        refresh_print("input your options. input \"help\" if you need help.\n");
        scanf(" %s",command);
        if(strcmp(command, ":q") == 0) break;
        if(strcmp(command, "help") == 0) help();
        if(strcmp(command, "cal") == 0) sort_calories();
        if(strcmp(command, "protein") == 0) sort_protein();
        if(strcmp(command, "fat") == 0) sort_fat(); 
        if(strcmp(command, "lcar_hfat") == 0) lcar_hfat();
        if(strcmp(command, "today_menu") == 0) today_menu();
    }

}

//각 명령어에 대한 설명을 제공한다.
void help()
{
    int check = 1;
    char input,pass;
    while(check == 1)
    {
    refresh_print("On recommend interface, it gives various recommendation options for the user.\n");
    printf("there are six commands.\n");
    printf(":q cal protein fat lcar_hfat today_menu\n");
    printf("you can read specific explanations of each command if you input keywords.\n");
    printf("keywords: :q[q] cal[c] protein[p] fat[f] lcar_hfat[l] today_menu[t]\n");
    printf("input out[o] to return to recommend menu.\n");
    scanf(" %c",&input);
    switch(input)
    {
        case 'q':
        refresh_print("If you input \":q\" you can return from the recommend menu \
to the home menu. Then, from the home menu, you can navigate to the \
desired menu (recommend, my page, feed, search). \n");
        scanf(" %c",&pass);
        break;
        case 'c':
        refresh_print("If you input \"cal\", you can view recipes sorted by calorie \
criteria. Both ascending and descending orders are possible, and you can also specify\
the index you want to display.");
        scanf(" %c",&pass);
        break;
        case 'p':
        refresh_print("If you input \"protein\", you can view recipes sorted by \
protein criteria. Both ascending and descending orders are possible, and you can \
specify the index you want to display.");
        scanf(" %c",&pass);
        break;
        case 'f':
        refresh_print("If you input \"fat\", you can view recipes sorted by \
fat criteria. Both ascending and descending orders are possible, and you can \
specify the index you want to display.");
        scanf(" %c",&pass);
        break;
        case 'l':
        refresh_print("If you input \"lcar_hfat\", you can view recipes \
sorted by low-carb and high-fat criteria. Both ascending and descending \
orders are possible, and you can specify the index you want to display. \
The criterion is calculated based on the difference in calories from \
carbohydrates and proteins.");
        scanf(" %c",&pass);
        break;
        case 't':
        refresh_print("If you input \"today_menu\", you can receive \
recommendations for recipes from the public archive based on the tags \
and ingredients you specify in order. The probability of receiving a \
recommendation for a recipe increases as the number of likes and comments \
increases, and this is determined probabilistically using the exponential distribution.");
        scanf(" %c",&pass);
        break;
        case 'o':
        check = 0;
        break;
        default:
        printf("No commands detected. please input one more time.\n");
        break;
    }
    }
}

// 크거나 작은 칼로리를 가진 레시피 순으로 정렬하여 출력한다. 
void sort_calories()
{
    printf("flag1");
    int i,j;
    int index,num;
    char input;
    darray* filenames;
    init_darray(filenames);
    return_public_code(filenames);
    printf("flag2");
    RECIPE** recipes = malloc(sizeof(RECIPE*) * filenames->count);
    float* calories = malloc(sizeof(float) * filenames->count);
    float* argsort = malloc(sizeof(float) * filenames->count);

    for(i=0;i<filenames->count;i++)
    {
        RECIPE rec; 
        init_recipe(&rec);
        recipes[i] = &rec; 
        read_recipe_from_file(filenames->arr[i],recipes[i]);

        calories[i] = 0;
        calc_calories(recipes[i],&(calories[i]));
    }
    for(i=0;i<filenames->count;i++)
    {
        argsort[i] = 0;
        for(j=0;j<filenames->count;j++)
        {
            if(calories[j]<calories[i])
            {
                argsort[i]++;
            }
        }
    }
    //refresh해야됨
    printf("please input options, high calories[h], low calories[l]\n");
    scanf(" %c",&input);
    printf("please input number of index you want from 1 to %d\n",filenames->count);
    scanf("%d",&index);
    num = (input=='h')? (filenames->count -1 - (index-1)) : (index-1);
    for(i=0;i<filenames->count;i++)
    {
        if(argsort[i]==num)
        {
            printf("Calorie of the recipe is %lf.\nWill you go to check the recipe? yes[y] no[n]\n",calories[i]);
            scanf(" %c",&input);
            if( input == 'y')
            {
                display_recipe_public(recipes[i]);
            }
        }
    }
    free(recipes);
    free(argsort);
}

//레시피의 칼로리를 계산한다. 
void calc_calories(RECIPE* recipe,float* calorie)
{
    ingre_darray ingre_arr = recipe->ingre_arr;
    struct ingre ingredient;
    int i;
    for(i=0;i<ingre_arr.count;i++)
    {
        ingredient = *(ingre_arr.arr[i]);
        *calorie = *calorie + ingredient.nutrient.arr[0][0] * (*ingredient.quantity);
    }
}

// 크거나 작은 단백질을 가진 레시피 순으로 정렬하여 출력한다. 
void sort_protein()
{
    int i,j;
    int index,num;
    char input;
    darray* filenames;
    return_public_code(filenames);
    RECIPE** recipes = malloc(sizeof(RECIPE*) * filenames->count);
    float* protein = malloc(sizeof(float) * filenames->count);
    float* argsort = malloc(sizeof(float) * filenames->count);

    for(i=0;i<filenames->count;i++)
    {
        RECIPE rec; 
        init_recipe(&rec);
        recipes[i] = &rec; 
        read_recipe_from_file(filenames->arr[i],recipes[i]);

        protein[i] = 0;
        calc_protein(recipes[i],&(protein[i]));
    }
    for(i=0;i<filenames->count;i++)
    {
        argsort[i] = 0;
        for(j=0;j<filenames->count;j++)
        {
            if(protein[j]<protein[i])
            {
                argsort[i]++;
            }
        }
    }
    refresh_print("please input options, high protein[h], low protein[l]\n");
    scanf(" %c",&input);
    printf("please input number of index you want from 1 to %d\n",filenames->count);
    scanf("%d",&index);
    num = (input=='h')? (filenames->count -1 - (index-1)) : (index-1);
    for(i=0;i<filenames->count;i++)
    {
        if(argsort[i]==num)
        {
            printf("protein of the recipe is %lf.\nWill you go to check the recipe? yes[y] no[n]\n",protein[i]);
            scanf(" %c",&input);
            if( input == 'y')
            {
                display_recipe_public(recipes[i]);
            }
        }
    }
    free(recipes);
    free(argsort);
}

//레시피의 단백질을 계산한다. 
void calc_protein(RECIPE* recipe,float* protein)
{
    ingre_darray ingre_arr = recipe->ingre_arr;
    struct ingre ingredient;
    int i;
    for(i=0;i<ingre_arr.count;i++)
    {
        ingredient = *(ingre_arr.arr[i]);
        *protein = *protein + ingredient.nutrient.arr[1][0] * (*ingredient.quantity);
    }
}

// 크거나 작은 지방을 가진 레시피 순으로 정렬하여 출력한다. 
void sort_fat()
{
    int i,j;
    int index,num;
    char input;
    darray* filenames;
    return_public_code(filenames);
    RECIPE** recipes = malloc(sizeof(RECIPE*) * filenames->count);
    float* fat = malloc(sizeof(float) * filenames->count);
    float* argsort = malloc(sizeof(float) * filenames->count);

    for(i=0;i<filenames->count;i++)
    {
        RECIPE rec; 
        init_recipe(&rec);
        recipes[i] = &rec; 
        read_recipe_from_file(filenames->arr[i],recipes[i]);

        fat[i] = 0;
        calc_fat(recipes[i],&(fat[i]));
    }
    for(i=0;i<filenames->count;i++)
    {
        argsort[i] = 0;
        for(j=0;j<filenames->count;j++)
        {
            if(fat[j]<fat[i])
            {
                argsort[i]++;
            }
        }
    }
    refresh_print("please input options, high fat[h], low fat[l]\n");
    scanf(" %c",&input);
    printf("please input number of index you want from 1 to %d\n",filenames->count);
    scanf("%d",&index);
    num = (input=='h')? (filenames->count -1 - (index-1)) : (index-1);
    for(i=0;i<filenames->count;i++)
    {
        if(argsort[i]==num)
        {
            printf("fat of the recipe is %lfcal.\nWill you go to check the recipe? yes[y] no[n]\n",fat[i]);
            scanf(" %c",&input);
            if( input == 'y')
            {
                display_recipe_public(recipes[i]);
            }
        }
    }
    free(recipes);
    free(argsort);
}

//레시피의 지방을 계산한다. 
void calc_fat(RECIPE* recipe,float* fat)
{
    ingre_darray ingre_arr = recipe->ingre_arr;
    struct ingre ingredient;
    int i;
    for(i=0;i<ingre_arr.count;i++)
    {
        ingredient = *(ingre_arr.arr[i]);
        *fat = *fat + ingredient.nutrient.arr[2][0] * (*ingredient.quantity);
    }
}

// 저탄고지 순서로 정렬하여 레시피를 출력한다. 기준은 탄수화물과 지방의 차를 정렬(칼로리 기준)한 것이다. 
void lcar_hfat()
{
    int i,j;
    int index,num;
    char input;
    darray* filenames;
    return_public_code(filenames);
    RECIPE** recipes = malloc(sizeof(RECIPE*) * filenames->count);
    float* car_fat = malloc(sizeof(float) * filenames->count);
    float* argsort = malloc(sizeof(float) * filenames->count);
    float* car = malloc(sizeof(float) * filenames->count);
    float* fat = malloc(sizeof(float) * filenames->count);

    for(i=0;i<filenames->count;i++)
    {
        RECIPE rec; 
        init_recipe(&rec);
        recipes[i] = &rec; 
        read_recipe_from_file(filenames->arr[i],recipes[i]);

        car_fat[i] = 0;
        calc_car(recipes[i],&(car[i]));
        calc_fat(recipes[i],&(fat[i]));
        car_fat[i] = car[i]*4 - fat[i]*9;
    }
    free(car);
    free(fat);
    for(i=0;i<filenames->count;i++)
    {
        argsort[i] = 0;
        for(j=0;j<filenames->count;j++)
        {
            if(car_fat[j]<car_fat[i])
            {
                argsort[i]++;
            }
        }
    }
    refresh_print("please input options, high car_fat[h], low car_fat[l]\n");
    printf("high car_fat meant higher carbohydrate, lower fat\n");
    scanf(" %c",&input);
    printf("please input number of index you want from 1 to %d\n",filenames->count);
    scanf("%d",&index);
    num = (input=='h')? (filenames->count -1 - (index-1)) : (index-1);
    for(i=0;i<filenames->count;i++)
    {
        if(argsort[i]==num)
        {
            printf("car_fat of the recipe is %lf.\nWill you go to check the recipe? yes[y] no[n]\n",car_fat[i]);
            scanf(" %c",&input);
            if( input == 'y')
            {
                display_recipe_public(recipes[i]);
            }
        }
    }
    free(recipes);
    free(argsort);
}

//레시피의 탄수화물 양을 계산한다.
void calc_car(RECIPE* recipe,float* car)
{
    ingre_darray ingre_arr = recipe->ingre_arr;
    struct ingre ingredient;
    int i;
    for(i=0;i<ingre_arr.count;i++)
    {
        ingredient = *(ingre_arr.arr[i]);
        *car = *car + ingredient.nutrient.arr[3][0] * (*ingredient.quantity);
    }
}

// 각 레시피의 정보들을 분석하여 오늘의 메뉴를 추천해준다. 
void today_menu()
{
    int index,num,counter,i,j,temp_count,check;
    char input;
    darray* filenames;
    init_darray(filenames);
    char option[100];
    return_public_code(filenames);
    counter = filenames->count;
    RECIPE** recipes = malloc(sizeof(RECIPE*) * counter);
    int* selected_index = malloc(sizeof(int) * counter);
    int* likes = malloc(sizeof(int) * counter);
    int* comments = malloc(sizeof(int) * counter);
    
    for(i=0;i<counter;i++)
    {
        RECIPE rec; 
        init_recipe(&rec);
        recipes[i] = &rec; 
        read_recipe_from_file(filenames->arr[i],recipes[i]);
    }
    for(i=0;i<counter;i++)
    {
        selected_index[i] = 1;
        likes[i] = recipes[i]->likes;
        comments[i] = recipes[i]->comments.count;
    }
    refresh_print("input options of tags. You can keep input tags and input \"stop\" to finish it.\n");
    while(1)
    {
        scanf("%s",option);
        if(strcmp(option,"stop")==0)
        {
            break;
        }
        for(i=0;i<counter;i++)
        {
            check = 0;
            temp_count = recipes[i]->tag.count;
            for(j=0;i<temp_count;j++)
            {
                if(strcmp(option,recipes[i]->tag.arr[j]) == 0)
                {
                    check = 1;
                }
            }
            if(check==0)
            {
                selected_index[i] = 0;
            }
        }
    }
    refresh_print("input options of ingredients. You can keep input ingredients and input \"stop\" to finish it.\n");
    while(1)
    {
        scanf("%s",option);
        if(strcmp(option,"stop")==0)
        {
            break;
        }
        for(i=0;i<counter;i++)
        {
            check = 0;
            temp_count = recipes[i]->ingre_arr.count;
            for(j=0;i<temp_count;j++)
            {
                if(strcmp(option,recipes[i]->ingre_arr.arr[j]->name) == 0)
                {
                    check = 1;
                }
            }
            if(check==0)
            {
                selected_index[i] = 0;
            }
        }
    }
    temp_count = 0;
    int* values = malloc(sizeof(int) * counter);
    float* probabilities = malloc(sizeof(float) * counter);
    float sum;
    int selection;
    for(i=0;i<counter;i++)
    {
        if(selected_index[i]==1)
        {
            values[temp_count] = i;
            probabilities[i] = exp(likes[i]*0.5 + comments[i]);
            sum = sum + probabilities[i];
            temp_count++;
        }
    }
    for(i=0;i<temp_count;i++)
    {
        probabilities[i] = probabilities[i] / sum;
    }
    selection = weighted_random_selection(values,probabilities, temp_count);
    printf("%d is the index, and code is %s\n",selection, filenames->arr[selection]);
    printf("input to see the recommended recipe. yes[y] no[n]\n");
    scanf(" %c",&input);
    if (input == 'y')
    {
        display_recipe_public(recipes[selection]);
    }
}

//특정 확률 분포에 따라 랜덤하게 값을 뽑는다.
int weighted_random_selection(int *values, float *probabilities, int size) 
{
    // 0과 1 사이의 랜덤 값을 생성
    float r = (float)rand() / (float)RAND_MAX;
    
    // 확률 누적 합을 계산
    float cumulative_probability = 0.0;

    for (int i = 0; i < size; i++) 
    {
        cumulative_probability += probabilities[i];
        if (r <= cumulative_probability) 
        {
            return values[i];
        }
    }
    return values[size - 1]; 
}


// 모든 고유 코드를 가져온다.
void return_all_code(darray* filenames)
{
    int i;
    get_file_names("../data/archive",filenames);
    //public.txt 제거
    filenames->count = filenames->count - 1;
    for(i =0;i<filenames->count;i++)
    {
        remove_txt_extension(filenames->arr[i]);
    }
}

//public 코드만 가져온다.
void return_public_code(darray* filenames)
{
    printf("flag2.4");
    int i;
    FILE *file = fopen("../data/archive/public.txt","r");
    if (file == NULL) {
    perror("Failed to open file");
    return 1;
    }
    filenames->count = 0;
    printf("flag2.5");
    darray* public_codes;
    printf("flag2.6");
    //init_darray(public_codes);
    printf("flag2.7");
    char buffer[20];
    printf("flag3");    
    
    while (fgets(buffer, 20, file) != NULL) 
    {
        buffer[strcspn(buffer, "\n")] = '\0'; 
        public_codes->arr[filenames->count] = strdup(buffer);
        if (public_codes->arr[filenames->count] == NULL) {
            perror("error");
            fclose(file);
            for (i = 0; i < filenames->count; i++) {
                free(public_codes->arr[i]);
            }
            free(public_codes->arr);
            return NULL;
        }
        (filenames->count)++;
    }
    filenames->arr = public_codes->arr;
    fclose(file);
}

// 아카이브 디렉토리에서 텍스트 파일 명을 가져온다.
void get_file_names(const char *directoryPath, darray* filenames) 
{
    DIR *dir;
    struct dirent *ent;
    int count = 0;
    
    char **fileNames = malloc(sizeof(char*)*100);
    if (fileNames == NULL) perror("error");
    if ((dir = opendir(directoryPath)) != NULL) 
    {
        while ((ent = readdir(dir)) != NULL) 
        {         
            if (ent->d_type == DT_REG) 
            {
            fileNames[count] = strdup(ent->d_name);
            count++;        
            }
        }  
    }
    closedir(dir);
    filenames->arr = fileNames;
    filenames->count = count;
    free(fileNames);
}

//.txt 확장자를 제거한다 
void remove_txt_extension(char *filename) {
    char *ext = strrchr(filename, '.');
    if (ext != NULL && strcmp(ext, ".txt") == 0) {
        *ext = '\0';  // .txt 부분을 문자열의 끝으로 설정하여 제거
    }
}


 
