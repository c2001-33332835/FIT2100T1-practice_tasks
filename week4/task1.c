/* task3.c: append the contents of the second file to the first file */

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/file.h>		/* change to <sys/fcntl.h> for System V */
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>      /* needed for perror function */

int main (int argc, char *argv[])
{
  int n, infile, outfile;
  char buffer[1024];

  if (argc != 3) { /* Q: what does argc (argument count) mean? */
      write(2, "Usage: ./appendfile file1 file2\n", 32);
      exit(1);
  }

  /*
  * Open the second file (file2) for reading
  */
  if ((infile = open(argv[2], O_RDONLY)) < 0) {
      perror(argv[2]);
      exit(1);
  }

  /*
  * CODE HERE: Open the first file (file1) for writing
  */

  /*
  * CODE HERE: Copy data from the second file to the first file
  */

  /* 
  * CODE HERE: Close files before exiting
  */

  exit(0);
}