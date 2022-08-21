#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define STDIN 0
#define STDOUT 1
#define STDERR 3
#define true 1
#define false 0

int main(int argc, char* argv[]){
    // naming convensions: snake_case, "fd" prefix when it's file descriptor.

    // define fallback values for when they are not given through the cli argments.
    char fallback_source_filename = "sample.txt";
    int fallback_word_count = 10;
    
    // define variables when append mode is enabled
    int append_mode = false;
    int append_filename = "";

    // prepare and open source file.
    int fd_sourcefile = open()

    return 0;
}

