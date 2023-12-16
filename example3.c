#include<stdio.h>
#include<sys/types.h>
#include<dirent.h>
#include<string.h>
#include<sys/stat.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_PATH_LENGTH 256


void traverseDirectory(char name[100]){
        
            DIR *dir;
    struct dirent *ent;
    struct stat states;

    dir = opendir(name);
    if (!dir)
{
    perror(name);
    return;
}

    while ((ent = readdir(dir)) != NULL)
    {
        stat(ent->d_name, &states);
        if (!strcmp(".", ent->d_name) || !strcmp("..", ent->d_name))
        {
            continue;
        }
        else
        {
            if (S_ISDIR(states.st_mode))
            {
                strcat(name, "/");
                strcat(name, ent->d_name);
                printf("%s\n", name);
                traverseDirectory(name);
            }
        }
    }

    closedir(dir);
}

int main(){
        //char path[100];
        //printf("Enter the path:\n");
        //scanf("%s",&path);
    char root_path[MAX_PATH_LENGTH];
    printf("Enter the root directory path: ");
    fgets(root_path, sizeof(root_path), stdin);

    // Remove trailing newline character from root_path
    root_path[strcspn(root_path, "\n")] = '\0';
    traverseDirectory(root_path);
}