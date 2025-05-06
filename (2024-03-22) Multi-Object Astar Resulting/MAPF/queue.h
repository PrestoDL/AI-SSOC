#pragma once
#include <stdio.h>
#include <stdlib.h>
#include "stack.h"

typedef struct {
    int x;
    int y;
    Stack* path;
    int x_goal;
    int y_goal;
} Agent;

typedef int Boolean;

typedef struct tQueueNode {
    struct tQueueNode* nextNode;
    Agent* data;
} QueueNode;

typedef struct tQueue {
    QueueNode* front;
    QueueNode* rear;
    int count;
} Queue;

Queue* CreateQueue() {
    Queue* temp;
    temp = (Queue*)malloc(sizeof(Queue));

    if (temp == NULL) {
        return;
    }

    temp->count = 0;
    return temp;
}

int CountQueue(Queue* temp) {
    return temp->count;
}

Boolean IsEmptyQueue(Queue* temp) {
    if (CountQueue(temp) == 0) {
        return 1;
    }

    else {
        return 0;
    }
}

void Enqueue(Queue* temp, Agent* item) {
    if (temp == NULL) {
        return;
    }

    QueueNode* tempNode = (QueueNode*)malloc(sizeof(QueueNode));
    if (tempNode == NULL) {
        // Handle memory allocation failure
        return;
    }

    tempNode->data = item;
    tempNode->nextNode = NULL;

    if (temp->count == 0) {
        temp->front = tempNode;
        temp->rear = tempNode;
    }
    else {
        temp->rear->nextNode = tempNode;
        temp->rear = tempNode;
    }

    temp->count++;
}

Agent* Dequeue(Queue* temp) {
    if (temp == NULL || IsEmptyQueue(temp)) {
        return NULL;
    }

    QueueNode* recycle = temp->front;
    Agent* item = recycle->data;

    if (temp->count == 1) {
        temp->front = NULL;
        temp->rear = NULL;
    }
    else {
        temp->front = temp->front->nextNode;
    }

    free(recycle);
    temp->count--;
    return item;
}

Agent* QueueFront(Queue* temp) {
    if (IsEmptyQueue(temp) == 1) {
        return;
    }

    else {
        return temp->front->data;
    }
}

Agent* QueueRear(Queue* temp) {
    if (IsEmptyQueue(temp) == 1) {
        return;
    }

    else {
        return temp->rear->data;
    }
}

void DestroyQueue(Queue* temp) {
    if (temp == NULL) {
        return;
    }

    while (!IsEmptyQueue(temp)) {
        Dequeue(temp);
    }

    free(temp);
}
