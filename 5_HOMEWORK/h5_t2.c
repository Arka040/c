#include <stdio.h>
#include <stdlib.h>

struct symbol {
    char c;
    struct symbol *next;
};

struct word {
    struct word *prev;
    struct symbol *symbols;
    struct word *next;
};

struct symbol *add_symbol(struct symbol *symbol, char c) {
    struct symbol *block;
    block = (struct symbol*)malloc(sizeof(struct symbol));
    block->c = c;
    block->next = NULL;
    return block;
}

struct word *create_word(struct word *word, struct word *previous, struct symbol *s) {
    struct word *block;
    block = (struct word*)malloc(sizeof(struct word));
    block->next = NULL;
    block->prev = previous;
    block->symbols = s;
    return block;
}

struct word *insert(struct word *start, int p) {
    struct word *block, *cur, *temp;    
    struct symbol *s = NULL;
    char c;
    block = (struct word*)malloc(sizeof(struct word));
    cur = start;
    if (p == 1) {
        block -> next = cur;
        block -> prev = NULL;
        cur -> prev = block;
        start = block;
    } else {
        while ((p != 1) && (cur) ) {
            p--;
            temp = cur;
            cur = cur -> next;
        }
        block -> next = cur;
        block -> prev = temp;
        
        if (cur) {
            cur -> prev = block;        
        }
        temp -> next = block;
    }
    scanf("%c", &c);
    block -> symbols = add_symbol(s, c);
    s = block -> symbols;
    while (((c = getchar()) != ' ') && (c != EOF) && (c != '\n')) {

        s->next = add_symbol(s->next, c);
        s = s->next;
    
    }
    return start;
}

struct word *erase(struct word *start, int p) {
    struct word *tmp;
    struct symbol *s, *temp;

    tmp = start;
    if (p == 1) {
        tmp = start;
        start = start -> next;
        start -> prev = NULL;
        s = tmp -> symbols;
        while (s) {
            temp = s;
            s = s->next;
            free(temp);
        }
        free(tmp);
        return start;
    }

    while (p != 1 && tmp -> next) {
        p--;
        tmp = tmp-> next;
    }
    if (p != 1) {
        return start;
    }
    if (tmp -> next) {
        tmp -> prev -> next = tmp -> next;
        tmp -> next -> prev = tmp -> prev;
    } else {
        tmp -> prev -> next = NULL;
    }
    
    s = tmp->symbols;
    while (s) {
        temp = s;
        s = s->next;
        free(temp);
    }
    free(tmp);
    
    return start;
}

void Print_forward(struct word *word){
    struct symbol *s;
    while (word) {
        s = word->symbols;
        while (s) {
            printf("%c", s->c);
            s = s->next;
        }
        printf(" ");
        word = word->next;
    }
}

void Print_backward(struct word *word){
    struct symbol *s;
    while (word -> next) {
        word = word -> next;
    }
    while (word) {
        s = word->symbols;
        while (s) {
            printf("%c", s->c);
            s = s->next;
        }
        printf(" ");
        word = word->prev;
    }
}

struct word *swap(struct word *pos1, struct word *pos2, struct word *start) {
    struct word *temp1 = NULL, *temp2 = NULL;
    
    temp2 = pos1->prev;
    temp1 = pos2->next;

    pos2->next = pos1;
    pos1->prev = pos2;

    pos1->next = temp1;

    pos2->prev = temp2;

    if (temp1){
        temp1->prev = pos1;

    }
    if (temp2) {
        temp2->next = pos2;
        return start;
    }
    else {
        return pos2;
    }
}

struct word *Bubble_sort(struct word *start) {
    int fl = 1;
    struct word *pos1, *pos2;
    struct symbol *s1, *s2;
    while (fl) {
        pos1 = start;
        pos2 = start->next;
        fl = 0;
        while (pos2 && (fl == 0)) {
            s1 = pos1->symbols;
            s2 = pos2->symbols;
            while (s1 && s2) {
                if (s1->c > s2->c) {
                    fl = 1;
                    start = swap(pos1, pos2, start);
                    break;
                }
                if (s1->c < s2->c) {
                    break;
                }
                else {
                    s1 = s1->next;
                    s2 = s2->next;
                }
            }
            pos1 = pos1->next;
            pos2 = pos2->next;
        }
    }
    return start;
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

int main () {
    struct symbol *s = NULL;
    struct word *word = NULL, *start = NULL, *previous = NULL;       
    char c;
    start = create_word(NULL, NULL, NULL);
    while ((c = getchar()) != EOF && c != '\n') {
        if (c == ' ') {
            continue;
        }
        if (word != NULL) {
            word->next = create_word(word, previous, NULL);
            word = word->next;
        }
        else {
            word = start;
        }
        
        word->symbols = add_symbol(s, c);
        s = word->symbols;

        while (((c = getchar()) != ' ') && (c != EOF) && (c != '\n')) {

            s->next = add_symbol(s->next, c);
            s = s->next;

        }

        if ((c == EOF) || (c == '\n')) {
            break;
        }
        previous = word;
        
    }

    printf("Forward output:  ");
    Print_forward(start);
    printf("\n");

    int p;
    printf("Enter place where to insert new word\n");
    scanf("%d\n", &p);
    start = insert(start, p);

    printf("Forward output:  ");
    Print_forward(start);
    printf("\n");

    printf("Backward output: ");
    Print_backward(start);
    printf("\n");

    printf("Enter num of elem to remove\n");
    scanf("%d", &p);
    start = erase(start, p);

    printf("Forward output:  ");
    Print_forward(start);
    printf("\n");

    printf("Backward output: ");
    Print_backward(start);
    printf("\n");

    start = Bubble_sort(start);
    printf("Sorted output:   ");
    Print_forward(start);
    
    printf("\n");

    Delete_all(start);

    return 0;
}