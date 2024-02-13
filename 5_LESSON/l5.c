#include <stdio.h>
#include <stdlib.h>

struct line {
    int n;
    int count;
    struct line *next;
};

struct line *add_one(struct line *curr, int x) {            // функция добавления новго звена в конец списка
    struct line *block;
    block = (struct line*)malloc(sizeof(struct line));  // выделяем память для звена
    block->n = x;
    block->count = 1;
    block->next = NULL;
    return block;
}

void delete_all(struct line *head) {
    struct line *current, *previous;
    while(head->next) {
        current = head;
        while (current->next) {
            previous = current;
            current = current->next;
        }
        free(current);
        previous->next = NULL;
    }
    free(head);
}

struct line *reverse(struct line *head) {
    struct line *previous, *current, *following;
    current = head;
    previous = NULL;
    while (current) {                           
        following = current->next;              // сохраняем следующее звено
        current->next = previous;               // реверсируем указатель на предыдущее звено
        previous = current;                     // двигаемся дальше по списку
        current = following;
    }
    return previous;

}

void print_list(struct line *head) {
    struct line *current;
    current = head;
    while (current) {                           
        printf("number ->   %d    counter ->   %d\n", current->n, current->count);   
        current = current->next;
    }
}

int main () {
    int x;

    scanf("%d", &x);                            // считываем первый символ для дальнейшего определения головы списка

    if (!x) {
        return 0;
    } 

    int max = 1;     
    struct line *start = NULL;
    start = add_one(start, x);

    struct line *current, *previous;
    while (scanf("%d", &x) != EOF) {
        current = start;                        // в current засовываем указатель на голову списка 
        while(current) {                        // в дальнейшем пробегаем по списку, сравниваем числа и если такое уже есть
            if (current->n == x) {              // то увеличваем счетчик coun на 1
                current->count++;               
                if (current->count > max) {     // обновляем максимум 
                    max = current->count;
                }
                break;
            }
            previous = current;
            current = current->next;
        }
        if (!current) {                         // если дошли до конца списка и повторений не было, значит повторений не было, значит надо добавить 
            previous->next = add_one(previous->next, x);   // новое звено с новым числом
        }
    }
    printf("---------------------------------\n");

    current = start;
    while (current) {                           // пока не дойдем до конца списка - выводим те числа, чьи счетчики count равны максимальному
        if (current->count == max) {
            printf("number ->   %d    counter ->   %d\n", current->n, current->count);   
        }
        current = current->next;
    }
    printf("---------------------------------\n");

    start = reverse(start);
    print_list(start);

    printf("---------------------------------\n");

    delete_all(start);
    return 0;
}