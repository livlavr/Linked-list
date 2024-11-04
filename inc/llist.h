#ifndef LLIST_H_
#define LLIST_H_

struct LinkedList {
    int* data     = NULL;
    int* next     = NULL;
    int* prev     = NULL;
    int  size     = 0;
    int  capacity = 0;
    int  free     = 0;
};

TYPE_OF_ERROR llistCtor(LinkedList* llist, int capacity);
TYPE_OF_ERROR llistDump(LinkedList* llist);
TYPE_OF_ERROR llistDtor(LinkedList* llist);
TYPE_OF_ERROR pushBack(LinkedList* llist, int element);
TYPE_OF_ERROR popBack(LinkedList* llist);
TYPE_OF_ERROR insertAfter(LinkedList* llist, int element, int index);


#endif
