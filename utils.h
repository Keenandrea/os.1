#ifndef UTILS_H
#define UTILS_H

#include <stdio.h> 
#include <stdlib.h> 
#include <string.h>

struct queue 
{
    char* name;
    struct queue* next;
};

struct queue* tail;

extern char* queuedup;

void initqueue(void);

void enqueue(char* name);

int dequeue(char* name);

void firstinline(void);

void error(char* msg);

#endif /* UTILS_H */