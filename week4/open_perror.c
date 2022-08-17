/* open_perror.c */

#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
  char file_name[] = "non_existent.txt";

  /* Assume the file non_existent.txt is indeed non-existent */
  int fd = open(file_name, O_RDONLY);

  if (fd < 0)
  {
    perror("Failed to open the file");
  }

  close(fd);
  return 0;
}
