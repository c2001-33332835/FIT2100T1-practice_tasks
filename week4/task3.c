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

  if (argc != 3) { /* Q: what does argc (argument count) mean? */ /* A: The count of ARGV (commandline arguments). The program will not work if the 2 extra arguments is not present (the 2 files) */
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

  // Open the first file for appending
  if ((outfile = open(argv[1], O_WRONLY | O_APPEND)) < 0) {
      perror(argv[1]);
      exit(1);
  }

  // copy the content of the first file to the second file.
  char buff[1];
  while (1){
    int status = read(infile, buff, 1);
    if (status == 0){
        break;
    }
    write(outfile, buff, 1);
  }

  // file close
  close(infile);
  close(outfile);

  exit(0);
}
