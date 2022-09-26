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

    if (end->empty){
        end->content = data;
        end->empty = 0;
        return end;
    }

    linked_node_t* new_node = (linked_node_t*) malloc(sizeof(linked_node_t));
    new_node->end = 1;
    new_node->prev = end;
    new_node->content = data;
    new_node->empty = 0;
    end->next = new_node;
    end->end = 0;
    return new_node;
}

linked_node_t* list_create(void* data){
    linked_node_t* new_node = (linked_node_t*) malloc(sizeof(linked_node_t));
    new_node->end = 1;
    new_node->start = 1;
    new_node->empty = 0;
    new_node->content = data;
    return new_node;
}

linked_node_t* list_create_size(int size){
    linked_node_t* new_node = (linked_node_t*) malloc(sizeof(linked_node_t));
    new_node->content = NULL;
    new_node->start = 1;
    new_node->end = 1;
    new_node->empty = 0;
    
    for (int i = 1; i < size; i ++){
        list_append_node(new_node, NULL);
    }
    
    return new_node;
}

linked_node_t* list_create_empty(){
    linked_node_t* new_node = (linked_node_t*) malloc(sizeof(linked_node_t));
    new_node->content = NULL;
    new_node->start = 1;
    new_node->end = 1;
    new_node->empty = 1;
    
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
    linked_node_t* target = list_get_node(node, index);
    if (target->empty){
        return;
    }
    
    target->content = data;
}

linked_node_t* list_get_node(linked_node_t* node, int index){
    linked_node_t* curr = list_get_start_node(node);

    for (int i = 0; i != index && !curr->end; i++){
        curr = curr->next;
    }
    return curr;
}

void* list_get_item(linked_node_t* node, int index){
    return list_get_node(node, index)->content;
}

unsigned list_length(linked_node_t* node){
    linked_node_t* curr = list_get_start_node(node);
    if (curr->empty){
        return 0;
    }
    
    unsigned len;
    for (len = 1; !curr->end; len++){
        curr = curr->next;
    }
    return len;
}

void list_reverse(linked_node_t* node){
    if (node->empty){
        return;
    }

    linked_node_t* start = list_get_start_node(node);
    list_reverse_r(start);
}

void list_reverse_r(linked_node_t* node){
    if (node->start){
        node->start = 0;
        node->end = 1;
    }
    else if (node->end){
        node->start = 1;
        node->end = 0;
    }

    // swap next and prev
    linked_node_t* temp = node->prev;
    node->prev = node->next;
    node->next = temp;

    if (node->start){
        return;
    }

    list_reverse_r(node->prev);
}

linked_node_t* list_duplicate(linked_node_t* node){
    linked_node_t* new = list_create_empty();
    for (int i = 0; i < list_length(node); i++){
        
    }
}