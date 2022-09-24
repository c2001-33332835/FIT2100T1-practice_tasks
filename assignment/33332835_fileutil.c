/*

Copyright (c) 2022 Xuanao Zhao

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

*/

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
#define BUFFER_BLOCK_SIZE 32

// naming convensions: snake_case for globals/locals/statics and functions. CapitalizedName for struct names.
void error(char message[]){
    /*
        func purpose: To format and print a message as an error to the stderr.
        input as argument:
          - messages: an array of chars, the message to be printed.
        output:
          none
    */
   
    // all error will have the program name and a colon as it's prefix to show the error was originated from this program.
    write(STDERR, PROGRAM_NAME, strlen(PROGRAM_NAME));
    write(STDERR, ": ", 2);
    write(STDERR, message, strlen(message));
    write(STDERR, "\n", 1);
}

int initialize_file(int* file_fd, int file_mode, char* filename, int permission){
    /*
        func purpose: To open a file in a specific filemode, and permission if applicable. Also outputs error if something went wrong.
        input as argument:
          - file_mode: an int, the mode flags for the open function
          - filename: an array of chars, the filename to be opened
          - permission: an int, the permission flags for the open function
        output as diirect address access:
          - file_fd: an int, the file decriptor of the opened file
        output as return value:
          - an int (0 or 1). 1 for file successfuly opened. 0 for file open failed.
    */
    
    *file_fd = open(filename, file_mode, permission);
    if (*file_fd < 0){
        // show err msg if file is not accessable

        // construct message
        int msg_length = 51 + strlen(filename);
        char err_message[msg_length];
        strcpy(err_message, "");
        strcat(err_message, "cannot access '");
        strcat(err_message, filename);
        strcat(err_message, "': No such file or permission denied");

        // print message to stderr
        error(err_message);
        return false;
    }
    return true;
}

int test_numerical_string(char numstr[]){
    /*
        func purpose: to test if a give string only contains positive integer.
        input as argument:
          - numstr: an array of chars, the string to test.
        output as return value:
          - an int (0 or 1). 1 for test success. 0 for test failed.
    */
    int has_num = false;
    for(int i = 0; i < strlen(numstr); i++){
        if (numstr[i] > '9' || numstr[i] < '0'){
            return false;
        }
        else {
            has_num = true;
        }
    }
    return has_num;
}

int test_flag_a(char* c){
    /*
        func purpose: to test if a give string is FLAG_A.
        input as argument:
          - c: an array of chars, the string to test.
        output as return value:
          - an int (0 or 1). 1 for test success. 0 for test failed.
    */

    return strcmp(c, FLAG_A) == 0;
}

int test_flag_n(char* c){
    /*
        func purpose: to test if a give string is FLAG_N.
        input as argument:
          - c: an array of chars, the string to test.
        output as return value:
          - an int (0 or 1). 1 for test success. 0 for test failed.
    */

    return strcmp(c, FLAG_N) == 0;
}

int parse_arguments(int argc, char* argv[], char** source_filename, char** destination_filename, int* word_count, int* append_flag){
    /*
        func purpose: to parse the command line argument and resolve information for later use.
        input as argument:
          - argc: an int, the count of number of the command line arguments
          - argv: a 2D array of chars, a list of command line arguments as strings.
        output as diirect address access:
          - source_filename: an array of chars, the source filename if specified
          - destination_filename: an array of chars, the destination filename if specified
          - word_count: an int, the destination filename if specified
          - append_flag: an int (0 or 1), if append mode is specified by FLAG_A flag.
        output as return value:
          - an int (0 or 1). 1 for error occured during parsing. 0 parsing completed successfuly.
    */

    // if no arguments, return success
    if (argc <= 1){
        return 0;
    }

    // current arg index
    int current = 1;

    // check if the first argument has a source name
        
    if (!test_flag_a(argv[1]) && !test_flag_n(argv[1])){
        // assign source filename to the source_filename variable
        realloc(*source_filename ,sizeof(char) * strlen(argv[1]) + 2); // reallocate sourcefile to have the same length of argv1
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
        char* current_arg = argv[current];

        // if it is the first time encountering -a
        if (test_flag_a(current_arg) && !(in_a_flag || a_done)){
            // first time seeing -a
            in_a_flag = true;
            continue;
        }
        
        if (test_flag_a(current_arg) && a_done){
            // -a appread again while it is already defined, print error
            error("-a cannot be specified more then once");
            return true;
        }

        // if it is expecting a filename
        if (in_a_flag && !a_done){
            // if it recieved another flag
            if (test_flag_a(current_arg) || test_flag_n(current_arg)){
                error("filename expected after -a option");
                return true;
            }
            
            // set destination file name and set a to done.
            realloc(*destination_filename, sizeof(char) * strlen(current_arg) + 2);
            strcpy(*destination_filename, current_arg);
            *append_flag = true;
            a_done = true;
            continue;
        }

        // if it is the first time encountering -n
        if (test_flag_n(current_arg) && !(in_n_flag || n_done)){
            // first time seeing -n
            in_n_flag = true;
            continue;
        }

        if (test_flag_n(current_arg) && n_done){
            // -n appeared again while it is already defined, print error
            error("-n cannot be specified more then once");
            return true;
        }
        
        // if it is expecting a number
        if (in_n_flag && !n_done){
            // test if number is valid
            if (!test_numerical_string(current_arg)){
                // it contains other chars then positive integers.
                char* err = "' is not a valid positive number. Word count has to be a positive integer number.";//temp var stores the error msg;
                char err_full[strlen(err) + strlen(current_arg) + 1];//temp var stores the error msg;
                strcpy(err_full, "'");
                strcat(err_full, current_arg);
                strcat(err_full, err);
                error(err_full);
                return true;
            }

            // set the word count and n_done to true
            *word_count = atoi(current_arg);
            n_done = true;
            continue;
        }

        // check if argument does not fall into any categories.
        char* err = "unexpected argument: '"; //temp error
        char err_full[strlen(err) + strlen(current_arg) + 1];
        strcpy(err_full, err);
        strcat(err_full, current_arg);
        strcat(err_full, "'");
        error(err_full);
        return true;
    }

    // final check partially initalized flag
    if (in_a_flag && !a_done){
        // check for partially inited -a
        error("filename must be specified after -a option");
        return true;
    }
    if (in_n_flag && !n_done){
        // check for partially inited -n
        error("word count must be specified after -n option");
        return true;
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
    char* destination_filename = (char*) malloc(sizeof(char) + 1);
    char* source_filename = (char*) malloc(sizeof(char) * strlen(fallback_source_filename) + 1);
    strcpy(source_filename, fallback_source_filename);
    int word_count = fallback_word_count;
    if (parse_arguments(argc, argv, &source_filename, &destination_filename, &word_count, &append_flag)){
        // exit 1 if parse_arguments returned true; Error message is already written to stderr in the function.
        exit_status = 1;
        goto cleanup;
    };


    // determin if the destination file is the same as the source file
    int same_file = strcmp(source_filename, destination_filename) == 0;
    if (same_file){
        // if samefile, uncheck O_RDONLY and check O_RDWR
        source_file_mode = O_RDWR;
    }

    // prepare and open source file
    int fd_source_file;
    if(!initialize_file(&fd_source_file, source_file_mode, source_filename, 0)){
        // exit 1 if initalize_file returned false; Error message is already written to stderr in the function.
        exit_status = 1;
        goto cleanup;
    }

    // prepare and open destination file if applicable. Default to STDOUT.
    int fd_destination_file = STDOUT;
    if (append_flag && !same_file){
        if (!initialize_file(&fd_destination_file, destination_file_mode, destination_filename, destination_file_permission)){
            // exit 1 if initalize_file returned false; Error message is already written to stderr in the function.
            exit_status = 1;
            goto cleanup;
        }
    }

    // ===== MAIN PROGRAM FUNCTION


    int char_index = 0;
    // write(1, "a\n", 2);

    // define buffer, this buffer is for reading char by char.
    char buffer[1];
    int current_word_count = 0;
    int current_space = true; // check if there are multiple spaces occuring next to each other.
    while (current_word_count < word_count){
        int success_size = read(fd_source_file, buffer, 1);
        if (!success_size){
            break;
        }
        if (same_file){
            // seek the end to append
            lseek(fd_source_file, 0, SEEK_END);
            write(fd_source_file, buffer, 1);
            char_index ++;

            // seek back to beginning of file with offset to continue
            lseek(fd_source_file, char_index, SEEK_SET);
        }
        else {
            // directly write to file if input and output are different files.
            write(fd_destination_file, buffer, success_size);
        }

        // for checking words count
        // checks for spaces, LF, CR and HT
        if (current_space && (buffer[0] != ' ' && buffer[0] != '\n' && buffer[0] != '\r' && buffer[0] != '\t')){
            current_space = false;
        }
        if ((!current_space) && (buffer[0] == ' ' || buffer[0] == '\n' || buffer[0] == '\r' || buffer[0] == '\t')){
            current_space = true;
            current_word_count ++;
        }
    }

    // print notice if append mode
    if (append_flag){
        char* msg = "Append successful\n";
        write(STDOUT, msg, strlen(msg));
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

