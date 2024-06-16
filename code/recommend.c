#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <errno.h>
#include "./headers/sign.h"
#include "./headers/basic_funcs.h"
#include "./headers/recipe_utils.h"
#include "./headers/global_vars.h"
#include "./headers/structs.h"

//recommend에서 입력으로 어떤 종류의 추천으로 입력받을지 확인한다.
void recommend();

// 명령어 도움 - "help" 라고 입력하면 각각의 옵션에 대해 도움을 준다. 
void help();
// 레시피를 칼로리 순으로 정렬
void sort_calories();
void calc_calories(RECIPE*, float*);

// 레시피를 단백질 순으로 정렬
void sort_protein();
void calc_protein(RECIPE*, float*);

// 레시피를 지방 순으로 정렬
void sort_fat();
void calc_fat(RECIPE*, float*);

// 레시피를 저탄고지 순으로 정렬
void lcar_hfat();
void calc_car(RECIPE*,float*);

// 입력값을 기반으로 오늘의 메뉴 추천
void today_menu();
//특정 확률 분포에 따라 랜덤하게 값을 뽑는다.
int weighted_random_selection(int *,float *,int);

// 코드로 파일 명 가져오기, public.txt에 있는 고유 코드들로 레시피 가져오기 
void return_all_code(darray* );
void return_public_code(darray* );
void get_file_names(const char*, darray*);
void remove_txt_extension(char *);

//recommend에서 입력으로 어떤 종류의 추천으로 입력받을지 확인한다.
void recommend()
{
    char command[MAX_LENGTH];
    while(1)
        {
        // 무슨 옵션을 입력받을지 질문 
        refresh_print("input your options. input \"help\" if you need help.\n");
        scanf(" %s",command);
        // strcmp로 입력한 커맨드가 각 키워드와 동일한지 확인. 동일하면 함수 실행
        if(strcmp(command, ":q") == 0) break;
        if(strcmp(command, "help") == 0) 
            {
            help();
            break;
            }
        if(strcmp(command, "cal") == 0) 
            {
            sort_calories(); 
            break;
            }
        if(strcmp(command, "protein") == 0) 
            {
            sort_protein(); 
            break;
            }
        if(strcmp(command, "fat") == 0) 
            {
            sort_fat();
            break;
            }
        if(strcmp(command, "lcar_hfat") == 0) 
            {
            lcar_hfat(); 
            break;
            }
        if(strcmp(command, "today_menu") == 0) 
            {
            today_menu();
            break;
            }
        }
}
// 명령어 도움 - "help" 라고 입력하면 각각의 옵션에 대해 도움을 준다. 
void help()
{
    int check = 1;
    char input,pass;
    // 'o'를 입력하면 전 화면으로 돌아가야 하므로 check=1이면 도움말 출력, 'o'입력하면 check=0 적용
    while(check == 1)
        {
        // 사용자가 입력 가능한 옵션과 키워드에 대해 설명
        // q,c,o,f,k,t 등이 있음
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

// 레시피를 칼로리 순으로 정렬
void sort_calories()
{
    // 각종 int 변수, public.txt에 있는 파일 이름을 저장하는 darray* filenames 정의
    int i,j;
    int index,num;
    char input;
    darray* filenames;
    // filenames에 public.txt에 있는 고유 코드 저장 
    return_public_code(filenames);
    // recipes 객체, calories, argsort 정의 
    RECIPE recipes[filenames->count];
    float* calories = malloc(sizeof(float) * filenames->count);
    float* argsort = malloc(sizeof(float) * filenames->count);
    j = filenames->count;

    // recipes 객체의 각 원소에 filenames의 고유 코드를 읽어와 레시피를 저장
    // 그에 대응되는 calories의 인덱스에 각 레시피의 칼로리 계산하여 저장
    for(i=0;i<j;i++)
        {
        init_recipe(recipes+i);
        
        read_recipe_from_file(filenames->arr[i],&recipes[i]);
        
        calories[i] = 0;
        
        calc_calories(&recipes[i],&(calories[i]));
        }
    
    // 이후 argsort에 각 인덱스 위치에 대응되는 레시피의 칼로리 순위 저장 
    for(i=0;i<filenames->count;i++)
        {
        argsort[i] = 0;
        for(j=0;j<filenames->count;j++)
            {
            // 각 순위를 0으로 저장하고, 자신보다 작은 것이 있을 때마다 값이 1씩 증가
            if(calories[j]<calories[i])
                {
                argsort[i]++;
                }
            }
        }
    //높은 순으로, 낮은 순으로 볼지 입력
    printf("please input options, high calories[h], low calories[l]\n");
    scanf(" %c",&input);
    //앞에서 결정한 순서에 따라 몇 번째 순위의 레시피를 볼지 입력
    printf("please input number of index you want from 1 to %d\n",filenames->count);
    scanf("%d",&index);
    num = (input=='h')? (filenames->count -1 - (index-1)) : (index-1);
    for(i=0;i<filenames->count;i++)
        {
        if(argsort[i]==num)
            {
            // 칼로리를 출력하고, 레시피를 확인할지 말지 입력
            printf("Calorie of the recipe is %lf.\nWill you go to check the recipe? yes[y] no[n]\n",calories[i]);
            scanf(" %c",&input);
            if( input == 'y')
                {
                display_recipe_public(&recipes[i]);
                }
            }
        }
    //할당된 메모리 free후 함수 종료 
    free_darray(filenames);
    filenames = NULL;
    free(calories);
    free(argsort);
}

//레시피의 칼로리를 계산한다. 
void calc_calories(RECIPE* recipe,float* calorie)
{
    ingre_darray ingre_arr = recipe->ingre_arr;
    struct ingre ingredient;
    int i;
    //calorie의 위치에 주어진 레시피의 재료 정보를 확인하여 계산한 칼로리 값을 저장 
    for(i=0;i<ingre_arr.count;i++)
        {
        ingredient = *(ingre_arr.arr[i]);
        *calorie = *calorie + ingredient.nutrient.arr[0][0] * (*ingredient.quantity);
        }
}

// 레시피를 단백질 순으로 정렬
void sort_protein()
{
    // 각종 int 변수, public.txt에 있는 파일 이름을 저장하는 darray* filenames 정의
    int i,j;
    int index,num;
    char input;
    darray* filenames;
    // filenames에 public.txt에 있는 고유 코드 저장 
    return_public_code(filenames);
    // recipes 객체, calories, argsort 정의
    RECIPE recipes[filenames->count];
    float* protein = malloc(sizeof(float) * filenames->count);
    float* argsort = malloc(sizeof(float) * filenames->count);
    // 임시로 int 변수 j에 filenames->count 값 저장 
    j = filenames->count;

    // recipes 객체의 각 원소에 filenames의 고유 코드를 읽어와 레시피를 저장
    // 그에 대응되는 protein의 인덱스에 각 레시피의 단백질 양을 계산하여 저장
    for(i=0;i<j;i++)
        {
        //recipe 객체의 각 성분에 filenames에 저장되어 있는 코드 명의 레시피를 순서대로 저장
        init_recipe(recipes+i);
        
        read_recipe_from_file(filenames->arr[i],&recipes[i]);
        
        protein[i] = 0;
        
        calc_protein(&recipes[i],&(protein[i]));
        }
    
    // 이후 argsort에 각 인덱스 위치에 대응되는 레시피의 단백질 양 순위 저장 
    for(i=0;i<filenames->count;i++)
        {
        argsort[i] = 0;
        for(j=0;j<filenames->count;j++)
            {
            // 각 순위를 0으로 저장하고, 자신보다 작은 것이 있을 때마다 값이 1씩 증가
            if(protein[j]<protein[i])
                {
                argsort[i]++;
                }
            }
        }
    //높은 순으로, 낮은 순으로 볼지 입력
    printf("please input options, high protein[h], low protein[l]\n");
    scanf(" %c",&input);
    //앞에서 결정한 순서에 따라 몇 번째 순위의 레시피를 볼지 입력
    printf("please input number of index you want from 1 to %d\n",filenames->count);
    scanf("%d",&index);
    num = (input=='h')? (filenames->count -1 - (index-1)) : (index-1);
    for(i=0;i<filenames->count;i++)
        {
        if(argsort[i]==num)
            {
            // 칼로리를 출력하고, 레시피를 확인할지 말지 입력
            printf("Protein of the recipe is %lf.\nWill you go to check the recipe? yes[y] no[n]\n",protein[i]);
            scanf(" %c",&input);
            if( input == 'y')
                {
                //선택한 레시피 터미널에 디스플레이 
                display_recipe_public(&recipes[i]);
                }
            }
        }
    //할당된 메모리 free후 함수 종료 
    free_darray(filenames);
    filenames = NULL;
    free(protein);
    free(argsort);
}

//레시피의 단백질 양을 계산한다. 
void calc_protein(RECIPE* recipe,float* protein)
{
    ingre_darray ingre_arr = recipe->ingre_arr;
    struct ingre ingredient;
    int i;
    //float 포인터 protein의 위치에 매개변수 레시피의 재료 정보를 확인하여 총 단백질 양을 합하여 저장
    for(i=0;i<ingre_arr.count;i++)
        {
        ingredient = *(ingre_arr.arr[i]);
        *protein = *protein + ingredient.nutrient.arr[1][0] * (*ingredient.quantity);
        }
}

// 레시피를 지방 순으로 정렬
void sort_fat()
{
    // 각종 int 변수, public.txt에 있는 파일 이름을 저장하는 darray* filenames 정의
    int i,j;
    int index,num;
    char input;
    darray* filenames;
    // filenames에 public.txt에 있는 고유 코드 저장 
    return_public_code(filenames);
    // recipes 객체, calories, argsort 정의
    RECIPE recipes[filenames->count];
    float* fat = malloc(sizeof(float) * filenames->count);
    float* argsort = malloc(sizeof(float) * filenames->count);
    // 임시로 int 변수 j에 filenames->count 값 저장 
    j = filenames->count;

    // recipes 객체의 각 원소에 filenames의 고유 코드를 읽어와 레시피를 저장
    // 그에 대응되는 protein의 인덱스에 각 레시피의 지방 양을 계산하여 저장
    for(i=0;i<j;i++)
        {
        //recipe 객체의 각 성분에 filenames에 저장되어 있는 코드 명의 레시피를 순서대로 저장
        init_recipe(recipes+i);
        
        read_recipe_from_file(filenames->arr[i],&recipes[i]);
        
        fat[i] = 0;
        
        calc_fat(&recipes[i],&(fat[i]));
        }
    
    // 이후 argsort에 각 인덱스 위치에 대응되는 레시피의 지방 양 순위 저장
    for(i=0;i<filenames->count;i++)
        {
        argsort[i] = 0;
        for(j=0;j<filenames->count;j++)
            {
            // 각 순위를 0으로 저장하고, 자신보다 작은 것이 있을 때마다 값이 1씩 증가
            if(fat[j]<fat[i])
                {
                argsort[i]++;
                }
            }
        } 
    //높은 순으로, 낮은 순으로 볼지 입력
    //refresh_print 사용 고려 
    printf("please input options, high fat[h], low fat[l]\n");
    scanf(" %c",&input);
    //앞에서 결정한 순서에 따라 몇 번째 순위의 레시피를 볼지 입력
    printf("please input number of index you want from 1 to %d\n",filenames->count);
    scanf("%d",&index);
    num = (input=='h')? (filenames->count -1 - (index-1)) : (index-1);
    for(i=0;i<filenames->count;i++)
        {
        if(argsort[i]==num)
            {
            // 지방 양을 출력하고, 레시피를 확인할지 말지 입력
            printf("Fat of the recipe is %lf.\nWill you go to check the recipe? yes[y] no[n]\n",fat[i]);
            scanf(" %c",&input);
            if( input == 'y')
                {
                //선택한 레시피 터미널에 디스플레이
                display_recipe_public(&recipes[i]);
                }
            }
        }
    //할당된 메모리 free후 함수 종료
    free_darray(filenames);
    filenames = NULL;
    free(fat);
    free(argsort);
}


//레시피의 지방 양을 계산한다. 
void calc_fat(RECIPE* recipe,float* fat)
{
    ingre_darray ingre_arr = recipe->ingre_arr;
    struct ingre ingredient;
    int i;
    //float 포인터 fat의 위치에 매개변수 레시피의 재료 정보를 확인하여 총 지뱡 양을 합하여 저장
    for(i=0;i<ingre_arr.count;i++)
        {
        ingredient = *(ingre_arr.arr[i]);
        *fat = *fat + ingredient.nutrient.arr[2][0] * (*ingredient.quantity);
        }
}

// 레시피를 저탄고지 순으로 정렬. 기준은 알려져 있는 탄수화물의 열량에서 지방의 열량을 뺀 것이다. 
void lcar_hfat()
{
    //각종 변수들와 public.txt에서의 고유 코드를 저장하는 filenames 정의
    int i,j;
    int index,num;
    char input;
    darray* filenames;
    //filenames에 고유 코드 저장
    return_public_code(filenames);
    //고유 코드의 레시피를 저장하는 recipes 객체, 물리량을 저장하는 car_fat, car, fat와 순서를 저장하는 argsort 정의
    RECIPE recipes[filenames->count];
    float* car_fat = malloc(sizeof(float) * filenames->count);
    float* argsort = malloc(sizeof(float) * filenames->count);
    float* car = malloc(sizeof(float) * filenames->count);
    float* fat = malloc(sizeof(float) * filenames->count);

    for(i=0;i<filenames->count;i++)
        {
        //recipes객체의 각 인덱스에 filenames의 고유 코드에 대응되는 레시피 저장 
        init_recipe(recipes+i);
        read_recipe_from_file(filenames->arr[i],&recipes[i]);

        // calc_car와 calc_fat으로 탄수화물과 지방 양을 계산하고, 차를 구하여 car_fat에 저장
        car_fat[i] = 0;
        calc_car(&recipes[i],&(car[i]));
        calc_fat(&recipes[i],&(fat[i]));
        car_fat[i] = car[i]*4 - fat[i]*9;
        }
    // car,fat 배열에 동적할당된 데이터 free
    free(car);
    free(fat);
    // 이후 argsort에 각 인덱스 위치에 대응되는 레시피의 저탄고지 순위 저장
    for(i=0;i<filenames->count;i++)
        {
        argsort[i] = 0;
        for(j=0;j<filenames->count;j++)
            {
            // 각 순위를 0으로 저장하고, 자신보다 작은 것이 있을 때마다 값이 1씩 증가
            if(car_fat[j]<car_fat[i])
                {
                argsort[i]++;
                }
            }
        }
    //높은 순으로, 낮은 순으로 볼지 입력
    refresh_print("please input options, high car_fat[h], low car_fat[l]\n");
    printf("high car_fat meant higher carbohydrate, lower fat\n");
    scanf(" %c",&input);
    //앞에서 결정한 순서에 따라 몇 번째 순위의 레시피를 볼지 입력
    printf("please input number of index you want from 1 to %d\n",filenames->count);
    scanf("%d",&index);
    num = (input=='h')? (filenames->count -1 - (index-1)) : (index-1);
    for(i=0;i<filenames->count;i++)
        {
        if(argsort[i]==num)
            {
            // 저탄고지 지수를 출력하고, 레시피를 확인할지 말지 입력
            printf("car_fat of the recipe is %lf.\nWill you go to check the recipe? yes[y] no[n]\n",car_fat[i]);
            scanf(" %c",&input);
            if( input == 'y')
                {
                //선택한 레시피 터미널에 디스플레이
                display_recipe_public(&recipes[i]);
                }
            }
        }
    //할당된 메모리 free후 함수 종료
    free_darray(filenames);
    filenames = NULL;   
    free(car_fat);
    free(argsort);
}

//레시피의 탄수화물 양을 계산한다.
void calc_car(RECIPE* recipe,float* car)
{
    ingre_darray ingre_arr = recipe->ingre_arr;
    struct ingre ingredient;
    int i;
    //float 포인터 car의 위치에 매개변수 레시피의 재료 정보를 확인하여 총 탄수화물 양을 합하여 저장
    for(i=0;i<ingre_arr.count;i++)
        {
        ingredient = *(ingre_arr.arr[i]);
        *car = *car + ingredient.nutrient.arr[3][0] * (*ingredient.quantity);
        }
}

// 입력값을 기반으로 오늘의 메뉴 추천 
void today_menu()
{
    //각종 변수 정의, public.txt에 있는 고유 코드를 저장하는 filenames 정의
    int index,num,counter,i,j,temp_count,check;
    char input;
    darray* filenames;
    init_darray(filenames);
    //추후 입력한 옵션들을 저장함
    char option[100];
    //filenaems에 고유 코드 저장
    return_public_code(filenames);
    //filenames->count 임시 저장
    counter = filenames->count;
    // 레시피를 저장하는 recipes 객체 정의
    RECIPE* recipes = malloc(sizeof(RECIPE) * counter);
    // 주어진 조건들에 대해서 선택인 인덱스를 저장하는 selected_index
    // 레시피의 좋아요와 댓글 수를 저장하는 likes와 comments 객체 정의
    int* selected_index = malloc(sizeof(int) * counter);
    int* likes = malloc(sizeof(int) * counter);
    int* comments = malloc(sizeof(int) * counter);
    
    for(i=0;i<counter;i++)
        {
        // recipes의 각 객체에 filenames의 고유 코드에 대응되는 레시피 저장
        init_recipe(recipes+i);
        read_recipe_from_file(filenames->arr[i],recipes+i);
        }
    for(i=0;i<counter;i++)
        {
        // 각 레시피 객체의 좋아요와 댓글 수 저장 
        selected_index[i] = 1;
        likes[i] = recipes[i].likes;
        comments[i] = recipes[i].comments.count;
        }
    // 원하는 태그 옵션 입력
    refresh_print("input options of tags. You can keep input tags and input \"stop\" to finish it.\n");
    while(1)
        {
        scanf("%s",option);
        // stop을 입력했을 경우 정지
        if(strcmp(option,"stop")==0)
            {
            break;
            }
        // 아닌 경우 레시피들에서 그 태그가 있는 것을 남기고 없다면 그 인덱스의 selected_index value를 0으로 수정
        for(i=0;i<counter;i++)
            {
            // 기본 check 값이 0
            check = 0;
            temp_count = recipes[i].tag.count;
            for(j=0;j<temp_count;j++)
                {
                if(strcmp(option,recipes[i].tag.arr[j]) == 0)
                    {
                    //만약 있다면 check = 1로 변경
                    check = 1;
                    }
                }
            //check 값이 0인 경우 selected_index 값을 0으로 변경 
            if(check==0)
                {
                selected_index[i] = 0;
                }
            }
        }
    //원하는 재료 옵션 입력 
    refresh_print("input options of ingredients. You can keep input ingredients and input \"stop\" to finish it.\n");
    while(1)
        {
        scanf("%s",option);
        // stop을 입력했을 경우 정지
        if(strcmp(option,"stop")==0)
            {
            break;
            }
        // 아닌 경우 레시피들에서 그 재료가 있는 것을 남기고 없다면 그 인덱스의 selected_index value를 0으로 수정
        for(i=0;i<counter;i++)
            {
            // 기본 check 값이 0
            check = 0;
            temp_count = recipes[i].ingre_arr.count;
            for(j=0;j<temp_count;j++)
                {
                if(strcmp(option, recipes[i].ingre_arr.arr[j]->name) == 0)
                    {
                    //만약 있다면 check = 1로 변경
                    check = 1;
                    }
                }
            //check 값이 0인 경우 selected_index 값을 0으로 변경 
            if(check==0)
                {
                selected_index[i] = 0;
                }
            }
        }
    //이 아래의 코드는 지수분포로 랜덤하게 레시피를 뽑는 과정
    // temp_count, values probabilities 등 정의
    temp_count = 0;
    int* values = malloc(sizeof(int) * counter);
    float* probabilities = malloc(sizeof(float) * counter);
    float sum;
    int selection;
    //전체 레시피에서 selected_index가 1인 경우만 지수분포 확률 값을 저장
    for(i=0;i<counter;i++)
        {
        if(selected_index[i]==1)
            {
            values[temp_count] = i;
            // 좋아요는 0.5, 댓글은 1로 가중치가 맞춰져 exp 함수를 이용해 확률 결정
            probabilities[i] = exp(likes[i]*0.5 + comments[i]);
            //sum에 확률 합 저장
            sum = sum + probabilities[i];
            temp_count++;
            }
        }
    //이후 sum 값이 기반으로 확률 합이 1이 되도록 정규화
    for(i=0;i<temp_count;i++)
        {
        probabilities[i] = probabilities[i] / sum;
        }
    //이후 weighted_random_selection으로 무작위하게 값 하나 결정
    selection = weighted_random_selection(values,probabilities, temp_count);
    //뽑힌 인덱스와 고유 코드 이름 출력하고 레시피 열람 여부 입력
    printf("%d is the index, and code is %s\n",selection, filenames->arr[selection]);
    printf("input to see the recommended recipe. yes[y] no[n]\n");
    scanf(" %c",&input);
    if (input == 'y')
        {
        //레시피를 터미널에 디스플레이 
        display_recipe_public(&recipes[selection]);
        }
}

//특정 확률 분포에 따라 랜덤하게 값을 뽑는다.
int weighted_random_selection(int *values, float *probabilities, int size) 
{
    // 0과 1 사이의 랜덤 값을 생성
    float r = (float)rand() / (float)RAND_MAX;
    
    float cumulative_probability = 0.0;

    // 확률 누적 합을 계산
    for (int i = 0; i < size; i++) 
        {
        cumulative_probability += probabilities[i];
        // 누적 확률 분포를 기반으로 r이 어느 위치에 있는지 확인하고 그 값 리턴
        if (r <= cumulative_probability) 
            {
            return values[i];
            }
        }
    // 앞에서 리턴되지 않았을 경우 마지막 값 리턴 
    return values[size - 1]; 
}


// 모든 고유 코드를 가져온다.
void return_all_code(darray* filenames)
{
    int i;
    // 특정 위치에 있는 파일들의 이름 가져오기 
    get_file_names("../data/archive",filenames);
    // 이 함수에 public.txt가 저장되어 있어 이것 제외 
    filenames->count = filenames->count - 1;
    for(i =0;i<filenames->count;i++)
        {
        // txt 확장자 제거 
        remove_txt_extension(filenames->arr[i]);
        }
}

//public.txt에 있는 코드만 가져온다.
void return_public_code(darray* filenames1)
{
    int i;
    // 파일 포인터 열기
    FILE *file1 = fopen("../data/archive/public.txt","rt");
    // public_codes와 buffer 객체 정의 
    char public_codes[20][20];
    char buffer[20];
    //오류 제외
    if (file1 == NULL) 
        {
    printf("file open error: %d\n",errno);
    exit(-1);
        }
    //filenames1에 코드 값들을 저장, count는 기본 0, arr에 동적 할당
    filenames1->count = 0;
    filenames1->arr = malloc(sizeof(char*)* 20);
    //fgets로 NULL이 오기 전까지 계속 buffer를 한줄씩 받아옴
    while (fgets(buffer, 20, file1) != NULL) 
        {
        // 끝의 개행 문자를 NULL로 대체 
        buffer[strcspn(buffer, "\n")] = '\0'; 
        // filenames1->arr에 동적 할당
        filenames1->arr[filenames1->count] = (char*)malloc(sizeof(char)*10);
        // strcpy로 저장 
        strcpy(filenames1->arr[filenames1->count],buffer);
        // 오류 제외 
        if (public_codes[filenames1->count] == NULL) 
            {
            perror("error");
            fclose(file1);
            for (i = 0; i < filenames1->count; i++) 
                {
                free(public_codes[i]);
                }
            free(public_codes);
            }
        // 하나가 저장될 때마다 count값 1 증가 
        (filenames1->count)++;
        }
    //파일 열기 종료 
    fclose(file1);
}

// 아카이브 디렉토리에서 텍스트 파일 명을 가져온다.
// 다른 파일에 있는 동일한 이름의 함수가 오류가 있어 수정한 것 
void get_file_names(const char *directoryPath, darray* filenames) 
{
    DIR *dir;
    struct dirent *ent;
    int count = 0;
    //fileNames 객체 정의 
    char **fileNames = malloc(sizeof(char*)*100);
    // 오류 제외
    if (fileNames == NULL) 
        perror("error");
    // 오류 제외
    if ((dir = opendir(directoryPath)) != NULL) 
        {
        while ((ent = readdir(dir)) != NULL) 
            {         
            if (ent->d_type == DT_REG) 
                {
            // fileNames[count]에 파일 명을 저장하고, count 1 증가
            fileNames[count] = strdup(ent->d_name);
            count++;        
                }
            }  
        }
    //filenames에 각 값을 저장하고, dir 종료 및 동적할당 free 
    closedir(dir);
    filenames->arr = fileNames;
    filenames->count = count;
    free(fileNames);
}

//.txt 확장자를 제거한다 
void remove_txt_extension(char *filename) 
{
    char *ext = strrchr(filename, '.');
    if (ext != NULL && strcmp(ext, ".txt") == 0) 
        *ext = '\0';  // .txt 부분을 문자열의 끝으로 설정하여 제거
} 


 
