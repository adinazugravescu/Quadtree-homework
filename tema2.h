#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#ifndef _tema_2_h
#define _tema_2_h
typedef struct pixel {
    unsigned char red;
    unsigned char green;
    unsigned char blue;
} Pixel;
typedef struct block {   
//the structure to retain the area in the picture - the first element of the mat and the side
    Pixel* firstElement;
    unsigned int side;
    unsigned char red, green, blue;
}Block;
typedef struct treeNode { 
//the generic node that holds the type, an area from the picture and has the vector of children
    unsigned char type;
    Block* block;
    struct treeNode** kids;
}*Tree;
Tree createTree (unsigned int size , int iMat, int jMat, Pixel** mat, int factor) {
    Tree tree = malloc(sizeof(struct treeNode));
    unsigned long long red ,green, blue, mean;  //mean for each colour
    red = green = blue = mean = 0;
    for (int i = iMat ; i < iMat + size ; i++) 
        for (int j = jMat ; j < jMat + size ; j++) {
            red += mat[i][j].red;
            green += mat[i][j].green;
            blue += mat[i][j].blue;
        }
    red /= size; red /= size; red = floor(red);
    green /= size; green /= size; green = floor(green);
    blue /= size; blue /= size; blue = floor(blue);
    for (int i = iMat ; i < iMat + size ; i++) 
        for (int j = jMat ; j < jMat + size ; j++) {
                mean += (red - mat[i][j].red)*(red - mat[i][j].red) + (green - mat[i][j].green)*(green - mat[i][j].green)
                 + (blue - mat[i][j].blue)*(blue - mat[i][j].blue);
        }
    mean = mean / (3*size*size);  mean = floor(mean);
    unsigned char type;
    if ( mean <= factor ) {
            type = 1;
    }
    else 
            type = 0;
    tree->type = type;
    tree->kids = (struct treeNode**)malloc(4*sizeof(struct treeNode*));
    if (type == 0) {
        tree->block = NULL;
        tree->kids[0] = createTree(size/2, iMat , jMat , mat, factor);
        tree->kids[1] = createTree(size/2, iMat, (jMat + size/2), mat, factor);
        tree->kids[2] = createTree(size/2, (iMat + size/2), (jMat + size/2), mat, factor);
        tree->kids[3] = createTree(size/2, (iMat + size/2), jMat, mat, factor);
    }
    else {
        tree->kids[0] = tree->kids[1] = tree->kids[2] = tree->kids[3] = NULL;
        tree->block = malloc(sizeof(struct block));
        tree->block->firstElement = &mat[iMat][jMat];
        tree->block->side = size;
        tree->block->red = red;
        tree->block->green = green;
        tree->block->blue = blue;
    }
    return tree;
}
int max(int a, int b, int c, int d) {
    int max = a;
    if (b > max) {
        max = b;
    }
    if (c > max) {
        max = c;
    }
    if (d > max) {
        max = d;
    }
    return max;
}
int heightTree (Tree tree) {
    if (tree == NULL || (tree->kids[0] == NULL && tree->kids[1] == NULL && tree->kids[2] == NULL && tree->kids[3] == NULL ))
        return 0;
    int ch1, ch2, ch3, ch4;
    ch1 = heightTree(tree->kids[0]);
    ch2 = heightTree(tree->kids[1]);
    ch3 = heightTree(tree->kids[2]);
    ch4 = heightTree(tree->kids[3]);
    return max(ch1, ch2, ch3, ch4) + 1;
}
int colorNodes (Tree tree) { //function that determines the number of colored leaves / nodes
    if(tree == NULL)
        return 0;
    else 
        if(tree->kids[0] == NULL && tree->kids[1] == NULL && tree->kids[2] == NULL && tree->kids[3] == NULL )
            return 1;
        else
            return colorNodes(tree->kids[0]) + colorNodes(tree->kids[1]) + colorNodes(tree->kids[2]) + colorNodes(tree->kids[3]);
}
int minHeight (Tree tree) {
    if (tree == NULL) {
        return -1;
    }
    int heightKid = -1;
    int min = 2000000000;
    for (int i = 0; i < 4 ; i++) {
        heightKid = minHeight(tree->kids[i]);
        if (heightKid < min)
            min = heightKid;
    }
    return min + 1;
}

//structures and functions for the implementation of operations for a queue
typedef struct list {
    Tree value;
    struct list* next;
}*List;
typedef struct queue {
    List front,rear;
}Queue;
Queue initQueue () {
    Queue q;
    q.front = q.rear = NULL;
    return q;
}
List createList (Tree value) {
    List head = malloc(sizeof(struct list));
    head->value = value;
    head->next = NULL;
    return head;
}
int isEmptyQueue(Queue* q) {
    return q->front == NULL;
}
void enqueue(Queue* q,Tree node) {
    if(q->front == NULL) {
        q->front = q->rear = createList(node);
    }
    else {
        List list = createList(node);
        q->rear->next = list;
        q->rear = list;
    }
    return;
}
Tree dequeue(Queue* q) {
    if (q->front == NULL)
        return NULL;
    Tree result = q->front->value;
    List tmp = q->front;
    q->front = q->front->next;
    if(tmp->next == NULL) 
        q->rear = NULL;
    free(tmp);
    return result;
}
void printLevel(Tree tree, FILE* file, int size) { //function for traversing the level by implementing a queue
    if (tree == NULL)
        return;
    Queue q = initQueue();
    Tree current;
    enqueue(&q, tree);
    while(!(isEmptyQueue(&q))) {
        current = dequeue(&q);
        if (current != NULL) {
            fwrite(&current->type, sizeof(unsigned char), 1, file);
            if(current->type == 1) { 
                fwrite(&current->block->red, sizeof(unsigned char), 1, file);
                fwrite(&current->block->green, sizeof(unsigned char), 1, file);
                fwrite(&current->block->blue, sizeof(unsigned char), 1, file);
            }
            enqueue(&q, current->kids[0]);
            enqueue(&q, current->kids[1]);
            enqueue(&q, current->kids[2]);
            enqueue(&q, current->kids[3]);
        }
    }
    return;
}
void freeTree(Tree tree) {
    if (tree == NULL) 
        return;
    //if (tree->type == 0) {
        freeTree(tree->kids[0]);
        freeTree(tree->kids[1]);
        freeTree(tree->kids[2]);
        freeTree(tree->kids[3]);
        free(tree->kids);
    //}
    if (tree->type == 1)
        free(tree->block);
    free(tree);
}
#endif 
