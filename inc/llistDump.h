#ifndef LLISTDUMP_H_
#define LLISTDUMP_H_

TYPE_OF_ERROR        llistDump(LinkedList* llist);
TYPE_OF_ERROR        setDumpFile(LinkedList* llist);
inline TYPE_OF_ERROR processFilename(char* filename);

#endif
