#ifndef LIST_H
#define LIST_H

/* This is the implementation of a linked list interface defined in list.h.
 * This list has dynamic length of dynamic type, both node and item should be stored in heap.
 * (basically each list item is a void pointer)
 * Each list is defined by nodes pointing to each other, ordered.
 * In my implementation, any node from a list can represent the entire list itself.
 * 
 * This is the header file for interface and intellisense 
 * The implementation of this file is in list.c
 */

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
linked_node_t* list_push_front(linked_node_t* node, void* data);
linked_node_t* list_create(void* data);
linked_node_t* list_create_size(int size);
linked_node_t* list_create_empty();
void list_free(linked_node_t* node);
void list_free_nodes(linked_node_t* node);
void list_remove_last(linked_node_t* node);
void list_remove_last_node(linked_node_t* node);
linked_node_t* list_remove_item(linked_node_t* node, int index);
linked_node_t* list_remove_node(linked_node_t* node, int index);
void list_set_item(linked_node_t* node, int index, void* data);
void* list_get_item(linked_node_t* node, int index);
linked_node_t* list_get_node(linked_node_t* node, int index);
unsigned list_length(linked_node_t* node);
void list_reverse(linked_node_t* node);
void list_reverse_r(linked_node_t* node);
linked_node_t* list_duplicate(linked_node_t* node);

#endif