#include "utils.h"

char* queuedup = NULL;

void initqueue(void)
{
    tail = NULL;
}

void enqueue(char* name)
{
    struct queue* ptr;
    char* cptr;

    if((ptr = (struct queue *) malloc (sizeof (struct queue))) == NULL)
	{
        error("malloc");
	}

    if((cptr = (char *) malloc (strlen (name) + 1)) == NULL)
	{
		error("malloc");
	}

    strcpy(cptr, name);
    ptr -> name = cptr;

    if(tail == NULL) 
	{
        ptr -> next = ptr;
    }
    else {
        ptr -> next = tail -> next;
        tail -> next = ptr;
    }
    tail = ptr;
}

int dequeue(char* name)
{
    struct queue *ptr;
    char *cptr;

    if(!tail) 
    {
        fprintf(stderr, "dt: warning: queue is empty.\n");
        return -1;
    }

    ptr = tail -> next;
    cptr = ptr -> name;

    if(ptr == tail)
	{
        tail = NULL;
	} else {
        tail -> next = ptr -> next;
	}

    free(ptr);
    strcpy(name, cptr);
    free(cptr);
    return 0;
}

void firstinline(void)
{
    struct queue *ptr, *head;

    if(!tail) 
    {
        fprintf(stderr, "dt: warning: queue is empty\n");
        return;
    }

    head = ptr = tail -> next;
    int i = 1;

    /* printf("%s\n", ptr -> name); */
    queuedup = ptr -> name;
} 

void error(char* msg)
{
    perror(msg);
    exit(EXIT_FAILURE);
}
