#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>


int main(int argc, char **argv) {

    if (argc > 6 || argc < 6){
        printf("incorrect input\n");
        return 0;
    }

    if (!strcmp(argv[argc - 1], "p")) {

        // Запускаем параллельно два сына и смотрим на их статусы по окончанию
        
        pid_t pid1 = fork(), pid2;
        
        if (pid1 == -1) {
            printf("Error occured\n");
            return 0;
        }

        if (pid1 == 0) {                 //  son
            execl(argv[1], argv[1], argv[2], NULL);
            printf("Error occured\n");
            exit(-1);
        }

        pid2 = fork();

        if (pid2 == -1) {
            printf("Error occured\n");
            return 0;
        }

        if (pid2 == 0) {                 //  son
            execl(argv[3], argv[3], argv[4], NULL);
            printf("Error occured\n");
            exit(-1);
        }
        

        int status1, status2;
        waitpid(pid1, &status1, 0);
        waitpid(pid2, &status2, 0);

        if (WIFEXITED(status1)) {
            printf("procces one ended with status = exit(%d)\n", WEXITSTATUS(status1));
        } else {
            printf("process one ended with status = abort()\n");
        }

        if (WIFEXITED(status2)) {
            printf("procces two ended with status = exit(%d)\n", WEXITSTATUS(status2));
        } else {
            printf("process ended with status = abort()\n");
        }

    } else {

        // запускаем сына, ждем окончания, выводим статус, запускаем второго сына
        // ждем окончания, выводим статус

        pid_t pid = fork();
        
        if (pid == -1) {
            printf("Error occured\n");
            return 0;
        }

        if (pid == 0) {                 //  son
            execl(argv[1], argv[1], argv[2], NULL);
            printf("Error occured\n");
            exit(-1);
        }
        int status;
        waitpid(pid, &status, 0);

        if (WIFEXITED(status)) {
            printf("procces one ended with status = exit(%d)\n", WEXITSTATUS(status));
        } else {
            printf("process one ended with status = abort()\n");
        }

        pid = fork();

        if (pid == -1) {
            printf("Error occured\n");
            return 0;
        }

        if (pid == 0) {                 //  son
            execl(argv[3], argv[3], argv[4], NULL);
            printf("Error occured\n");
            exit(-1);
        }
        waitpid(pid, &status, 0);

        if (WIFEXITED(status)) {
            printf("procces two ended with status = exit(%d)\n", WEXITSTATUS(status));
        } else {
            printf("process two ended with status = abort()\n");
        }
    }

    return 0;
}