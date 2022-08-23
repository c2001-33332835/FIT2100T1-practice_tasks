#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#define STDIN 0
#define STDOUT 1
#define STDERR 2
#define true 1
#define false 0

#define PROGRAM_NAME "fileutil"
#define FLAG_A "-a"
#define FLAG_N "-n"

// naming convensions: snake_case for globals/locals/statics and functions. CapitalizedName for struct names.
void error(char message[]){
    // all error will have the program name and a colon as it's prefix to show the error was originated from this program.
    write(STDERR, PROGRAM_NAME, strlen(PROGRAM_NAME));
    write(STDERR, ": ", 2);
    write(STDERR, message, strlen(message));
}

int initialize_file(int* file_fd, int file_mode, char* filename, int permission){
    *file_fd = open(filename, file_mode, permission);
    if (*file_fd < 0){
        // show err msg if file is not accessable

        // construct message
        int msg_length = 52 + strlen(filename);
        char err_message[msg_length];
        strcpy(err_message, "");
        strcat(err_message, "cannot access '");
        strcat(err_message, filename);
        strcat(err_message, "': No such file or permission denied\n");

        // print message to stderr
        error(err_message);
        return 0;
    }
    return 1;
}

int test_numerical_string(char message[]){
    // 1:success, 0:failed
    int has_num = false;
    for(int i = 0; i < strlen(message); i++){
        if (message[i] > '9' || message[i] < '0'){
            return 0;
        }
        else {
            has_num = true;
        }
    }
    return has_num;
}

int test_flag_a(char* c){
    // 0: not flag, 1: is flag a
    return strcmp(c, FLAG_A) == 0;
}

int test_flag_n(char* c){
    // 0: not flag, 1: is flag n
    return strcmp(c, FLAG_N) == 0;
}

int parse_arguments(int argc, char* argv[], char** source_filename){
    // 0 success, 1 failed

    // if no arguments, return success
    if (argc <= 1){
        return 0;
    }

    // current arg index
    int current = 1;

    // check if the first argument has a source name
        
    if (!test_flag_a(argv[1]) && !test_flag_n(argv[1])){
        // assign source filename to the source_filename variable
        *source_filename = (char*) malloc(sizeof(char) * strlen(argv[1])); // reallocate sourcefile to have the same length of argv1
        strcpy(*source_filename, argv[1]);
        current ++;
    }

    // parse -a and -n flags
    int in_a_flag = false; // waiting for destination filename
    int in_n_flag = false; // waiting for word count in integer
    int a_done = false; // already collected for -a
    int n_done = false; // already collected for -n
    // for all other arguments
    // the variable "current" starts from 1 if there are no source file specified, starts from 2 if source file are specified
    for (; current < argc; current ++){
        // if it is the first time encountering -a
        char* current_arg = argv[current];
        if (test_flag_a(current_arg) || !(in_a_flag || a_done)){
            // first time seeing -a
            in_a_flag = true;
            continue;
        }
        
        if (test_flag_a(current_arg) && a_done){
            // -a appread again while it is already defined, print error
            char* e = "-a cannot be specified twice"; // temp var for err message
            
            return true;
        }

        // if it is expecting a filename
        if (in_a_flag && !a_done){

        }
    }

    return false;
}

int main(int argc, char* argv[]){
    // ===== PROGRAM PREPARE STAGE
    // define constants
    int source_file_mode = O_RDONLY;
    int destination_file_mode = O_WRONLY | O_CREAT | O_APPEND;
    int destination_file_permission = 0664;
    int exit_status = 0;
    
    // define fallback values for when they are not given through the cli argments.
    char fallback_source_filename[] = "sample.txt";
    int fallback_word_count = 10;

    // read and parse argv
    int append_flag = false;
    char* destination_filename = (char*) malloc(sizeof(char));
    char* source_filename = (char*) malloc(sizeof(char) * strlen(fallback_source_filename));
    strcpy(source_filename, fallback_source_filename);
    int word_count = fallback_word_count;
    if (parse_arguments(argc, argv, &source_filename)){
        goto cleanup;
    };

    // prepare and open source file
    int fd_source_file;
    if(!initialize_file(&fd_source_file, source_file_mode, source_filename, 0)){
        // exit 1 if initalize_file returned true; Error message is already written to stderr in the function.
        exit_status = 1;
        goto cleanup;
    }

    // prepare and open destination file if applicable. Default to STDOUT.
    int fd_destination_file = STDOUT;
    if (append_flag){
        if (!initialize_file(&fd_destination_file, destination_file_mode, destination_filename, destination_file_permission)){
            // exit 1 if initalize_file returned true; Error message is already written to stderr in the function.
            exit_status = 1;
            goto cleanup;
        }
    }

    // ===== MAIN PROGRAM FUNCTION

    char buffer[1];
    int current_word_count = 0;
    int current_space = true; // check if there are multiple spaces occuring next to each other.
    while (current_word_count < word_count){
        int success_size = read(fd_source_file, buffer, 1);
        if (!success_size){
            break;
        }
        write(fd_destination_file, buffer, success_size);
        if (current_space && (buffer[0] != ' ')){
            current_space = false;
        }
        if ((!current_space) && (buffer[0] == ' ')){
            current_space = true;
            current_word_count ++;
        }
    }

    goto cleanup;
    
    // ===== CLEANUP STAGE
    cleanup:

    // close source file, and append file if applicable
    close(fd_source_file); 
    if (append_flag) close(fd_destination_file);

    // free heap memories
    free(source_filename);
    free(destination_filename);

    // exit with exit status.
    return exit_status; 
}

