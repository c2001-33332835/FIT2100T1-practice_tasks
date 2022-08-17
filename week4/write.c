/* write.c */

#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <string.h>

int main(int argc, char *argv[])
{

  char* outputString = "Hello, world!\n";
  write(1, outputString, strlen(outputString));

  // Can we put the whole thing in one line like with a printf()?
  write(1, "This also works\n", 16);

  /*...but note that hard-coding the length like this is 
  not considered ideal coding practice for readability! */

  return 0;
}
