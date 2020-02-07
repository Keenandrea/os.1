#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <string.h>
#include <dirent.h>
#include <errno.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>
/* http://www.cs.cmu.edu/afs/cs/user/yachen/include/pwd.h */
#include <pwd.h>
/* http://man7.org/linux/man-pages/man0/grp.h.0p.html */
#include <grp.h>

#include "flags.h"
#include "utils.h"

#define MAXSIZE 1024
#define BUFSIZE 256

char active_filesys[MAXSIZE] = "";

void *getcwdir(void); 

void bfs(char* node, char* fname, int indent, char* data);

int isdir(char* path);

void catenater(int* marker, char* datum);

void printi(char* path, char* fname, int indent, char* data);

void printe();

void filesystem(char* path, int indent, char* root);

void helpme();

/* MAIN ================================================================================== */
/* ======================================================================================= */
int main(int argc, char *argv[])
{
    init();
    int it;
    int ch;

    while( (ch = getopt(argc,argv,"hI:Ltpiugsdl")) != -1 )
    {
        switch(ch)
        {
            case 'h':
                helpme();
                return 0;
            case 'L':
                optL = 1;
                break;
            case 't':
                optt = 1;
                break;
            case 'p':
                optp = 1;
                break;
            case 'i':
                opti = 1;
                break;
            case 'u':
                optu = 1;
                break;
            case 'g':
                optg = 1;
                break;
            case 's':
                opts = 1;
                break;
            case 'd':
                optd = 1;
                break;
            case 'l': 
                optp = 1; 
                opti = 1; 
                optu = 1; 
                optg = 1; 
                opts = 1;  
                optd = 1;
                optt = 1; 
                break;
            /* if getopt() finds      
               an option charact
               er in argv that w
               as not included i
               n optstring, or i
               f it detects a mi
               ssing option argu
               ment, it returns
               '?' and sets the 
               external variable
               optopt to the act
               ual option char*/
            case '?': 
                printf("dt: error: invalid argument. refer to option -h.\n");
                return 0;
        }
    }
    
    initqueue();
    char cwd[PATH_MAX];
    /* gets program root, th
       at is, the name, only
       the name, of the dire
       ctory that houses the
       program modules    */
    char *root = getcwdir();

    if(argv[optind] != NULL)
    {
        dirname = argv[optind];
    }
    if(dirname == NULL)
    {
       if(getcwd(cwd, sizeof(cwd)) != NULL)
       {
            dirname = cwd;
       } else {
           perror("dt: error");
           exit(EXIT_FAILURE);
       }
    }

    /* prints the root directory    */
    printi(dirname, dirname, 0, root);
    printe();
    
    /* traverses and queues in ho
       wever many items are the n
       ext nodes after root    */
    filesystem(dirname, 0, root);

    while(tail)
    {
        firstinline();
        /* printf("fil is %s", queuedup); */
        filesystem(queuedup, 0, root);
        dequeue(queuedup);
    }
    

    free(root);
    return 0;
}
/* END =================================================================================== */


/* RETRIEVES ONLY THE NAME OF OUR CWD ==================================================== */
/* ======================================================================================= */
void *getcwdir(void)
{
    char *cwdir, *token;
    char buffer[PATH_MAX + 1];
    char *directory;
    size_t length;

    cwdir = getcwd(buffer, PATH_MAX + 1);
    token = strrchr(cwdir, '/');

    if(cwdir == NULL)
    {
        perror("dt: error"); 
        exit(EXIT_FAILURE);
    }

    if (token == NULL)
    {
        perror("dt: error");
        exit(EXIT_FAILURE);
    }

    length = strlen(token);
    directory = malloc(length);
    memcpy(directory, token+1, length);

    return directory;
}
/* END =================================================================================== */


/* BREADTHE FIRST SEARCH ================================================================= */
/* ======================================================================================= */
void bfs(char* node, char* fname, int indent, char* root)
{
    printf("BFS calls with: %s\n", node);


}
/* END =================================================================================== */


/* CHECKS FOR DIRECTORY ================================================================== */
/* ======================================================================================= */
int isdir(char* path)
{
    /* on linux, stat struct is defined as follows:
    dev_t     st_dev;      id of th file device
    ino_t     st_ino;      inode number
    mode_t    st_mode;     file's type and mode
    nlink_t   st_nlink;    number of hard links
    uid_t     st_uid;      the user ID of owner
    gid_t     st_gid;      th group ID of owner
    dev_t     st_rdev;     device ID if special
    off_t     st_size;     total size, in bytes */
    struct stat stats;

    if(stat (path, &stats) == -1)
    {
        return 0;
    } else {
        return S_ISDIR(stats.st_mode);
    }
}
/* END =================================================================================== */


/* CATENATES DATA ======================================================================== */
/* ======================================================================================= */
void catenater(int* marker, char* datum)
{
    int kickoff = *marker;
    for(; *marker < kickoff+strlen(datum); ++*marker)
    {
        active_filesys[*marker] = datum[*marker - kickoff];
    }
    active_filesys[*marker + 1] = '\0';
}
/* END =================================================================================== */


/* CONTROLS WHICH DATA TO CATENATE ======================================================= */
/* ======================================================================================= */
void printi(char *path, char *fname, int indent, char* root)
{
    int index;

    /* so we don't mistakenly regar
       d current and parent dirs */
    if(path == "." || path == "..")
    {
        return;
    }

    struct stat fstats;

    /* lstat() is equivalent to stat()
       except that if path is a symbol
       ic link, then the link itself i
       s stat-ed, and not the file tha
       t symbolic link refers to    */
    /* stats the link pointed to by pa
       th and fills in (fstats) buf */
    if(lstat (path, &fstats) == -1)
    {
        perror("dt: error");
        exit(EXIT_FAILURE);
    }

    /* for loop includes path properly         
    for(index = 0; index < strlen(root); ++index)
    {
        active_filesys[index] = root[index];
    }
    */
    
    /*
    if(isdir(path))
    {
        enqueue(path);
        printf("DIR  ");
    } else {
        enqueue(fname);
        printf("NOT DIR  ");
    }
    */
    /*
    active_filesys[index] = "/";
    index = index + 1;
    */
    /* for loop appends the proper filename   
    for(index = strlen(path); index < indent+strlen(fname); ++index)
    {
        active_filesys[index] = fname[index - indent];
    }
 
    active_filesys[index] = '\0';
       */

    if(optL == 1)
    {
        /* S_IFMT bit mask for the file type bit
        field. S_IFLNK symbolic link       */
        if( (fstats.st_mode & S_IFMT) == S_IFLNK)
        {
            char symlink_ref[256];
            const char* symlink_path = fname;
            /* readlink places the contents of symlink_path in the symlink_ref, which
            has a size of symlink_ref[256]. readlink() does not append a null byte
            to symlink_ref. it will silently truncate the contents to a length of 
            symlink_ref[256], in case the buffer is too small to hold contents  */
            int bufsiz_len = readlink(symlink_path, symlink_ref, sizeof(symlink_ref));

            /* on error, -1 is r
            eturned and errno
            is indicated   */
            if(bufsiz_len == -1)
            {
                perror("dt: error");
            /* on success, these calls return t
            he num of bytes in symlink_ref*/
            } else {
                /* append null byte to ref   */
                symlink_ref[bufsiz_len] = '\0';
                catenater(&index, "symlink:");
                catenater(&index, symlink_ref);
            }
        }
    }

    int jindex;
    /* space the printed information prettily*/
    for(jindex = index; jindex < 0; ++jindex)
    {
        catenater(&index, " ");
    }

    /* print the
       permissio
       n bits */
    if(optp == 1)
    {
        int fmode = fstats.st_mode;

        if(S_ISDIR(fmode))
        {
            catenater(&index, "d");
        } else {
            catenater(&index, "-");
        }

        if(fmode & S_IRUSR)
        {
            catenater(&index, "r");
        } else {
            catenater(&index, "-");
        }

        if(fmode & S_IWUSR)
        {
            catenater(&index, "w");
        } else {
            catenater(&index, "-");
        }

        if(fmode & S_IXUSR)
        {
            catenater(&index, "x");
        } else {
            catenater(&index, "-");
        }

        if(fmode & S_IRGRP)
        {
            catenater(&index, "r");
        } else {
            catenater(&index, "-");
        }

        if(fmode & S_IWGRP)
        {
            catenater(&index, "w");
        } else {
            catenater(&index, "-");
        }

        if(fmode & S_IXGRP)
        {
            catenater(&index, "x");
        } else {
            catenater(&index, "-");
        }

        if(fmode & S_IROTH)
        {
            catenater(&index, "r");
        } else {
            catenater(&index, "-");
        }

        if(fmode & S_IWOTH)
        {
            catenater(&index, "w");
        } else {
            catenater(&index, "-");
        }

        if(fmode & S_IXOTH)
        {
            catenater(&index, "x");
        } else {
            catenater(&index, "-");
        }

        catenater(&index, "  ");
    }

    /* print the number of 
       links to file in in
       ode table        */
    if(opti == 1)
    {
        char chabuff[128];
        /* string print the
           numlinks instead
           of printing on t
           he console. stor
           es output on cha
           r buffer      */
        sprintf(chabuff, "%ld", fstats.st_nlink);
        catenater(&index, chabuff);
        catenater(&index, "  "); 
    }

    /* print user
       id associa
       ted with t
       he file */
    if(optu == 1)
    {
        struct passwd *pwd;
        char uids[128];

        if( (pwd = getpwuid(fstats.st_uid)) == NULL)
        {
            sprintf(uids, "%ld", fstats.st_uid);
            catenater(&index, uids);
            catenater(&index, "  ");
        } else {
            catenater(&index, pwd->pw_name);
            catenater(&index, "  ");
        }
    }

    /* print grou
       p id assoc
       iated with
       the file*/
    if(optg == 1)
    {
        struct group *grp;
        char gids[128];

        if( (grp = getgrgid(fstats.st_gid)) == NULL)
        {
            sprintf(gids, "%ld", fstats.st_gid);
            catenater(&index, gids);
            catenater(&index, "  ");
        } else {
            catenater(&index, grp->gr_name);
            catenater(&index, "  ");
        }
    }

    /* print the
       size of t
       he file i
       n bytes*/
    if(opts == 1)
    {
        const int bytesize = fstats.st_size;
        /* 1 byte = 8 bis. 1 KB(K/Kb) = 2^10
           bytes = 1024 bytes. 1 MB(M/MB) =
           2^20 bytes = 1,048,576 bytes. 1 G
           B(G/GB) = 2^30 bytes = 1,073,741,
           824 bytes. these conversions   */
        int kilo = 1024;
        int mega = 1048576;
        int giga  = 1073741824;

        char bs[kilo];
        /* keep bytesize */
        if(bytesize < kilo)
        {
            sprintf(bs, "%d", bytesize);
            catenater(&index, bs);
            if(bytesize >= 1000)
            {
                catenater(&index, " ");
            }
            else if(bytesize >= 100 && bytesize < 1000)
            {
                catenater(&index, "   "); 
            }
            else if(bytesize >= 10 && bytesize < 100)
            {
                catenater(&index, "    ");
            } else {
                catenater(&index, "     ");
            }
        }

        /* proselytize to kilobytes              */
        if(bytesize / kilo >= 1 && bytesize < mega)
        {
            sprintf(bs, "%dK", bytesize / kilo);
            catenater(&index, bs);
            if(bytesize / kilo >= 100 && bytesize / kilo < 1000)
            {
                catenater(&index, "  "); 
            }
            else if(bytesize / kilo >= 10 && bytesize / kilo < 100)
            {
                catenater(&index, "   "); 
            } else {
                catenater(&index, "    ");
            }
        }

        /* proselytize to megabytes              */
        if(bytesize / mega >= 1 && bytesize < giga)
        {
            sprintf(bs, "%dM", bytesize / mega);
            catenater(&index, bs);
            if(bytesize / mega >= 100 && bytesize / mega < 1000)
            {
                catenater(&index, "  "); 
            }
            else if(bytesize / mega >= 10 && bytesize / mega < 100)
            {
                catenater(&index, "   "); 
            } else {
                catenater(&index, "    ");
            }
        }

        /* proselytize to gigabytes */
        if( (bytesize / giga) >= 1)
        {
            sprintf(bs, "%dG", bytesize / giga);
            catenater(&index, bs);
            catenater(&index, "   "); 
        }
    }

    /* show the t
       ime of las
       t mod   */
    if(optd == 1)
    {
        /* read all file prop
           erties of fname an
           d dump them into f
           stats buf struct*/
        stat(fname, &fstats);
        char date[17];
        strftime(date, 17, "%b %d, %Y", gmtime(&(fstats.st_ctime)));
        catenater(&index, date);
        catenater(&index, "  ");
    }

    /*
    if(optt == 1)
    {
        switch(fstats.st_mode & S_IFMT)
        {

            case S_IFBLK:
                catenater(&index, "block special");
                break;

            case S_IFCHR:
                catenater(&index, "character special");
                break;
            
            case S_IFIFO:
                catenater(&index, "FIFO special");
                break;

            case S_IFREG:
                catenater(&index, "regular");
                break;

            case S_IFDIR:
                catenater(&index, "directory");
                break;

            case S_IFLNK:
                catenater(&index, "symbolic link");
                break;   
        }
        catenater(&index, "     ");
    }
    */

    /* loop appends the proper filename 
    for(; index < indent+strlen(fname); ++index)
    {
        active_filesys[index] = fname[index - indent];
    }
    active_filesys[index] = '\0';
    */
    catenater(&index, path);
}
/* END =================================================================================== */


/*  CREATES A NEWLINE FOR FURTHER DATA   ================================================= */
/* ======================================================================================= */
void printe()
{
    int index;

    printf("%s\n", active_filesys);
    for(index = 0; index < MAXSIZE; ++index)
    {
        active_filesys[index] = 0;
    }
}
/* END =================================================================================== */


/* TRAVERSES THE DIRECTORY =============================================================== */
/* ======================================================================================= */
void filesystem(char* path, int indent, char* root)
{
    /* on linux, dirent struct is defined as follows:    
    ino_t          d_ino;       inode number
    off_t          d_off;       offset to next dirent
    unsigned short d_reclen;    length of this record
    unsigned char  d_type;      file type
    char           d_name[256]; file name          */
    struct dirent *de = NULL;
    DIR *dr = NULL;

    /* returns DIR pointer */
    if(!(dr = opendir(path)))
    {
        return;
    }

    /* readdir() returns a pointer to a
       dirent structure (de) representi
       ng the next directory entry in t
       he directory stream pointed to b
       y (dr). it returns NULL on reach
       ing the end of the directory str
       eam or it an error occured    */
    while( (de = readdir(dr) ) != NULL)
    {
        if(isdir(path))
        {
            char pathn[1024];

            /* if both of the parameters are equal, strcmp() returns int 0   */
            if(strcmp (de->d_name, ".") == 0 || strcmp (de->d_name, "..") == 0)
            {
                continue;
            }
            /* formats and stores a series of characters and values in
               the array buffer. so the buffer is [currently_working_n
               ode/directory]                                       */
            snprintf(pathn, sizeof(pathn), "%s/%s", path, de->d_name);
            
            printi(pathn, de->d_name, indent, root);
            printe();
            enqueue(pathn);

        } else {
            printi(de->d_name, de->d_name, indent, root);
            printe();
        }
    }   
closedir(dr);
}
/* END =================================================================================== */


/* SHOWS HELP MENU======================================================================== */
/* ======================================================================================= */
void helpme()
{
    printf("\n|HELP|MENU|");
    printf("\t-h : display help menu\n");
    printf("\t-L : follow symbolic links, if any. default does not follow symbolic links\n");
    printf("\t-t : print information on file type\n");
    printf("\t-p : print permission bits\n");
    printf("\t-i : print the number of links to file in inode table\n");
    printf("\t-u : print the UID associated with the file\n");
    printf("\t-g : print the GID associated with the file\n");
    printf("\t-s : print the size of file in bytes\n");
    printf("\t-d : show the time of last modification\n");
    printf("\t-l : print information on file as if options t p i u g s are all specified\n");
}
/* END =================================================================================== */
/* END =================================================================================== */