#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#define LEN 100

// передеать через угловую скобочку < файл с входными данными  
// (./program.c output_file.txt <input_file.txt)
// in input_file last line must be like: r'\w\s(exit)'

int main(int argc, char **argv) {

    FILE *f = fopen(argv[1], "r+");
    
    int fd1[2], fd2[2];
    pipe(fd1);                    // main pipe
    pipe(fd2);                    // pipe to return back
    pid_t p1;

    if( (p1 = fork()) < 0 ){
        printf("Error occured");
    }
    else if (p1 == 0) {         // son

        close(fd1[1]);          // close write main pipe
        close(fd2[0]);          // close read return back pipe

        char *str = malloc(LEN);

        while(read(fd1[0], str, LEN)) {
            if (!strcmp(str, "exit")){
                close(fd1[0]);
                close(fd2[1]);
                exit(0);
            }

            int length = strlen(str);
            int count = 0;

            for (int i = 0; i < length - 1; i++) {
                if (str[i] == str[length - 1]) {
                    count ++;
                }
            }

            fprintf(f, " %d\n", count);
            fflush(f);
            write(fd2[1], str, LEN);    // return back to parent process
            
        }

        close(fd1[0]);      
        close(fd2[1]);
        

    } else {                        // parent

        close(fd1[0]);
        close(fd2[1]);

        char *c = malloc(2);
        char *str = malloc(LEN);

        scanf("%s", c);
        scanf("%s", str);

        while(strcmp(str, "exit")) {
            fputs(str, f);
            fprintf(f, "%c",' ');
            fputs(c, f);
            fflush(f);
 
            strcat(str, c);

            write(fd1[1], str, LEN);    // write our concatenated string with char at the end in main pipe

            read(fd2[0], str, LEN);     // retrun back to son process

            scanf("%s", c);
            scanf("%s", str);
        }

        write(fd1[1], str, LEN);

        close(fd1[1]);
        close(fd2[0]);
        fclose(f);
    }

    return 0;
}
