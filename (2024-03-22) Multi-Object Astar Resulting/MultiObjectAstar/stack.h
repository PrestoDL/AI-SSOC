#pragma once
#include <stdlib.h>
#include <stdio.h>

typedef struct {
    int x;
    int y;
} Coordinate;

typedef int Boolean;

typedef struct tStackNode {
    struct tStackNode* nextNode;
    Coordinate *data;
} StackNode;

typedef struct tStack {
    StackNode* topNode;
    int count;
} Stack;

Stack* CreateStack() {
    Stack* temp = (Stack*)malloc(sizeof(Stack));
    if (temp == NULL) {
        return NULL;
    }

    temp->count = 0;
    temp->topNode = NULL;

    return temp;
}

void Push(Stack* temp, Coordinate* item) {
    StackNode* tempNode = (StackNode*)malloc(sizeof(StackNode));
    if (tempNode == NULL) {
        return;
    }

    tempNode->data = item;
    tempNode->nextNode = temp->topNode;
    temp->count++;
    temp->topNode = tempNode;
}

Coordinate* Pop(Stack* temp) {
    if (temp->count == 0) {
        return;
    }

    StackNode* recycle;
    recycle = temp->topNode;
    temp->topNode = recycle->nextNode;
    Coordinate* result = recycle->data;
    free(recycle);
    temp->count--;
    return result;
}

Coordinate* Top(Stack* temp) {
    if (temp->count == 0) {
        return;
    }

    return temp->topNode->data;
}

void DestroyStack(Stack* temp) {
    while (temp->count > 0) {
        StackNode* recycle;
        recycle = temp->topNode;
        temp->topNode = recycle->nextNode;
        free(recycle);
        temp->count--;
    }

    free(temp);
}

Boolean IsEmptyStack(Stack* temp) {
    if (temp->count == 0) {
        return 1;
    }

    return 0;
}

int CountStackItem(Stack* temp) { return temp->count; }

void ClearStack(Stack* temp) {
    while (temp->count > 0) {
        StackNode* recycle;
        recycle = temp->topNode;
        temp->topNode = recycle->nextNode;
        free(recycle);
        temp->count--;
    }
}
