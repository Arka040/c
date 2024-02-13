#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>


char *reqs(int n) {

    if (n > 511) {
        return NULL;
    }

    char *s = malloc(10);
    s[9] = '\0';
    int d = 0;
    for (int i = 3; i >= 0; i--) {
        d = n % 8;
        n /= 8;

        if (d % 2 == 0) {
            s[i * 3 - 1] = '-';
        } else {
            s[i * 3 - 1] = 'x';
        }

        if ((d - 4) >= 0) {
            s[i * 3 - 3] = 'r';
        } else {
            s[i * 3 - 3] = '-';
        }

        if (d == 2 || d == 3 || d == 6 || d == 7) {
            s[i * 3 - 2] = 'w';
        } else {
            s[i * 3 - 2] = '-';
        }


    }

    return s;
}


int main() {


    DIR *dir = opendir(".");

    if (!dir) {
        return 0;
    }

    struct dirent *dr;
    struct stat stat1;

    

    while(dr = readdir(dir)) {
        printf("%s ", dr->d_name);
        lstat(dr->d_name, &stat1);
        printf("%ld ", dr->d_ino);

        if(S_ISREG(stat1.st_mode))
            {
                printf("REGULAR ");
            }
            else if(S_ISDIR(stat1.st_mode))
            {
                printf("DIR     ");
            }
            else if(S_ISCHR(stat1.st_mode))
            {
                printf("SYM_div ");
            }
            else if(S_ISBLK(stat1.st_mode))
            {
                printf("B_div   ");
            }
            else if(S_ISFIFO(stat1.st_mode))
            {
                printf("FIFO_CH ");
            }
            else if(S_ISLNK(stat1.st_mode))
            {
                printf("SYM_PNT ");
            }
            else if(S_ISSOCK(stat1.st_mode))
            {
                printf("SOCKET  ");
            }
            else
            {
                printf("UNKNOWN ");
            }

        printf("%s", reqs(dr->d_ino % 512));

        printf("\n");
    }
    return 0;
}
