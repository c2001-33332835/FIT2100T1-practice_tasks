#include "list.h"
#include "stdlib.h"

/* This is the implementation of a linked list interface defined in list.h.
 * This list has dynamic length of dynamic type, both node and item should be stored in heap.
 * (basically each list item is a void pointer)
 * Each list is defined by nodes pointing to each other, ordered.
 * In my implementation, any node from a list can represent the entire list itself.
 */

linked_node_t* list_get_last_node(linked_node_t* node){
    /* Given any node form a list in argument,
     * Return the pointer to the last node in the list.
     */
    linked_node_t* curr = node;
    while (!curr->end){
        curr = curr->next;
    }
    return curr;
}

linked_node_t* list_get_start_node(linked_node_t* node){
    /* given any node from a list in the argument,
     * Return the pointer to the first node in the list.
     */
    linked_node_t* curr = node;
    while (!curr->start){
        curr = curr->prev;
    }
    return curr;
}

linked_node_t* list_append_node(linked_node_t* node, void* data){
    /* Given any node from a as the argument,
     * Append data (pointer to data) from argument to the end of the list.
     * Return the newly created node
     */
    linked_node_t* end = list_get_last_node(node);

    if (end->empty){
        end->content = data;
        end->empty = 0;
        return end;
    }

    linked_node_t* new_node = (linked_node_t*) malloc(sizeof(linked_node_t));
    new_node->end = 1;
    new_node->start = 0;
    new_node->prev = end;
    new_node->content = data;
    new_node->empty = 0;
    end->next = new_node;
    end->end = 0;
    return new_node;
}

linked_node_t* list_push_front(linked_node_t* node, void* data){
    /* Given any node from a as the argument,
     * Push data (pointer to data) from argument to the beginning of the list.
     * Return the newly created node
     */
    linked_node_t* start = list_get_start_node(node);

    if (start->empty){
        start->content = data;
        start->empty = 0;
        return start;
    }

    linked_node_t* new_node = (linked_node_t*) malloc(sizeof(linked_node_t));
    new_node->start = 1;
    new_node->end = 0;
    new_node->next = start;
    new_node->content = data;
    new_node->empty = 0;
    start->start = 0;
    start->prev = new_node;
    return new_node;
}

linked_node_t* list_create(void* data){
    /* Accepts a data (pointer to a data)
     * creates a list in heap containg only the data
     * The return type is a pointer to the newly created list.
     */
    linked_node_t* new_node = (linked_node_t*) malloc(sizeof(linked_node_t));
    new_node->end = 1;
    new_node->start = 1;
    new_node->empty = 0;
    new_node->content = data;
    return new_node;
}

linked_node_t* list_create_size(int size){
    /* Accepts an integer as the size of the list
     * creates an uninitialised list of the given size in the heap.
     * The return type is a pointer to the newly created list.
     */
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
    /* Creates an empty list in the heap.
     * The return type is a pointer to the newly created list.
     */
    linked_node_t* new_node = (linked_node_t*) malloc(sizeof(linked_node_t));
    new_node->content = NULL;
    new_node->start = 1;
    new_node->end = 1;
    new_node->empty = 1;
    
    return new_node;
}

void list_free(linked_node_t* node){
    /* Accepts any node from a list.
     * Free all nodes and data* (the content) of the given list from heap.
     */
    
    // check if empty
    if (node->empty){
        free(node);
        return;
    }

    linked_node_t* start = list_get_start_node(node);
    while (!start->end){
        list_remove_last(start);
    }
    list_remove_last(start);
}

void list_free_nodes(linked_node_t* node){
    /* Accepts any node from a list.
     * Free only the nodes (not the content) of the given list from heap.
     */

    linked_node_t* start = list_get_start_node(node);
    while (!start->end){
        list_remove_last_node(start);
    }
    list_remove_last_node(start);
}

void list_remove_last(linked_node_t* node){
    /* Accepts any node from a list
     * Remove the last node from the list (including the content)
     * Free the last node and the content
     */
    linked_node_t* end = list_get_last_node(node);
    if (!end->start){
        end->prev->end = 1;
    }
    free(end->content);
    free(end);
}

void list_remove_last_node(linked_node_t* node){
    /* Accepts any node from a list
     * Remove only the last node from the list
     * Free the last node and *not the content
     */
    
    linked_node_t* end = list_get_last_node(node);
    if (!end->start){
        end->prev->end = 1;
    }
    free(end);
}

linked_node_t* list_remove_item(linked_node_t* node, int index){
    /* Accepts any node from a list, and an index to be removed
     * Remove only the node and the content in the position.
     * Free the node and the content from heap.
     * Return a new pointer to the list since the pointer may be freed from memory.
     */

    linked_node_t* target = list_get_node(node, index);
    
    // check if empty
    if (target->empty){
        return target;
    }

    free(target->content);
    return list_remove_node(target, index);
}

linked_node_t* list_remove_node(linked_node_t* node, int index){
    /* Accepts any node from a list, and an index to be removed
     * Remove only the node at the position.
     * Free the node and *not the content from heap.
     * Return a new pointer to the list since the pointer may be freed from memory.
     */

    linked_node_t* target = list_get_node(node, index);
    
    // check if empty
    if (target->empty){
        return target;
    }

    // check if 1 element
    if (list_length(target) == 1){
        target->empty = 1;
        target->start = 1;
        target->end = 1;
        return target;
    }

    // check if start
    if (target->start){
        linked_node_t* next = target->next;
        next->start = 1;
        free(target);
        return next;
    }

    // check if end
    if (target->end){
        linked_node_t* prev = target->prev;
        prev->end = 1;
        free(target);
        return prev;
    }

    // target is in middle, connect next and prev
    target->next->prev = target->prev;
    target->prev->next = target->next;
    free(target);
    return list_get_start_node(target);
}

void list_set_item(linked_node_t* node, int index, void* data){
    /* Accepts any node from a list, an integer index, and a data to be set.
     * Sets the content of the list at the given index to given data.
     * data should always be a pointer of a heap memory address
     */

    linked_node_t* target = list_get_node(node, index);
    if (target->empty){
        return;
    }
    
    target->content = data;
}

linked_node_t* list_get_node(linked_node_t* node, int index){
    /* Accepts any node from a list, and an integer index.
     * Gets the node of the list at the given index.
     * If index is larger then list lenght, then last node is given.
     * Returns a pointer to that node
     */
    linked_node_t* curr = list_get_start_node(node);

    for (int i = 0; i != index && !curr->end; i++){
        curr = curr->next;
    }
    return curr;
}

void* list_get_item(linked_node_t* node, int index){
    /* Accepts any node from a list, and an integer index.
     * Gets the content of the list at the given index.
     * If index is larger then list lenght, then last item is given.
     * Returns a pointer to the content of the item at index.
     */
    return list_get_node(node, index)->content;
}

unsigned list_length(linked_node_t* node){
    /* Accepts any node from a list.
     * Returns an unsigned int of the length of the list.
     */
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
    /* Accepts any node from a list.
     * Reverse the content of the list,
     * ie. the first item will become the last item and etc.
     */
    if (node->empty){
        return;
    }

    linked_node_t* start = list_get_start_node(node);
    list_reverse_r(start);
}

void list_reverse_r(linked_node_t* node){
    /* Accpets the FIRST NODE of a list.
     * Recursivly reverse the prev and next addresses to reverse the list.
     */
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
    /* Accepts any node from a list.
     * the pointer of content is copied to another newly created list in heap.
     * Return the pointer to the new list.
     * Only list is copied, content is not copied
     * (2 lists containing same set of pointers).
     */
    linked_node_t* new = list_create_empty();
    for (int i = 0; i < list_length(node); i++){
        list_append_node(new, list_get_item(node, i));
    }
    return new;
}