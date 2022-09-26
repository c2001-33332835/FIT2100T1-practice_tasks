#ifndef LIST_H
#define LIST_H

typedef struct linked_node {
    struct linked_node* next;
    struct linked_node* prev;
    void* content;
    int end;
    int start;
    int empty;
} linked_node_t;

linked_node_t* list_get_last_node(linked_node_t* node);
linked_node_t* list_get_start_node(linked_node_t* node);
linked_node_t* list_append_node(linked_node_t* node, void* data);
linked_node_t* list_create(void* data);
linked_node_t* list_create_size(int size);
linked_node_t* list_create_empty();
void list_free(linked_node_t* node);
void list_remove_last(linked_node_t* node);
void list_set_item(linked_node_t* node, int index, void* data);
void* list_get_item(linked_node_t* node, int index);
linked_node_t* list_get_node(linked_node_t* node, int index);
unsigned list_length(linked_node_t* node);
void list_reverse(linked_node_t* node);
linked_node_t* list_duplicate(linked_node_t* node);

#endif