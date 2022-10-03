#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "logs.h"
#include "string_utils.h"

#define ESC 27
#define BLACK '0'
#define RED '1'
#define GREEN '2'
#define YELLOW '3'
#define BLUE '4'
#define MAGENTA '5'
#define WHITE '7'

void log_string(char* message, log_string_type_t log_type){
    char label[17];
    strcpy(label, "[ [3 ;1m  [0m]");
    label[1] = label[9] = ESC;
    switch(log_type){
        case LOG_STRDFLT:
            label[4] = GREEN;
            label[8] = 'i';
            break;
        case LOG_STRWARN:
            label[4] = YELLOW;
            label[8] = '*';
            break;
        case LOG_STRERR:
            label[4] = RED;
            label[8] = '!';
            break;
        case LOG_STRDBUG:
            label[4] = MAGENTA;
            label[8] = 'd';
            break;
    }

    char full_length = strlen(message) + strlen(label) + 3;
    char full_message[full_length];
    strcpy(full_message, label);
    strcat(full_message, " ");
    strcat(full_message, message);
    strcat(full_message, "\n");

    if (log_type == LOG_STRERR){
        fprintf(stderr, full_message);
        return;
    }
    printf(full_message);
};

void log_iteration(int iteration){
    printf("%c[30;1m[@t=%d]%c[0m\n", ESC, iteration, ESC);
}

void log_pcb(pcb_t* block, log_functional_type_t log_type, int t, int output_fd){
    char label[] = "  [3 ;1m  [0m ";
    label[1] = label[9] = ESC;

    char* name = block->process_name;
    int entry_time = block->entry_time;
    int service_time = block->service_time;
    int remaining_time = block->remaining_time;
    int deadline = block->deadline;
    int start_time = block->start_time;
    int exit_time = block->exit_time;
    int wait_time = start_time - entry_time;
    int turnaround_time = exit_time - entry_time;
    int deadline_missed = turnaround_time > deadline;

    if (log_type == LOG_EXIT){
        int wait_length;
        int turnaround_length;
        int_count_digits(wait_time, &wait_length);
        int_count_digits(wait_time, &turnaround_length);

        int base_log_length = strlen(name) + wait_length + turnaround_length + 7;
        char msg[base_log_length];
        sprintf(msg, "%s %d %d %d\n", name, wait_time, turnaround_time, !deadline_missed);
        write(output_fd, msg, strlen(msg));
    }

    switch(log_type){
        case LOG_ARRIVE:
            label[4] = BLUE;
            label[8] = '+';
            printf("%s %s Has entered the queue.\n", label, name);
            break;
        case LOG_START:
            label[4] = GREEN;
            label[8] = '>';
            printf("%s %s Has started executing.\n", label, name);
            break;
        case LOG_SUSPENSION:
            label[4] = RED;
            label[8] = '*';
            printf("%s %s Has temporarly suspended.\n", label, name);
            break;
        case LOG_RESUME:
            label[4] = GREEN;
            label[8] = '*';
            printf("%s %s Has resumed execution.\n", label, name);
            break;
        case LOG_EXIT:
            label[4] = RED;
            label[8] = '<';

            if (deadline_missed){
                printf("%s %s Has done execution and missed the deadline.\n", label, name);
            }
            else {
                printf("%s %s Has done execution and meets its deadline.\n", label, name);
            }
            break;
        case LOG_DEADLINEMISS:
            label[4] = RED;
            label[8] = '!';
            printf("%s %s has reached its deadline.\n", label, name);
            break;
    }
}