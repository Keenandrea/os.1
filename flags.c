#include "flags.h"

int opth; // help
int optL; // symbolic links
int optt; // print info
int optp; // print permission bits
int opti; // print number of links
int optu; // print file UID
int optg; // print file GID
int opts; // print file size
int optd; // show time of last mod
int optl; // print tpiugs info

char* indents;
char* dirname;

void init() 
{
    int opth = 0; 
    int optL = 0; 
    int optt = 0; 
    int optp = 0; 
    int opti = 0; 
    int optu = 0; 
    int optg = 0; 
    int opts = 0; 
    int optd = 0; 
    int optl = 0; 

    indents = NULL;
    dirname = NULL;
}
