#include <stdio.h>
#include <stdlib.h>
#define COUNT 10

struct Tree {
    int n;
    struct Tree *left;
    struct Tree *right;
    int height;
};

int max(int a, int b) {
    if (a > b) {
        return a;
    } else {
        return b;
    }
}

int Height(struct Tree *block) {
    if (!block) {
        return 0;
    }
    return block -> height;
}

struct Tree *push (struct Tree *start, int num) {
    if (!start) {
        start = (struct Tree*)malloc(sizeof(struct Tree));
        start -> right = NULL;
        start -> left = NULL;
        start -> n = num;
        start -> height = 1;
    } else {
        if ((start -> n) > num) {
            start -> left = push(start -> left, num);
            start -> height = max(Height(start -> left), Height(start -> right)) + 1;
        } else if (start -> n < num) {
            start -> right = push(start -> right, num);
            start -> height = max(Height(start -> left), Height(start -> right)) + 1;
        }
    }
    return start;
}

void Delete(struct Tree *start) {
    if (start) {
        Delete(start -> left);
        Delete(start -> right);
    }
    free(start);
}

int Check(struct Tree *start, int num) {
    int len = 0;
    while (start) {
        if (start -> n == num) {
            return len;
        } 
        else if (start -> n > num) {
            start = start -> left;
        } else {
            start = start -> right;
        }
        len++;
    }
    return -1;
}

struct Tree *Delete_block(struct Tree *start, int num) {
    struct Tree *p, *parent, *temp_left, *temp_right;
    p = start;
    parent = NULL;
    while ((p -> n != num) && p) {
        if (p -> n < num) {
            parent = p;
            p = p -> right;
        }
        else if (p -> n > num) {
            parent = p;
            p = p -> left;
        } 
    }
    if (!(p -> left) && !(p -> right)) {  // если сын без детей, то говорим родителю, что у него больше нет сына
        if (!parent) {
            free(p);
            return NULL;
        }                                     
        if (parent -> left) {
            if (p -> left) {
                parent -> left = NULL;
            }
        } else {
            parent -> right = NULL;
        }
        free(p);
        return start;
    }
    else if (!(p -> left) || !(p -> right)) {   // если у ребенка есть хотя бы один потомок
        temp_left = p -> left;
        temp_right = p -> right;
    }
    if (!temp_left) {                           // если левый потомок отсутсвует, то находим с какой стороны у родителя наш сын и вместо него
        if (!parent) {
            free(p);
            return temp_right;
        }                               
        if (parent -> left) {                   // привязываем единственного сына сына
            if (parent -> left -> n == num) {
                parent -> left = temp_right;
            }
        } else {
            parent -> right = temp_right;
        }
    }
    else if (!temp_right) {                     // если правый потомок отсутсвует, то находим с какой стороны у родителя наш сын и вместо него
        if (!parent) {
            free(p);
            return temp_left;
        } 
        if (parent -> left) {                   // привязываем единственного сына сына
            if (parent -> left -> n == num) {
                parent -> left = temp_left;
            }
        } else {
            parent -> right = temp_left;
        }
    } else {
        int fl = 0;
        p = p -> right;                         // если у ребенка есть оба сына, то нам нужно вместо сына поставить следующий элемент по значению
        if (!(p -> left)) {
            fl = 1;
        }
        while (p -> left) {                     // для этого идем к правому сыну и от него начинаем искать последнего левого сына
            p = p -> left;                      // последгний левый сын лежит в p
        }
        if (!parent) {
            printf("%d\n", p -> n);
            parent = start;
            parent -> n = p -> n;
            if (fl) {
                parent -> right = parent -> right -> right;
            } else {
                parent -> right -> left = p -> right;
            }
            free(p);
            return parent;
        } 
        if (parent -> left) {                   
            if (parent -> left -> n == num) {   
                parent -> left -> n = p -> n;                       // значение последнего левого сына ставим вместо удаляемого элемента
                if (fl) {
                    parent -> left -> right = p -> right;
                } else {
                    parent -> left -> right -> left = p -> right;       // правого сына последнего левого сына передаем левому сыну правого сына удаляемого звена
                }
            }
        } else {
            parent -> right -> n = p -> n;
            if (fl) {
                parent -> right -> right = p -> right;
            } else {
                parent -> right -> right -> left = p -> right;
            }
        }
    }
    free(p);

    return start;
}

void Print(struct Tree* root, int space)
{
    if (root == NULL)
        return;
 
    space += COUNT;
    Print(root->right, space);

    printf("\n");
    for (int i = COUNT; i < space; i++)
        printf(" ");
    printf("%d  h(%d)\n", root->n, root -> height);
 
    Print(root->left, space);
}

struct Tree *left_left_turn(struct Tree *block) {  
    struct Tree *temp1, *p;

    temp1 = block -> left -> right;
    p = block -> left;
    block -> left = temp1;
    p -> right = block;
    block = p;
    block -> left -> height = max(Height(block -> left-> left), Height(block -> left -> right)) + 1; 
    block -> right -> height = max(Height(block -> right -> left), Height(block -> right -> right)) + 1;    
    block -> height = max(Height(block -> left), Height(block -> right)) + 1;

    return block;
}

struct Tree *left_right_turn(struct Tree *block) {
    struct Tree *temp1, *temp2, *p;

    p = block -> left -> right;
    temp1 = block -> left -> right -> right;
    temp2 = block -> left -> right -> left; 
    p -> right = block;
    p -> left = block -> left;
    p -> left -> right = temp2;
    p -> right -> left = temp1;
    block = p;
    block -> left -> height = max(Height(block -> left-> left), Height(block -> left -> right)) + 1; 
    block -> right -> height = max(Height(block -> right -> left), Height(block -> right -> right)) + 1;    
    block -> height = max(Height(block -> left), Height(block -> right)) + 1;

    return block;
}

struct Tree *right_right_turn(struct Tree *block) {
    struct Tree *temp, *p;

    temp = block -> right -> left;
    p = block -> right;
    block -> right = temp;
    p -> left = block;
    block = p;
    block -> left -> height = max(Height(block -> left-> left), Height(block -> left -> right)) + 1; 
    block -> right -> height = max(Height(block -> right -> left), Height(block -> right -> right)) + 1;    
    block -> height = max(Height(block -> left), Height(block -> right)) + 1;
    
    return block;
}

struct Tree *right_left_turn(struct Tree *block) {
    struct Tree *temp1, *temp2, *p;

    p = block -> right -> left;
    temp1 = block -> right -> left -> right;
    temp2 = block -> right -> left -> left;
    p -> left = block;
    p -> right = block -> right;

    p -> left -> right = temp2;
    p -> right -> left = temp1;
    block = p;

    block -> left -> height = max(Height(block -> left-> left), Height(block -> left -> right)) + 1; 
    block -> right -> height = max(Height(block -> right -> left), Height(block -> right -> right)) + 1;    
    block -> height = max(Height(block -> left), Height(block -> right)) + 1;

    return block;
}

int GetBalance(struct Tree *block){
    int h = 0;
    
    if (block -> left) {
        h -= block -> left -> height;
    }
    if (block -> right) {
        h += block -> right -> height;
    }
    return h;
}

struct Tree *balance(struct Tree *root, struct Tree *par, struct Tree *start) {
    struct Tree *p;
    p = root;
    if (p) {
        balance(p -> left, p, start);
        balance(p -> right, p, start);
    }
    if (!p) {
        return root;
    }
    if ((GetBalance(p) < -1)) {
        if (GetBalance(p -> left) <= 0) {    
            if (par == NULL) {
                return left_left_turn(p);
            }
            if (par -> left == p) {
                par -> left = left_left_turn(p);
            }
            else {
                par -> right = left_left_turn(p);
            }
            printf("*****************************************\n");
            Print(start, 0);
            printf("*****************************************\n");
        } else if (GetBalance(p -> left) > 0){
            if (par == NULL) {
                return left_right_turn(p);
            }
            if (par -> left == p) {
                par -> left = left_right_turn(p);
            }
            else {
                par -> right = left_right_turn(p);
            }        
            printf("*****************************************\n");
            Print(start, 0);
            printf("*****************************************\n");
        }
    }
    else if (GetBalance(p) > 1) {
        if (GetBalance(p -> right) >= 0) { 
            if (par == NULL) {
                return right_right_turn(p);
            }
            if (par -> left == p) {
                par -> left = right_right_turn(p);
            }
            else {
                par -> right = right_right_turn(p);
            }        
            printf("*****************************************\n");
            Print(start, 0);
            printf("*****************************************\n");
        } else if (GetBalance(p -> right) < 0){
            if (par == NULL) {
                return right_left_turn(p);
            }
            if (par -> left == p) {
                par -> left = right_left_turn(p);
            }
            else {
                par -> right = right_left_turn(p);
            }
            printf("*****************************************\n");
            Print(start, 0);
            printf("*****************************************\n");
        }
    }
    return root;
}

struct Tree *rebalance(struct Tree *start) {
    if (start) {
        rebalance(start -> left);
        rebalance(start -> right);
    } else {
        return start;
    }
    start -> height = max(Height(start -> left), Height(start -> right)) + 1;
    return start;
}

int CheckBalance(struct Tree *start) {
    if (!start) {
        return 0;
    }
    int fl = GetBalance(start);
    if ((fl > 1) || (fl < -1)) {
        return 1;
    }
    return CheckBalance(start -> left) || CheckBalance(start -> right);
}

int main () {
    int num;
    struct Tree *root = NULL;

    printf("Enter numbers. To stop input press 0\n");

    while ((scanf("%d", &num)), num != 0) {
        root = push(root, num);
    }
    
    printf("-----------------------------------------\n");
    Print(root, 0);
    printf("-----------------------------------------\n");

    printf("Enter number to find\n");
    scanf("%d", &num);
    printf("---------\n");
    printf("%d\n", Check(root, num));
    printf("---------\n");

    printf("Enter number to delete\n");
    scanf("%d", &num);
    root = Delete_block(root, num);

    root = rebalance(root);

    printf("-----------------------------------------\n");
    Print(root, 0);
    printf("\n");
    printf("-----------------------------------------\n");

    while(CheckBalance(root) == 1) {
        root = balance(root, NULL, root);
        root = rebalance(root);
    }

    printf("        BALANCED TREE           \n");
    Print(root, 0);
    printf("\n        BALANCED TREE           \n");

    Delete(root);
    

    return 0;
}