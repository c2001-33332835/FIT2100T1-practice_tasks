/* Short description of program:
 * This program takes a list of 4 predefined unit codes
 * For each unit code, the program will store the index as well as the code string into a struct.
 * The struct is then being written to a file named recordfile.
 * The program will create the file if it does not exist, or truncate the file if it does.
 * The content of the file will be precise bytes of the content of each struct.
 * The order of the codes being stored in the file will be exactly the same as how the list is being defined,
 * no matter in what order the for loop takes.
 * If there was any error occured when opening the file, the program should exit with status 1
 * or else it will exit successfuly with status 0 (after execution of all instructions)
 * 
 * Please see detailed description of each procedure of program below, 
 * or the CONTENT OF THE OUTPUTTED FILE at the end of this file
 */

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/file.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

// define a struct to store unit id and unit code
struct Record {
  int unitid;
  char unitcode[8];
};

// define a list of predefined units
char *unitcodes[] = {"FIT2100", "FIT3159", "FIT3142","FIT3165"};

int main(int argc, char *argv[])
{
  // define 2 variables. 
  //  - i: the variable for loop index (which happens later in the app).
  //  - outfile: the file descriptor to be open by the application.
  int i, outfile;

  // defines eachrec to temporarly store the value of each predefined units during the loop
  // so that it can be written to the file.
  struct Record eachrec;

  // open the output file "recordfile" with write mode, while create and truncate the file no matter if it exists or not.
  // if the file descrioptor is less then 0, then there are something wrong, exit with status code of 1.
  if ((outfile = open("recordfile", O_WRONLY | O_CREAT | O_TRUNC, 0664)) < 0) {
    exit(1);
  }

  // i dont know why would you do this, but according to how this loop is defined,
  // the index will go from 3 -> 1, and when it's 1, it will ge resets to 4 by the if statement, then -2 will get 2,
  // then 0, then end of for loop.
  // So the actual index is 3 -> 1 -> 2 -> 0. (How ever this doesn't effect what its about to do in the loop)
  for (i = 3; i >= 0; i -=2) {

    // temporarly store the unit code and the unit id to the struct.
    // the unit id is literarly the index of the code in that list defined eariler.
    eachrec.unitid = i;
    strcpy(eachrec.unitcode, unitcodes[i]);

    // basically creating slots for creating the different unit codes.
    // each slot are in the size of the Record struct. Seek for the struct of that index (unit id),
    // and write the content of the struct into the file. (See the end of this file)
    lseek(outfile, (long) i * sizeof(struct Record), SEEK_SET);
    write(outfile, &eachrec, sizeof(struct Record));

    // reset the index to 4 (and instently get minused by 2) when i reached 1.
    if (i == 1) {
      i = 4;
    }
  }

  // close the file
  close(outfile);
  // exit with success status (0)
  exit(0);
}


// the following is the content of recordfile after execution of this program
// (Compiled and Executed on Windows Subsystem for Linux 2 with Arch Linux as the distribution)
// ┌────────┬─────────────────────────┬─────────────────────────┬────────┬────────┐
// │00000000│ 00 00 00 00 46 49 54 32   31 30 30 00 01 00 00 00 │0000FIT2 1000•000│
// │00000010│ 46 49 54 33 31 35 39 00   02 00 00 00 46 49 54 33 │FIT31590 •000FIT3│
// │00000020│ 31 34 32 00 03 00 00 00   46 49 54 33 31 36 35 00 │1420•000 FIT31650│
// └────────┴─────────────────────────┴─────────────────────────┴────────┴────────┘
//
// As you can see it takes up 12 bytes for each struct. 4 bytes occupied by the index (unit id) and 8 bytes for unit code
// while the last byte of unitcode is always null byte (00).
// The content may very based on the operating system the program is being compiled and executed
