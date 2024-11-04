#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/file.h>
#include <sys/wait.h>
#include <signal.h>
#include <string.h>

// константы для контроля ввода
#define BUF 128
#define DEF4 4 // для увеличения размера массивов
#define MAX_COM 100

// для заполнения списка
enum operations
{
    START,
    STOP, 
    ENTER,
	AMPERSAND,
	DOUBLE_AMPERSAND, 
	SEMICOLON, 
	STICK, 
	DOUBLE_STICK, 
	MORE, 
	LESS, 
	DOUBLE_MORE, 
	LEFT_BRACKET, 
	RIGHT_BRACKET, 
    WORD
};

// прототипы для функций рекурсивного спуска
int com_sh(int , int );
int com_if(int , int );
int com_(int , int );

// список
char **lst; // список слов(команд)
char *buf; // буффер
int c; // текущий символ
int size_buf; // размер текущего слова
int size_list; // размер списка
int cur_list; // элементов в списке
int cur_buf; // элементов в буфере
int arr[MAX_COM]; // массив номеров команд

//---------Функции для работы со списком-----------
// печать
void print_list() {
    int i;
    if (lst == NULL){
        return;
    }
    for ( i = 0; i < size_list - 1; i++){
       printf("%s\n", lst[i]);
    }
	return;
}

// добавление элементов в буфер
void add_sym(){
    if (cur_buf > size_buf - 1){
        size_buf += DEF4;
        buf = realloc(buf,size_buf);
    } 
    buf[cur_buf] = c;
    cur_buf++;
    return;
}

// добавление элемента в список
void add_word(){
    if (cur_buf > size_buf - 1){
        size_buf +=1;
        buf = realloc(buf,size_buf);
    } 
    buf[cur_buf] = '\0';
    if (cur_list > size_list - 1){
        size_list += DEF4;
        lst = realloc(lst, sizeof(*lst)*size_list);
    }
    lst[cur_list] = (char *) malloc (size_buf);
    strcpy(lst[cur_list], buf);
    cur_list++;

    return;
}

// очистка памяти
void clean_list(){
    int i = 0;
    size_list = 0;
    cur_list = 0;
    if (lst == NULL){
        return;
    }
    while( lst[i]!= NULL){
        free(lst[i]);
        i++;
    }
    free(lst);
    lst = NULL;
}

// инициализация списка
void null_list(){
    size_list = 0;
    cur_list = 0;
    lst = NULL;
}

void null_buf(){
    buf = NULL;
    size_buf = 0;
    cur_buf = 0;
}

// проверка на правильность символа
int sym_err(int c){
    return (c!= '\n') && (c!= ' ')&& (c!= '(') && (c!= '>') 
    && (c!= EOF)&&(c!= '&')&&(c!= ';')&&(c!='|') && (c != ')');
}

// регулируем размер списка
void term_list(){
    if (lst == NULL){
        return;
    }
    if (cur_list > size_list - 1){
        lst = realloc(lst, sizeof(*lst)*(size_list + 1));
    }
    lst[cur_list] = NULL;
    size_list = cur_list + 1;
    lst = realloc(lst, sizeof(*lst)*size_list);
    return;
}

// создание списка
void create_list(){
    enum operations top = START;
    c = getchar();
    null_list();
    while (1){
        switch(top){
        case START:
            if (c == ' '){
                c = getchar();
            }
            else if (c == EOF){
                term_list();
                clean_list();
                top = STOP;
                exit(0); // выход по EOF(ctrl+d)
            }
            else if (c == '\n'){
                term_list();
                top = STOP;
            }
            else {
                null_buf();
                add_sym();
                if (c == '&'){
                    top = AMPERSAND;
                }
                else if (c == '>'){
                    top = MORE;
                }
                else if (c == '|'){
                    top = STICK;
                }
                else if (c == ';'){
                    top = SEMICOLON;
                }
                else if (c == '<'){
                    top = LESS;
                }
                else if (c == '('){
                    top = LEFT_BRACKET;
                }
                else if (c == ')'){
                    top = RIGHT_BRACKET;
                }
                else {
                    top = WORD;
                }
                c = getchar();
            }
        break;
        case WORD:
            if (sym_err(c)){
                add_sym();
                c = getchar();
            }
            else {
                top = START;
                add_word();
            }
        break;

        case MORE:
            if (c == '>'){
                add_sym();
                c = getchar();
                top = DOUBLE_MORE;
            }
            else {
                top = START;
                add_word();
            }
        break;
        case DOUBLE_MORE:
               top = START;
                add_word();
        break;

        case AMPERSAND:
            if (c == '&'){
                add_sym();
                c = getchar();
                top = DOUBLE_AMPERSAND;
            }
            else {
                top = START;
                add_word();
            }
        break;
        case DOUBLE_AMPERSAND:
               top = START;
                add_word();
        break;

        case STICK:
            if (c == '|'){
                add_sym();
                c = getchar();
                top = DOUBLE_STICK;
            }
            else {
                top = START;
                add_word();
            }
        break;
        
        case DOUBLE_STICK:
               top = START;
                add_word();
        break;

        case SEMICOLON:
            if (c == ';'){
                add_sym();
                c = getchar();
            }
            else {
                top = START;
                add_word();
            }
        break;

        case LESS:
            if (c == '<'){
                add_sym();
                c = getchar();
            }
            else {
                top = START;
                add_word();
            }
        break;

        case LEFT_BRACKET:
            if (c == '('){
                add_sym();
                c = getchar();
            }
            else {
                top = START;
                add_word();
            }
        break;

        case RIGHT_BRACKET:
            if (c == ')'){
                add_sym();
                c = getchar();
            }
            else {
                top = START;
                add_word();
            }
        break;

        case ENTER:
            clean_list();
            top = START;
        break;

        case STOP:
            return;
        break;

        }
    }
}

//-----------Рекурсивный спуск-------------

// работа с каналами и возврат PID
pid_t conv(int begin, int end, int pipe_before[2]){
    int i, end_flag = 0;
    char *args[MAX_COM];
    for (i = begin; i < end; i++){
        if (strcmp(lst[i], "|")== 0){
            break;
        }
        args[i - begin] = lst[i];
    }
    args[i - begin] = NULL;
    if (i == end){
        end_flag = 1;
    }
    pid_t pid;
    int pipe_fd[2];
    if (!end_flag){
        pipe(pipe_fd);
    }
    if ((pid = fork()) == 0){
        dup2(pipe_before[0], 0);
        if (!end_flag) {
            close(pipe_fd[0]);
            dup2(pipe_fd[1], 1);
        }
        execvp(args[0], args);
    }
    if (pipe_before[0] != 0){
        close(pipe_before[0]);
    }
    if (!end_flag){
        close(pipe_fd[1]);
    }
    if (!end_flag){
        pid = conv(i + 1, end, pipe_fd);
    }
    return pid;
}

// перенаправление ввода-вывода
int redirect(int begin, int end, char **file_in, 
  char **file_out, char **file_out_plus){
    if (strcmp(lst[begin], "<") == 0){
        *file_in = lst[begin + 1];
    } else if (strcmp(lst[begin], ">") == 0){
        *file_out = lst[begin + 1];
    } else if (strcmp(lst[begin], ">>") == 0){
        *file_out_plus = lst[begin + 1];
    }
    if ((begin + 2 != end) && ((*file_in != NULL) || 
    (*file_out != NULL) || (*file_out_plus != NULL))){
        begin += 2;
        if (strcmp(lst[begin], "<") == 0){
            *file_in = lst[begin + 1];
            begin += 2;
        } else if (strcmp(lst[begin], ">") == 0){
            *file_out = lst[begin + 1];
            begin += 2;
        } else if (strcmp(lst[begin], ">>") == 0){
            *file_out_plus = lst[begin + 1];
            begin += 2;
        }
    }
    return begin;
}

// либо com_ либо com_if в зависимости от флагов
// эти 2 функции реализуют команды

int com_(int begin, int end){
    char *file_in = NULL, *file_out = NULL, *file_out_plus = NULL;
    if (lst[begin] == NULL){
        return 1;
    }
    int new_end = end;
    int new_begin = redirect(begin, end, &file_in, &file_out, &file_out_plus);
    if (new_begin == begin){
        for (int i = begin; i < end; i++){
            if ((strcmp(lst[i], ">>") == 0) || (strcmp(lst[i], ">") == 0) 
            || (strcmp(lst[i], "<") == 0)){
                new_end = i;
                redirect(i, end, &file_in, &file_out, &file_out_plus);
                break;
            }
        }
    }
    if (fork() == 0){
        if (strcmp(lst[begin], "(") == 0){
            exit(arr[begin]);
        }
        int fd_in, fd_out, fd_out_plus;
        if (file_in){
            fd_in = open(file_in, O_RDONLY);
            dup2(fd_in, 0);
        }
        if (file_out){
            fd_out = open(file_out, O_WRONLY | O_TRUNC | O_CREAT, 0666);
            dup2(fd_out, 1);
        }
        if (file_out_plus){
            fd_out_plus = open(file_out_plus, O_WRONLY 
            | O_APPEND | O_CREAT, 0666);
            dup2(fd_out_plus, 1);
        }
        int init_pipe[2] = {0, 1};
        pid_t last_son = conv(new_begin, new_end, init_pipe);
        int s;
        int last_s = -1;
        pid_t pid;
        while((pid = wait(&s)) != -1){
            if (pid == last_son){
                last_s = WEXITSTATUS(s);
            }
        }
        if (last_s == 0){
            exit(1);
        } else{
            exit(0);
        }
    }
    int s;
    wait(&s); //жду завершения сына
    return WEXITSTATUS(s);
}

int com_if(int begin, int end){
    int and_flag = 0, or_flag = 0, i;
    if (lst[begin] == NULL){
        return 1;
    }
    int opened = 0;
    for (i = begin; i < end; i++){
        if (strcmp(lst[i], "(") == 0){
            opened += 1;
        }
        if (strcmp(lst[i], ")") == 0){
            opened -= 1;
        }
        if ((strcmp(lst[i], "&&") == 0) && (opened == 0)){
            and_flag = 1;
            break;
        } else if ((strcmp(lst[i], "||") == 0)  && (opened == 0)){
            or_flag = 1;
            break;
        }
    }
    int ok = com_(begin, i);
    if (ok && and_flag){
        ok = com_if(i + 1, end);
    } else if (!ok && or_flag){
        ok = com_if(i + 1, end);
    }
    return ok;
}

// контроль скобок
void brackets(int begin, int end){
    int new_begin = begin, new_end, end_flag = (begin == end);
    while (!end_flag){
        while (!end_flag){
            if (strcmp(lst[new_begin], "(") == 0){
                new_begin += 1;
                break;
            }
            new_begin += 1;
            if (new_begin == end){
                end_flag = 1;
                break;
            }
        }
        if (end_flag){
            break;
        }
        new_end = new_begin;
        int opened = 1;
        while ((strcmp(lst[new_end], ")") != 0) || (opened != 1)){
            if (strcmp(lst[new_end], "(") == 0){
                opened += 1;
            }
            if (strcmp(lst[new_end], ")") == 0){
                opened -= 1;
            }
            new_end += 1;
        }
        int st;
        if (fork() == 0){
            if (arr[new_begin - 1] == -1){
                int ok = com_sh(new_begin, new_end);
                exit(ok);
            } else {
                exit(arr[new_begin]);
            }
        }
        wait(&st);
        arr[new_begin - 1] = WEXITSTATUS(st);
        new_begin = new_end + 1;
        if (new_begin == end){
            end_flag = 1;
        }
    }
}

// основное происходит здесь
int com_sh(int begin, int end){
    int flag_background = 0, flag = 0, end_flag = (begin == end), i;
    if (lst[begin] == NULL){
    	return 1;
    }
    brackets(begin, end);
    int ok = -1;
    int opened = 0;
    for (i = begin; i < end; i++){
        if (strcmp(lst[i], "(") == 0){
            opened += 1;
        }
        if (strcmp(lst[i], ")") == 0){
            opened -= 1;
        }
        if ((strcmp(lst[i], ";") == 0) && (opened == 0)){
            flag = 1;
            break;
        } else if ((strcmp(lst[i], "&") == 0) && (opened == 0)){
            flag_background = 1;
            break;
          }
    }
    if (i + flag_background + flag == end){
        end_flag = 1;
    }
    if (flag_background){
        if (fork() == 0){
            if (fork() == 0){ 
                int null[2];
                null[0] = open("/dev/null", O_RDONLY);
                null[1] = open("/dev/null", O_WRONLY);
                dup2(null[0], 0);
                dup2(null[1], 1);
                signal(SIGINT, SIG_IGN);
                com_if(begin, i);
                exit(0);
            } else{
                exit(0);
              }
        } else{
            wait(NULL);
          }
    } else{
        ok = com_if(begin, i);
      }
    if (!end_flag){
        ok = com_sh(i + 1, end);
    }
    return ok;
}

//---------MAIN-------------
int main(){
    while (1){
        for (int i = 0; i < MAX_COM; i++){
            arr[i] = -1;
        }
        create_list();
        com_sh(0, size_list - 1);
        // printf("Конец!\n");
    }
    return 0;
}
