#ifndef QING_ARRAY_H
#define QING_ARRAY_H
#include "qing.h"
typedef struct {
    size_t count; 
    size_t capacity;     
    size_t elem_size;
    void* entries;
} qing_array_t;

qing_array_t* qing_array_new(qing_vtype_t vtype);
void qing_array_free(qing_array_t* array);
void qing_array_push(qing_array_t* array, void* value);
void qing_array_pop(qing_array_t* array);

#endif