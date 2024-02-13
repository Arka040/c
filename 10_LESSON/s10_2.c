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

int bit;
pid_t p1, p2, par;


void send_0(int x) {
    bit = 0;
    return;
}

void send_1(int x) {
    bit = 1;
    return;
}

void end(int x) {
    printf("\n");
    exit(1);
}

void back(int x) {
    kill(p2, SIGALRM);
}

void n(int x) {

}

int main(int argc, char **argv) {
    int f = open(argv[1], O_RDONLY);

    signal(SIGALRM, back);

    if ( (p1 = fork()) == -1) {
        printf("haha error\n");
    } else if (p1 == 0) {               // first son
        int ch = 0;
        int fl = 0;
        signal(SIGUSR1, send_0);
        signal(SIGUSR2, send_1);
        signal(SIGIO, end);
        while (1) {
            pause();

            // if (fl) {
            //     kill(getppid(), SIGALRM);
            // }
            fl = 1;
            ch = 0; 
            for (int i = 0; i < 8; i++) {
                ch = ch * 2 + bit;
                if (i != 7) {
                    kill(getppid(), SIGALRM);
                }   
            }
            printf("%c", ch);
            kill(getppid(), SIGALRM);

        }
        exit(1);
    }

    if ( ( p2 = fork()) == -1) {
        printf("haha Erorr\n");
    } else if (p2 == 0) {               // second son
        char c;
        signal(SIGALRM, n);

        while (read(f, &c, 1)) {
            for (int i = 7; i >=0; i--) {
                if (((int)(c >> i) & 1) == 1) {
                    kill(p1, SIGUSR2);
                } else {
                    kill(p1, SIGUSR1);
                }
                pause();
            }
        }
        kill(p1, SIGIO);
        exit(1);
    }
    waitpid(p1, NULL, 0);
    waitpid(p2, NULL, 0);

    close(f);

    return 0;
}