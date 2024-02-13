#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <dirent.h>
#include <signal.h>
#include <sys/wait.h>
#include <fcntl.h>

#define MAXLEN 1000


int flag = 0;

void usr1 (int x) {
    return;
}

void end(int x) {
    kill(getppid(), SIGUSR1);
    exit(1);
}

void setflag(int x) {
    flag = 1;
}

void ex(int x) {
    exit(1);
}

int read_line(int fd) {
    char c = 'a';
    while(read(fd, &c, 1) && c != '\n') {
        putchar(c);
    }
    putchar('\n');
    if (c != '\n') {
        exit(1);
        return 1;
    } else {
        return 0;
    }


}

int main(int argc, char **argv) {
    pid_t p, par = getpid();
    int f = open(argv[1], O_RDONLY);
    // FILE *f = fopen(argv[1], "r");

    char *str = malloc(MAXLEN);

    signal(SIGUSR1, usr1);

    if ( (p = fork()) == -1) {
        printf("error lol");
        exit(1);
    } else if (p == 0) {
        signal(SIGALRM, end);
        char *s_child = malloc(MAXLEN);
        int fl = 0;
        while(1 && !fl) {
            usleep(100005);
            printf("Son\n");
            fl = read_line(f);
            kill(par, SIGUSR1);
            pause();
        }
    } else {
        signal(SIGCHLD, setflag);
        int fl = 0;
        while(!fl && !flag) {
            usleep(100000);
            printf("Father\n");
            fl = read_line(f);
            kill(p, SIGUSR1);
            pause();
        }   

        // while(1) {
        //     printf("Father\n");
        //     sleep(1);

        // }

        wait(NULL);
        exit(0);
    }
    close(f);
    return 0;



}