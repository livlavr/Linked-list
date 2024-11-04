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
    pushBack(&llist, 40);
    pushBack(&llist, 40);
    erase(&llist, 3);



    llistDump(&llist);

    llistDtor(&llist);
}

TYPE_OF_ERROR pushBack(LinkedList* llist, int element) {
    check_expression(llist, POINTER_IS_NULL);
    check_expression(llist->free != 0, VALUE_ERROR);

    int index                   = llist->free;
    llist->free                 = llist->next[llist->free];

    llist->data[index]          = element;
    llist->next[index]          = 0;
    llist->prev[index]          = llist->prev[0];

    llist->next[llist->prev[0]] = index;
    llist->prev[llist->free]    = 0;
    llist->prev[0]              = index;

    return SUCCESS;
}

TYPE_OF_ERROR popBack(LinkedList* llist) {
    check_expression(llist, POINTER_IS_NULL);
    check_expression(llist->free != 0, VALUE_ERROR);

    return SUCCESS;
}

TYPE_OF_ERROR insertAfter(LinkedList* llist, int element, int index) {
    check_expression(llist, POINTER_IS_NULL);
    check_expression(llist->free != 0, VALUE_ERROR);
    check_expression(index != llist->capacity - 2, VALUE_ERROR);
    check_expression(index != llist->capacity - 2, VALUE_ERROR);
    check_expression(index != llist->capacity - 2, VALUE_ERROR);

    int actual_index     = index + 1;
    int next_after_index = llist->next[actual_index];

    llist->data[llist->free]               = element;
    llist->prev[llist->next[llist->free]]  = 0;
    llist->next[actual_index]              = llist->free;
    llist->free                            = llist->next[llist->free];
    llist->next[llist->next[actual_index]] = next_after_index;
    llist->prev[llist->next[actual_index]] = actual_index;
    llist->prev[llist->next[llist->next[actual_index]]] = llist->next[actual_index];

    return SUCCESS;

}

TYPE_OF_ERROR erase(LinkedList* llist, int index) {
    check_expression(llist, POINTER_IS_NULL);

    llist->data[index] = -1;
    llist->next[llist->prev[index]] = llist->next[index];
    llist->prev[llist->next[index]] = llist->prev[index];
    llist->next[index] = llist->free;
    llist->prev[index] = 0;
    llist->prev[llist->next[index]] = index;
    llist->free = index;

    return SUCCESS;
}

TYPE_OF_ERROR llistDump(LinkedList* llist) {
    check_expression(llist, POINTER_IS_NULL);

    FILE* dump_file = fopen("dump.dot", "w");

    warning(dump_file, FILE_OPEN_ERROR);

    fprintf(dump_file, "digraph llist{\nsplines=ortho;\nrankdir=HR;\nnodesep=0.4;\nnode [shape=record, fontname=\"Arial\"];\n"
                       "edge [style=bold, color=\"#009700:black;0.001\", weight=0, penwidth=2.5, "
                       "arrowsize=0.4];\n");

    fprintf(dump_file, "0 ");
    fprintf(dump_file, "[style = \"filled, rounded\", fillcolor=\"#1fcbf2\", label=\" {index = %d | data = %d | next = %d | prev = %d}\" ];\n", 0, llist->data[0], llist->next[0], llist->prev[0]);
    int i = 0;
    for(i = 1; i < llist->capacity; i++) {
        fprintf(dump_file, "%d ", i);
        fprintf(dump_file, "[style = \"filled, rounded\", fillcolor=\"#f2291f\", label=\" {index = %d | data = %d | next = %d | prev = %d}\" ];\n", i, llist->data[i], llist->next[i], llist->prev[i]);
    }

    fprintf(dump_file, "free [style = \"filled, rounded\", fillcolor=\"#26e5a2\", label=\"free = %d\" ];\n", llist->free);

    i = llist->free;
    while(i != 0) {
        fprintf(dump_file, "%d ", i);
        fprintf(dump_file, "[style = \"filled, rounded\", fillcolor=\"#26e5a2\"];\n");

        i = llist->next[i];
    }

    fprintf(dump_file, "{ rank = same; ");
    for(int i = 0; i < llist->capacity; i++) {
        fprintf(dump_file, "%d; ", i);
    }
    fprintf(dump_file, "}\n");

    for(i = 0; i < llist->capacity - 1; i++) {
        fprintf(dump_file, "%d->", i);
    }

    fprintf(dump_file, "%d ", llist->capacity - 1);
    fprintf(dump_file, "[style=bold, weight=10, style=invis];\n");


    fprintf(dump_file, "free->%d;\n", llist->free);
    for(int i = 0; i < llist->capacity; i++) {
        if(i && llist->next[i]) {
            fprintf(dump_file, "%d->%d;\n", i, llist->next[i]);
        }
    }

    fprintf(dump_file, "0->%d;\n", llist->next[0]);
    fprintf(dump_file, "%d->0;\n", llist->prev[0]);

    fprintf(dump_file, "edge [style=bold, color=\"#891728:black;0.001\", weight=0, penwidth=2.5, arrowsize=0.4];\n");

    for(int i = 0; i < llist->capacity; i++) {
        if(i && llist->prev[i]) {
            fprintf(dump_file, "%d->%d;\n", i, llist->prev[i]);
        }
    }

    fprintf(dump_file, "0->%d;\n", llist->prev[0]);
    fprintf(dump_file, "%d->0;\n", llist->next[0]);

    fprintf(dump_file, "}\n");

    fclose(dump_file);

    system("dot -Tpng dump.dot -o dump.png");

    return SUCCESS;
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
        llist->prev[i] = i - 1;
        llist->data[i] = -1;
    }

    llist->prev[0] = 0;
    llist->next[0] = 0;

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
