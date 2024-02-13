#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int requires(const char *str) {
    char pattern[] = "rwxrwxrwx";

    int len = sizeof(pattern);

    if (!str) {
        return -1;
    }

    int req = 0;
    int i = 0;
    while (i < len - 1) {
        req <<= 1;

        if (str[i] == pattern[i]) {
            req = req | 1;
        } else if (str[i] != '-') {
            return -1;
        }

        i++;
    }

    if (str[i]) {
        return -1;
    }

    return req;
}

char* rwx(int r){
    if (r>511 || r<0){
        return NULL;
    }
    char temp[]="rwxrwxrwx";
    char *str=malloc(10);
    for (int i=8; i>=0; i--){
        if (r%2){
            str[i]=temp[i];
        }
        else{
            str[i]='-';
        }

        r/=2;
    } 
    return str;
}

int main(int argc, char **argv) {

    char *str = malloc(30);
    scanf("%s", str);

    int res = requires(str);
    if (res != -1) {
        printf("%o\n", res);
    } else {
        printf("%d\n", -1);
    }

    int r;
    scanf("%d", &r);
    printf("%s\n", rwx(r));

    return 0;
}
