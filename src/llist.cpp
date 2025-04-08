#include <stdio.h>
#include <stdlib.h>

#include "color_printf.h"
#include "custom_asserts.h"
#include "debug_macros.h"
#include "llist.h"
#include "llistDump.h"

int main() {
    LinkedList llist = {};
    llistCtor(&llist, 10);

    pushBack(&llist, 10);
    pushBack(&llist, 20);
    pushBack(&llist, 30);
    pushBack(&llist, 40);
    // pushBack(&llist, 50);
    // pushBack(&llist, 60);
    // pushBack(&llist, 70);
    llistDump(&llist);

    llistDtor(&llist);
}

TYPE_OF_ERROR llistCtor(LinkedList* llist, int capacity) {
    check_expression(llist, POINTER_IS_NULL);
    check_expression(capacity > 0, VALUE_ERROR);

    int actual_capacity = capacity + 1;
    llist->data = (int*)calloc(capacity, sizeof(int));
    warning(llist->data, CALLOC_ERROR);

    llist->next = (int*)calloc(capacity, sizeof(int));
    warning(llist->next, CALLOC_ERROR);

    llist->prev = (int*)calloc(capacity, sizeof(int));
    warning(llist->prev, CALLOC_ERROR);

    llist->capacity = actual_capacity;
    llist->size     = 0;
    llist->free     = 1;

    for(int i = 0; i < llist->capacity - 1; i++) {
        llist->next[i] = i + 1;
    }
    llist->next[llist->capacity - 1] = 0;

    for(int i = 1; i < llist->capacity; i++) {
        llist->prev[i] = -1;
        llist->data[i] = -1;
    }

    llist->prev[0] = 0;
    llist->next[0] = 0;

    setDumpFile(llist);

    return SUCCESS;
}

TYPE_OF_ERROR pushBack(LinkedList* llist, int element) { // TODO linkedListPushBack
    check_expression(llist, POINTER_IS_NULL);
    if(llist->free == 0) {
        color_printf(RED_TEXT, BOLD, "ERROR: Linked-List is full\n");

        check_expression(llist->free != 0, VALUE_ERROR);
    }

    insertAfter(llist, element, llist->prev[0]);

    return SUCCESS;
}

TYPE_OF_ERROR popBack(LinkedList* llist) {
    check_expression(llist, POINTER_IS_NULL);
    if(llist->size == 0) {
        color_printf(RED_TEXT, BOLD, "ERROR: Linked-List is empty\n");

        check_expression(llist->size != 0, POINTER_IS_NULL);
    }

    erase(llist, llist->prev[0]);

    return SUCCESS;
}

TYPE_OF_ERROR insertAfter(LinkedList* llist, int element, int index) { //TODO rename index
    check_expression(llist, POINTER_IS_NULL);
    if(llist->free == 0) {
        color_printf(RED_TEXT, BOLD, "ERROR: Linked-List is full\n");

        check_expression(llist->free != 0, VALUE_ERROR); //TODO if ndebug it will turn off
    }
    if(llist->prev[index] == -1) {
        color_printf(RED_TEXT, BOLD, "ERROR: You trying to insert element after free cell\n");

        check_expression(llist->prev[index] != -1, INPUT_ERROR);
    }

    //Add element to data
    llist->data[llist->free] = element;

    //Connect index+1 element with first free cell (prev = free)
    llist->prev[llist->next[index]] = llist->free;

    //Change prev of first free cell to index (prev = index)
    llist->prev[llist->free] = index;

    //Change index of first free cell
    llist->free = llist->next[llist->free];

    //Change index "next" of inserted element on next logical cell
    llist->next[llist->prev[llist->next[index]]] = llist->next[index];

    //Change index "next" of element before inserted
    llist->next[index] = llist->prev[llist->next[index]];

    //Size up
    llist->size++;

    return SUCCESS;
}

TYPE_OF_ERROR pushFront(LinkedList* llist, int element) {
    check_expression(llist, POINTER_IS_NULL);
    if(llist->free == 0) {
        color_printf(RED_TEXT, BOLD, "ERROR: Linked-List is full\n");

        check_expression(llist->free != 0, VALUE_ERROR);
    }

    insertAfter(llist, element, 0);

    return SUCCESS;
}

TYPE_OF_ERROR erase(LinkedList* llist, int index) {
    check_expression(llist, POINTER_IS_NULL);

    //Poison data value
    llist->data[index] = -1;

    //Next of prev becomes next of deleted index
    llist->next[llist->prev[index]] = llist->next[index];

    //Prev of next becomes prev of deleted index
    llist->prev[llist->next[index]] = llist->prev[index];

    //Make next of deleted index first free cell (include it to the free-elements chain)
    llist->next[index] = llist->free;

    //Prev value of free element is -1
    llist->prev[index] = -1;

    //Change first free cell on deleted index
    llist->free = index;

    //Size down
    llist->size--;

    return SUCCESS;
}

TYPE_OF_ERROR llistDtor(LinkedList* llist) {
    llist->capacity = 0;
    llist->free     = 0;
    llist->size     = 0;
    free(llist->data);
    free(llist->next);
    free(llist->prev);

    return SUCCESS;
}
