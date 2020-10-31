#include <stdio.h>
#include <stdlib.h>
#include <string.h>



struct Node {
    
    void* data;
    struct Node* next;
};


void push(struct Node* head_ref, void* new_data, size_t data_size) {

    struct Node* tmp = head_ref;
    while (tmp->next != NULL) {
       tmp = tmp->next; 

    }

    struct Node* new_node = (struct Node*)malloc(sizeof(struct Node));
    
    tmp->next = new_node;
    new_node->data = malloc(data_size);
    new_node->next = NULL;
    
    memcpy(new_node->data, new_data, data_size);
}
