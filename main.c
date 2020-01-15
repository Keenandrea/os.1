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
// http://www.cs.cmu.edu/afs/cs/user/yachen/include/pwd.h
// #include <pwd.h>
// http://man7.org/linux/man-pages/man0/grp.h.0p.html
// #include <grp.h>

#define MAX_FILESYS_SIZE 1024
char active_filesys[MAX_FILESYS_SIZE] = "";

int isdir(const char* path);

void catenater(int* marker, const char* datum);

void printi(const char* path, const char* fname, int indent);

void printe();

void filesystem(const char* path, int indent);

void helpme();

int main(int argc, char *argv[])
{
    int opth; // help
    int optI; // set indent
    int optL; // symbolic links
    int optt; // print info
    int optp; // print permission bits
    int opti; // print number of links
    int optu; // print file UID
    int optg; // print file GID
    int opts; // print file size
    int optd; // show time of last mod
    int optl; // print tpiugs info

    int indent_convert;
    char* indents;
    char* dirname;

    int it;
    int ch;

    int opth = 0; 
    int optI = 0; 
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

    while( (ch = getopt(argc,argv,"hI:Ltpiugsdl")) != -1 )
    {
        switch(ch)
        {
            case 'h':
                helpme();
                return 0;
            case 'I':
                optI = 1;
                indents = optarg;
                if(indents != NULL)
                {
                    indent_convert = atoi(indents);
                }
                if(indents == NULL)
                {
                    return -1;
                }
                break;
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
                optl = 1;
                optt = 1; 
                optp = 1; 
                opti = 1; 
                optu = 1; 
                optg = 1; 
                opts = 1; 
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
                if(optopt = 'I')
                {
                    printf("dt: missing argument <n> number of indent spaces.\n");
                    exit(0);
                }
                break;
        }
    }
    
    char cwd[PATH_MAX];

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
           perror("error");
           exit(0);
       }
    }
    //printf("dirname: %s", dirname);

    filesystem(dirname, 0);

    return 0;
}


int isdir(const char* path)
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

void catenater(int* marker, const char* datum)
{
    int kickoff = *marker;
    for(; *marker < kickoff+strlen(datum); ++*marker)
    {
        active_filesys[*marker] = datum[*marker - kickoff];
    }
    active_filesys[*marker + 1] = '\0';
}

void printi(const char *path, const char *fname, int indent)
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
       except that when path refers to
       a symbolic link, lstat() will r
       eturn info about the link    */
    if(lstat (path, &fstats) == -1)
    {
        perror("dt: error");
        exit(EXIT_FAILURE);
    }

    /* for loop includes indent properly */
    for(index = 0; index < indent; ++index)
    {
        active_filesys[index] = ' ';
    }

    /* for loop appends the proper filename   */
    for(; index < indent+strlen(fname); ++index)
    {
        active_filesys[index] = fname[index - indent];
    }
    active_filesys[index] = '\0';

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
    /* space the printed information prettily   */
    for(int jindex = index; jindex < 42; ++jindex)
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

        catenater(&index, "     ");
    }

    /* print the number of 
       links to file in in
       ode table        */
    if(opti == 1)
    {
        char charbuff[128];
        /* string print the
           numlinks instead
           of printing on t
           he console. stor
           es output on cha
           r buffer      */
        sprintf(charbuff, "%ld", fstats.st_nlink);
        catenater(&index, charbuff);
        catenater(&index, "     "); 
    }

    if(optu == 1)
    {
        struct passwd *pwd;

        if( (pwd = getpwuid(fstats.st_uid)) == NULL)
        {
            perror("dt: error");
        } else {
            catenater(&index, pwd->pw_name);
            catenater(&index, "     ");
        }
    }

    if(optg == 1)
    {
        struct group *grp;

        if( (grp = getgrgid(fstats.st_gid)) == NULL)
        {
            perror("dt: error");
        } else {
            catenater(&index, grp->gr_name);
            catenater(&index, "     ");
        }
    }

    if(opts == 1)
    {
        const int bytesize = fstats.st_size;
        /* 1 byte = 8 bis. 1 KB(K/Kb) = 2^10
           bytes = 1024 bytes. 1 MB(M/MB) =
           2^20 bytes = 1,048,576 bytes. 1 G
           B(G/GB) = 2^30 bytes = 1,073,741,
           824 bytes. these conversions   */
    }
}

void printe()
{
    int index;

    printf("%s\n", active_filesys);
    for(index = 0; index < MAX_FILESYS_SIZE; ++index)
    {
        active_filesys[index] = 0;
    }
}

void filesystem(const char* path, int indent)
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

    while( (de = readdir(dr) ) != NULL)
    {
        if(isdir(path))
        {
            char pathn[1024];
            if(strcmp (de->d_name, ".") == 0 || strcmp (de->d_name, "..") == 0)
            {
                continue;
            }
            /* int snprintf(char *str, size_t size, const char *format, ...) */
            snprintf(pathn, sizeof(pathn), "%s/%s", path, de->d_name);
            printi(pathn, de->d_name, indent);
            printe();
            /* recursive call to filesystem func   */
            filesystem(pathn, indent+indent_convert);
        } else {
            printi(de->d_name, de->d_name, indent);
            printe();
        }
    }
closedir(dr);
}

void helpme()
{
    printf("\n|HELP|MENU|");
    printf("\t-h : display help menu\n");
    printf("\t-I <n> : change indentation to <n> spaces for each level\n");
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
