#ifndef ARRAY_LIST
#define ARRAY_LIST

// arraylist que serve de historico
typedef struct arraylist {
    unsigned int n_occupied;
    unsigned int capacity;
    unsigned long int *values;
} *ArrayList;

ArrayList arraylist_create();

void arraylist_add(ArrayList a, unsigned long int n);

#endif
