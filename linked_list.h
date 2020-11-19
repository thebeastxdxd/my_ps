#ifndef __LINKED_LIST_H__
#define __LINKED_LIST_H__

struct Node {
    
    void* data;
    struct Node* next;
};


struct Node** list_init(void* new_data);
void list_push(struct Node** head_ref, void* new_data);
void list_free(struct Node** head_ref);
void list_print(struct Node** head_ref, void (*print)(void*));

#endif //__LINKED_LIST_H__
