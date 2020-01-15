Purpose:
	The purpose of this program is to familiarize 
    yourself with using getopt and perror while p
    racticing system calls.

How to Run:	
	make		//to compile
	
	./dt <opt> [../filename]
	File name must be given in ../filename format or it won't be recognized as a directory
	./dt -h		//to print the help message
	./dt -I n	//to change indentation to n spaces for each level
	./dt -L		//to follow any symbolic links
	./dt -t		//to print information on file type
	./dt -p		//to print permission bits as rwxrwxrwx
	./dt -i		//to print the number of links to file in inode table
	./dt -u		//to print the UID associated with the file
	./dt -g		//to print the GID associated with the file
	./dt -s		//to print the size of the file in bytes
	./dt -d		//to show the time of the last modification
	./dt -l		//to print information on the file as if the options tpiugs are all specified

	make clean	//to remove all the object and executable files 