#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
// #define DT_DIR 4       // these things are defined in dirent.h 
// #define DT_LNK 10      // but to ignore the warning in vscode i will define copies of them
#define MY_DIR 4
#define MY_LNK 10
#define LEN 1000


void getdir(char *name) {

    char *str_link = malloc(LEN);                   // str for link files
    char *str_name = malloc(LEN);                   // str for current path

    DIR *dir = opendir(name);

    if (!dir) {
        return;
    }

    struct dirent *dr;
    struct stat s;

    printf("DIR  %s\n", name);
    printf("{\n");

    while (dr = readdir(dir)) {
        if (strcmp(dr -> d_name, "..") && strcmp(dr -> d_name, ".")) {
            strcpy(str_name, name);
            strcat(str_name, "/");                  // create
            strcat(str_name, dr -> d_name);         // new extended path
            if (dr -> d_type == MY_DIR) {
                getdir(str_name);
            } else {
                lstat(str_name, &s);
                if (dr -> d_type == MY_LNK) {
                    int r;
                    printf("\tLINK\t");
                    r = readlink(dr -> d_name, str_link, LEN);
                    printf("%s\n", str_link);

                } else {
                    if (s.st_mode & 0777 & 0110 == 0110) {  // check file rights
                        printf("\tFILE");
                        printf("\t%s\n", dr -> d_name);
                    }            
                }
            }
        } 
    }
    printf("}\n");
    closedir(dir);
    return;
}

int main() {

    getdir(".");

    return 0;
}