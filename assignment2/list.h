#ifndef LIST_STRUCT
#define LIST_STRUCT

typedef struct linked_node {
    struct linked_node* next;
    struct linked_node* prev;
    void* content;
    int end;
    int start;
} linked_node_t;


linked_node_t* list_get_last_node(linked_node_t* node);
linked_node_t* list_get_start_node(linked_node_t* node);
linked_node_t* list_append_node(linked_node_t* node, void* data);
linked_node_t* list_create(void* data);
void list_free(linked_node_t* node);
void list_remove_last(linked_node_t* node);
void list_set_item(linked_node_t* node, int index, void* data);
void* list_get_item(linked_node_t* node, int index);
void* list_get_node(linked_node_t* node, int index);
linked_node_t* list_slice(linked_node_t* node, int start, int end);
unsigned list_length(linked_node_t* node);

#endif