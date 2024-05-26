#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
void low_calories();
void return_all_code(darray* );
void return_public_code(darray* );
void get_file_names(const char*, darray*);
void remove_txt_extension(char *);


void recommend()
{
    char command[MAX_LENGTH];
    while(1)
    {   
        printf("input your options.\n");
        scanf("%s",command);
        if(strcmp(command, ":q") == 0) break;
        if(strcmp(command, "low_cal") == 0) low_calories();

    }
}

void low_calories()
{
    int i;
    darray* filenames;
    return_public_code(filenames);
    for(i=0;i<filenames->count;i++)
    {
        printf("%s\n",filenames->arr[i]);
    }
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
    int i;
    FILE *file;
    char **public_codes = malloc(sizeof(char*) * 100);
    char buffer[20];
    if (public_codes == NULL) perror("error");    
    file = fopen("../data/archive/public.txt", "r");
    filenames->count = 0;
    while (fgets(buffer, 10, file) != NULL) 
    {
        buffer[strcspn(buffer, "\n")] = '\0';
        public_codes[filenames->count] = strdup(buffer);
        if (public_codes[filenames->count] == NULL) {
            perror("error");
            fclose(file);
            for (i = 0; i < filenames->count; i++) {
                free(public_codes[i]);
            }
            free(public_codes);
            return NULL;
        }
        (filenames->count)++;
    }
    filenames->arr = public_codes;
    free(public_codes);
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



