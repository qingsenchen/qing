#include "utils/qing_array.h"
#include "qing_memory.h"
#include <stdlib.h>
#include <string.h>

#define QING_ARRAY_INITIAL_CAPACITY 8
#define QING_ARRAY_GROWTH_FACTOR 2

void qing_array_alloc(qing_array_t* array) {
    qarray->values = NULL;
    array->capacity = 0;
    array->count = 0;
    
    return array;
}

void qing_array_free(qing_array_t* array) {
    QING_FREE_ARRAY(Value, array->values, array->capacity);
    qing_array_alloc(array);
}

void qing_array_push(qing_array_t* array, void* value) {
    if (array->capacity < array->count + 1) {
        int oldCapacity = array->capacity;
        array->capacity = QING_GROW_CAPACITY(oldCapacity);
        array->values = QING_GROW_ARRAY(Value, array->values,
                                   oldCapacity, array->capacity);
    }

    array->values[array->count] = value;
    array->count++;
}