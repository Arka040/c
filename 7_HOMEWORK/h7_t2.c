#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

struct symbol {
    char c;
    struct symbol *next;
};

struct word {
    struct symbol *symbols;
    struct word *next;
    int count;
};

struct symbol *add_symbol(struct symbol *symbol, char c) {
    struct symbol *block;
    block = (struct symbol*)malloc(sizeof(struct symbol));
    block->c = c;
    block->next = NULL;
    return block;
}

struct symbol *init(struct symbol *s) {
    struct symbol *block;
    block = (struct symbol*)malloc(sizeof(struct symbol));
    block -> next = NULL;
    block -> c = 'l';
    return block;
}

struct word *create_word(struct word *word, struct symbol *s) {
    struct word *block;
    block = (struct word*)malloc(sizeof(struct word));
    block->next = NULL;
    block->symbols = s;
    block -> count = 1;
    return block;
}

void Print_forward(struct word *word){
    struct symbol *s;
    while (word) {
        s = word->symbols;
        while (s) {
            printf("%c", s->c);
            s = s->next;
        }
        printf(" %d ", word -> count);
        printf("\n");
        
        word = word->next;
    }
}

void Print_string(struct symbol *str) {
    while (str) {
        printf("%c", str -> c);
        str = str -> next;
    }
    printf("\n");
}

void Check(struct word *word, struct symbol *s, int len) {  

    // Данная функция принимает в себя указатель на список из символов, указатель на первую строку (на начало верхнего списка) и параметр -s (int)
    // В этой функции происходит сравнение новой строки s с каждой строкой из верхнего списка (списка из всех строк) и при совпадении увеличивает count
    // а иначе добавляет новую строку в конец списка

    struct symbol *check = NULL, *curs = NULL;    
    struct word *temp = NULL, *prev = NULL;
    temp = word;
    int fl = 0, l = len;
    while (temp) {  
        fl = 0;
        curs = s;
        check = temp -> symbols;
        l = len;
        while (l != 0 && curs && check) {
            l--;
            curs = curs -> next;
            check = check -> next;
        }
        while(curs && check) {
            if (curs -> c != check -> c) {
                prev = temp;
                temp = temp -> next;
                fl = 1;
                break;
            } else {
                curs = curs -> next;
                check = check -> next;
            }
        }
        if (fl) {
            continue;
        }
        if ( (!curs && !check) || (l != 0) ) {
            temp -> count++;
            return;
        } else {
            prev = temp;
            temp = temp -> next;
        }
    }
    prev -> next = create_word(temp, s);
}

void Delete_all(struct word *word) {
    struct word *tmp;
    struct symbol *s, *temp;
    while (word) {
        s = word->symbols;
        while (s) {
            temp = s;
            s = s->next;
            free(temp);
        }
        tmp = word;
        word = word->next;
        free(tmp);
    }
}


int main(int argc, char **argv) {

    // Идея решения заключается в том, что я формирую список, в котором лежит список из символов одной строки из файла.
    // Дальше достаточно легко бегать по списку и сравнивать данные строки. Также в каждом верхнем списке лежит параметр count, который
    // считает количество повторений данных строк в файле

    int c_flag = 0, d_flag = 0, u_flag = 0, f, len = 0, j = 0;
    for (int i = 1; i < argc; i++) {
        if (argv[i][0] != '-'){
            f = i;
        } else {
            if (argv[i][1] == 'c') {
                c_flag = 1;
            } else if (argv[i][1] == 'd') {
                d_flag = 1;
            } else if (argv[i][1] == 'u') {
                u_flag = 1;
            } else {
                j = 3;
                while (argv[i][j]) {
                    len = len * 10 + (argv[i][j] - '0');
                    j++;
                }
            }
        }
    }

    int fd = open(argv[f], O_RDONLY);

    char *c = malloc(sizeof(char));

    struct symbol *s = NULL, *full_s = NULL, *start_s = NULL;
    struct word *word = NULL, *start = NULL;

    start = create_word(NULL, NULL);
    start_s = init(s);
    while(read(fd, c, sizeof(char))) {
        if (*c != '\n') {
            if (!full_s) {
                full_s = start_s;
                full_s -> c = *c;
                full_s -> next = NULL;
            } else {
                full_s -> next = add_symbol(s, *c);
                full_s = full_s -> next;
            }
        } else {
            if (!word) {
                word = start;
                word -> symbols = start_s;
            } else {
                if (full_s) {
                    Check(start, start_s, len);
                }
            }          
            start_s = init(s);
            full_s = NULL;
        }
    }
    if (!word) {
        word = start;
        word -> symbols = start_s;
    } else {
        if (full_s) {
            Check(start, start_s, len);
        }
    }  

    while (start) {
        if (d_flag) {
            if (start -> count > 1) {
                if (c_flag) {
                    printf("%d ", start -> count);
                }
                Print_string(start -> symbols);
            }
        }
        else if (u_flag) {
            if (start -> count == 1) {
                if (c_flag) {
                    printf("%d ", start -> count);
                }
                Print_string(start -> symbols);
            }
        }
        else {
            if (c_flag) {
                printf("%d ", start -> count);
            }
            Print_string(start -> symbols);
        }
        start = start -> next;
    }

    close(fd);

    Delete_all(start);

    return 0;
}