#include <stdio.h>
#include <stdlib.h>

#include "color_printf.h"
#include "custom_asserts.h"
#include "debug_macros.h"
#include "llist.h"

int main() {
    LinkedList llist = {};
    llistCtor(&llist, 10);

    pushBack(&llist, 10);
    pushBack(&llist, 20);
    pushBack(&llist, 30);
    pushBack(&llist, 40);
    pushBack(&llist, 50);
    pushBack(&llist, 60);
    pushBack(&llist, 70);
    insertAfter(&llist, 5, 0);
    insertAfter(&llist, 35, 3);
    insertAfter(&llist, 90, 7);
    popBack(&llist);
    popBack(&llist);
    popBack(&llist);
    popBack(&llist);
    insertAfter(&llist, 90, 4);
    insertAfter(&llist, 90, 4);
    insertAfter(&llist, 90, 4);
    insertAfter(&llist, 90, 4);
    insertAfter(&llist, 90, 4);

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

    return SUCCESS;
}

TYPE_OF_ERROR pushBack(LinkedList* llist, int element) {
    check_expression(llist, POINTER_IS_NULL);
    check_expression(llist->free != 0, VALUE_ERROR);

    insertAfter(llist, element, llist->prev[0]);

    return SUCCESS;
}

TYPE_OF_ERROR popBack(LinkedList* llist) {
    check_expression(llist, POINTER_IS_NULL);
    check_expression(llist->size != 0, POINTER_IS_NULL);

    erase(llist, llist->prev[0]);

    return SUCCESS;
}

TYPE_OF_ERROR insertAfter(LinkedList* llist, int element, int index) { //TODO rename index
    check_expression(llist,                              POINTER_IS_NULL);
    check_expression(llist->free != 0,                   VALUE_ERROR);
    check_expression(index       != llist->capacity - 1, VALUE_ERROR);

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

TYPE_OF_ERROR llistDump(LinkedList* llist) {
    check_expression(llist, POINTER_IS_NULL);

    FILE* dump_file = fopen("dump.dot", "w");

    warning(dump_file, FILE_OPEN_ERROR);

    //Header of graphviz file
    fprintf(dump_file, "digraph llist{\nsplines=ortho;\nrankdir=HR;\nnodesep=0.4;"
                       "\nnode [shape=record, fontname=\"Arial\"];\n"
                       "edge [style=bold, color=\"#009700:black;0.001\", weight=10, penwidth=2.5, "
                       "arrowsize=0.4];\n");

    //Output zero index (service element)
    fprintf(dump_file, "0 ");
    fprintf(dump_file, "[style = \"filled, rounded\", fillcolor=\"#1fcbf2\", label=\" {index = %d | data = %d | next = %d | prev = %d}\" ];\n", 0, llist->data[0], llist->next[0], llist->prev[0]);

    //Output all other elements
    int i = 1;
    for(i = 1; i < llist->capacity; i++) {
        fprintf(dump_file, "%d ", i);
        fprintf(dump_file, "[style = \"filled, rounded\", fillcolor=\"#f2291f\", label=\" {index = %d | data = %d | next = %d | prev = %d}\" ];\n", i, llist->data[i], llist->next[i], llist->prev[i]);
    }

    //Output size of linked-list
    fprintf(dump_file, "size ");
    fprintf(dump_file, "[style = \"filled, rounded\", fillcolor=\"#ffe41f\", label=\" {size = %d}\" ];\n", llist->size);

    //Output first free cell
    fprintf(dump_file, "free [style = \"filled, rounded\", fillcolor=\"#26e5a2\", label=\"free = %d\" ];\n", llist->free);

    //Change color of all free elements to green
    i = llist->free;
    while(i != 0) {
        fprintf(dump_file, "%d ", i);
        fprintf(dump_file, "[style = \"filled, rounded\", fillcolor=\"#26e5a2\"];\n");

        i = llist->next[i];
    }

    //Set rank
    fprintf(dump_file, "{ rank = same; ");
    for(i = 0; i < llist->capacity; i++) {
        fprintf(dump_file, "%d; ", i);
    }
    fprintf(dump_file, "}\n");

    //Set "next" relations (green arrows)
    fprintf(dump_file, "free->%d;\n", llist->free);
    for(i = 0; i < llist->capacity; i++) {
        if(i && llist->next[i]) {
            fprintf(dump_file, "%d->%d;\n", i, llist->next[i]);
        }
    }
    fprintf(dump_file, "0->%d;\n", llist->next[0]);
    fprintf(dump_file, "%d->0;\n", llist->prev[0]);

    //Change arrows color
    fprintf(dump_file, "edge [style=bold, color=\"#891728:black;0.001\", weight=0, penwidth=2.5, arrowsize=0.4];\n");

    //Set "prev" relations (red arrows)
    i = 0;
    do {
        fprintf(dump_file, "%d->%d;\n", i, llist->prev[i]);
        i = llist->prev[i];
    }while(i != 0);

    fprintf(dump_file, "}\n");

    fclose(dump_file);

    system("dot -Tpng dump.dot -o dump.png");

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
