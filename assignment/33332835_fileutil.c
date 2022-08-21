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

// naming convensions: snake_case, "fd" prefix when it's file descriptor. Capitalisation for struct name

typedef struct {
    int* files;
    int file_count;
    void** pointers;
    int pointer_count;
} CleanupData;

typedef struct {
    void* data;
    LinkedNode* next;
    int is_end;
} LinkedNode;

LinkedNode* linked_list_get_last_node(LinkedNode* source){
    LinkedNode* node = source;
    while (!(node->is_end)){
        node = node->next;
    }
    return node;
}

LinkedNode* linked_list_append(LinkedNode* source, void* data){
    LinkedNode* last_node = linked_list_get_last_node(source);
    LinkedNode* new_node;
    new_node->data = data;
    new_node->is_end = true;
    last_node->next = new_node;
    last_node->is_end = false;
    return new_node;
}

int linked_list_length(LinkedNode* source){
    LinkedNode* node = source;
    int length = 1;
    while (!(node->is_end)){
        node = node->next;
        length++;
    }
    return length;
}

void cleanup(CleanupData* cleanup_data){
    // close all files and free all pointers.

    int* file_pointer = cleanup_data->files

    for (int i = 0; i < cleanup_data->file_count; i++){
        close(* file_pointer);
        file_pointer++;
    }

    void** pointer_pointer = cleanup_data->pointers; // pointer to pointers to be freed
    for (int i = 0; i < cleanup_data->pointer_count; i++){
        free(*pointer_pointer);
        pointer_pointer++;
    }
}

void die(char message[], int status, CleanupData* cleanup_data){
    // writes message to stderr and cleanup, then exit with status code passed in to this function

    write(STDERR ,PROGRAM_NAME, strlen(PROGRAM_NAME));
    write(STDERR ,": ", 2);
    write(STDERR, message, strlen(message));
    write(STDERR, "\n", 1);
    cleanup(cleanup_data);
    exit(status);
}

void initialize_file(int* file_fd, int file_mode, char* filename, int permission, CleanupData* cleanup_data){
    *file_fd = open(filename, file_mode, permission);
    if (*file_fd < 0){
        // show err msg and die if file is not accessable
        int msg_length = 17 + strlen(filename);
        char err_message[msg_length];
        strcpy(err_message, "");
        strcat(err_message, "cannot access '");
        strcat(err_message, filename);
        strcat(err_message, "'\n");
        die(err_message, 1, cleanup_data);
    }
}

int main(int argc, char* argv[]){
    // ===== PROGRAM PREPARE STAGE
    // define constants
    int source_file_mode = O_RDONLY;
    int destination_file_mode = O_WRONLY | O_CREAT;
    int destination_file_permission = 0664;
    CleanupData cleanup_data;
    cleanup_data.file_count = 0;
    cleanup_data.pointer_count = 0;
    
    // define fallback values for when they are not given through the cli argments.
    char fallback_source_filename[] = "sample.txt";
    int fallback_word_count = 10;

    // read and parse argv
    int append_flag = false;
    char destination_filename[] = "";
    char source_filename[strlen(fallback_source_filename)];
    strcpy(source_filename, fallback_source_filename);
    int word_count = fallback_word_count;

    // prepare and open source file
    int fd_source_file;
    initialize_file(&fd_source_file, source_file_mode, source_filename, &cleanup_data);
    cleanup_data.pointer_count

    // prepare and open destination file if applicable
    if (append_flag){
        int fd_destination_file;
        initialize_file(&fd_destination_file, destination_file_mode, destination_filename, destination_file_permission, &cleanup_data);
    }

    return 0;
}

