#ifndef FLAGS_H
#define FLAGS_H

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>

/* if you put variables in
   a .c file and extern th
   same variables in th .h
   file, only the one .o f
   ile will contain the da
   ta, causing a smaller f
   inal executable      */

/* explanation of opt[I-l] 
   can be found in file na
   med flags.c          */
extern int optL; 
extern int optt; 
extern int optp; 
extern int opti; 
extern int optu;
extern int optg;
extern int opts;
extern int optd;
extern int optl;

extern char* indents;
extern char* dirname; 

void init();

#endif // FLAGS_H