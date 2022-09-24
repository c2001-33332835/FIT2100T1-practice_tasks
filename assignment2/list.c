#include "list.h"
#include "stdlib.h"

linked_node_t* list_get_last_node(linked_node_t* node){
    linked_node_t* curr = node;
    while (!curr->end){
        curr = curr->next;
    }
    return curr;
}

linked_node_t* list_get_start_node(linked_node_t* node){
    linked_node_t* curr = node;
    while (!curr->start){
        curr = curr->prev;
    }
    return curr;
}

linked_node_t* list_append_node(linked_node_t* node, void* data){
    linked_node_t* end = list_get_last_node(node);
    linked_node_t* new_node = (linked_node_t*) malloc(sizeof(linked_node_t));
    new_node->end = 1;
    new_node->prev = end;
    new_node->content = data;
    end->next = new_node;
    end->end = 0;
    return new_node;
}

linked_node_t* list_create(void* data){
    linked_node_t* new_node = (linked_node_t*) malloc(sizeof(linked_node_t));
    new_node->end = 1;
    new_node->start = 1;
    new_node->content = data;
    return new_node;
}

void list_free(linked_node_t* node){
    linked_node_t* start = list_get_start_node(node);
    while (!start->end){
        list_remove_last(start);
    }
    list_remove_last(start);
}

void list_remove_last(linked_node_t* node){
    linked_node_t* end = list_get_last_node(node);
    if (!end->start){
        end->prev->end = 1;
    }
    free(end->content);
    free(end);
}

void list_set_item(linked_node_t* node, int index, void* data){
    
}

void* list_get_node(linked_node_t* node, int index){

}

void* list_get_item(linked_node_t* node, int index){

}

linked_node_t* list_slice(linked_node_t* node, int start, int end){

}

unsigned list_length(linked_node_t* node){

}