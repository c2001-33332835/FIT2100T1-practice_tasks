#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define STDIN 0
#define STDOUT 1
#define STDERR 2
#define true 1
#define false 0

#define PROGRAM_NAME "fileutil"

// naming convensions: snake_case for globals/locals/statics and functions. CapitalizedName for struct names.

// Impelementation of Linked List: LinkedNode is the nodes of the list.
typedef struct {
    void* data; // the actual element of the list
    struct LinkedNode* next; // points to the next element of the list
    int is_end; // if the node is at the end of a list
    int is_null; // if the node is null (happens when the list is newly created and no value is provided)
} LinkedNode;

void initialize_new_node(LinkedNode* node){
    // initialization of a new node as a new list
    // PARAM:
    //   - node: takes in a newly defined node, and initialize required values
    // RETURN:
    //   void.

    node->is_end = true;
    node->is_null = true;
}

void initialize_new_append_node(LinkedNode* node){
    // initialization of a new node to be appended to another node
    // PARAM:
    //   - node: takes in a newly defined node, and initialize required values
    // RETURN:
    //   void.

    node->is_end = true;
    node->is_null = false;
}

LinkedNode* linked_list_get_last_node(LinkedNode* start){
    // Gets the last node of a given starting node. 
    // PARAM:
    //   - start: a reference of the starting node to search
    // RETURN:
    //   - the reference of the last node. Returns the starting node if the list is still null, or has one element only.

    if (start->is_null) {
        return start;
    }

    LinkedNode* result = start;
    while (!(result->is_end)){
        result = result->next;
    }
    return result;
}

void linked_list_append_node(LinkedNode* start, LinkedNode* new, void* value){
    // Append a value to the linked list to the ned of the list or updates the given node if the list is null.
    // PARAM:
    //   - start: takes in any node from the list
    //   - new: the address of the new node to be appended
    //   - value the value to be added to the new node
    // RETURN:
    //   - void.

    LinkedNode* target;
    if (start->is_null){
        target = start;
        printf("1");
    }
    else {
        target = &new;
        linked_list_get_last_node(start)->is_end = false;
        linked_list_get_last_node(start)->next = target;
        printf("2");
    }
    initialize_new_append_node(target);
    target->data = value;
    return target;
}


int linked_list_length(LinkedNode* start){
    // Finds out the length between the starting point and the end of the list.
    // PARAM:
    //   - start: a reference of any node in the list (give the absolute starting node of the list if the length of the whole list is needed).
    // RETURN:
    //   - the length between the starting node and the ending node.
    if (start->is_null) {
        return 0;
    }

    int count = 1;
    LinkedNode* result = start;
    while (!(result->is_end)){
        result = result->next;
        count ++;
    }
    return count;
}

// A structure to store the cleanup information including a list of file descriptors and a list of pointers.
typedef struct {
    LinkedNode* files; // files to be closed
    LinkedNode* pointers; // pointers to be free
} CleanupData;


void cleanup(CleanupData* cleanup_data){
    // close all files and free all pointers.
    // PARAM:
    //   - cleanup_data: a reference of the CleanupData for this program.
    // RETURN:
    //   - void.

    int files_len = linked_list_length(cleanup_data->files);
    LinkedNode* files_node = cleanup_data->files;
    for (int i = 0; i < files_len; i++){
        int* file_desc = (int*) files_node->data;
        close(*file_desc);
        files_node = files_node->next;
    }

    int pointers_len = linked_list_length(cleanup_data->pointers);
    LinkedNode* pointers_node = cleanup_data->pointers;
    for (int i = 0; i < pointers_len; i++){
        free(pointers_node->data);
        pointers_node = pointers_node->next;
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
    printf("a");
    // ===== PROGRAM PREPARE STAGE
    // define constants
    int source_file_mode = O_RDONLY;
    int destination_file_mode = O_WRONLY | O_CREAT;
    int destination_file_permission = 0664;

    // define cleanup struct. files and pointers in this struct will be closed and freed automatically by cleanup()
    CleanupData cleanup_data;
    LinkedNode files_descriptors;
    LinkedNode heap_pointers;
    initialize_new_node(&files_descriptors);
    initialize_new_node(&heap_pointers);
    cleanup_data.files = &files_descriptors;
    cleanup_data.pointers = &heap_pointers;
    
    // define fallback values for when they are not given through the cli argments.
    char fallback_source_filename[] = "sample.txt";
    int fallback_word_count = 10;

    // read and parse argv
    int append_flag = false;
    char destination_filename[] = "1";
    char source_filename[strlen(fallback_source_filename)];
    strcpy(source_filename, fallback_source_filename);
    int word_count = fallback_word_count;

    // prepare and open source file
    int fd_source_file;
    initialize_file(&fd_source_file, source_file_mode, source_filename, 0, &cleanup_data);
    LinkedNode fd_source_file_node;
    linked_list_append_node(&files_descriptors, &fd_source_file_node, &fd_source_file);

    // prepare and open destination file if applicable
    int fd_destination_file;
    LinkedNode fd_destination_file_node;
    if (!append_flag){
        initialize_file(&fd_destination_file, destination_file_mode, destination_filename, destination_file_permission, &cleanup_data);
        linked_list_append_node(&files_descriptors, &fd_destination_file_node, &fd_destination_file);
    }

    return 0;
}

