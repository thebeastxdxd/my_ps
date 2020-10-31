#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "linked_list.h"


struct Node** list_init(void* new_data) {

    struct Node** head_ptr = NULL;
    struct Node* head = NULL;
    head_ptr = (struct Node**)malloc(sizeof(struct Node*));
    if (head_ptr == NULL) {
       return NULL; 
    }

    head = (struct Node*)malloc(sizeof(struct Node));
    if (head == NULL) {
        free(head_ptr);
        return NULL;
    }
    *head_ptr = head;
    head->data = new_data;
    head->next = NULL;

    return head_ptr;
}

void list_push(struct Node** head_ref, void* new_data) {

    struct Node* tmp = NULL;
    struct Node* new_node = NULL;
    if (head_ref == NULL || *head_ref == NULL) {
        // TODO: hm?
    }

    tmp = *head_ref; 
    if (tmp->data == NULL) {
        tmp->data = new_data;

    } else {
            
        while (tmp->next != NULL) {
           tmp = tmp->next; 
        }

        new_node = (struct Node*)malloc(sizeof(struct Node));
        if (new_node == NULL) {
            // TODO: hm? 
        }
        
        tmp->next = new_node;
        new_node->data = new_data;
        if (new_node == NULL) {
            // TODO: hm?
        }
        new_node->next = NULL;
    }
}

void list_free(struct Node** head_ref) {
    struct Node* curr = *head_ref;
    struct Node* next = NULL;

    while (curr != NULL) {
        next = curr->next;
        free(curr->data);
        free(curr);
        curr = next;

    }
    free(head_ref);
}

void list_print(struct Node** head_ref, void (*print)(void *)) {

    struct Node* curr = *head_ref;
    while (curr != NULL) {
        print(curr->data);
        curr = curr->next;
    }
}
