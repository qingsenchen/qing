#ifndef QING_HASH_H
#define QING_HASH_H


#include <stdbool.h>
#include "qing_string.h"

typedef enum {
    QING_TYPE_NULL, 
    QING_TYPE_BOOL, 
    QING_TYPE_FLOAT,
    QING_TYPE_INT, 
    QING_TYPE_STRING
} qing_vtype_t;

#define QING_IS_BOOL(value)         ((value).type == QING_TYPE_BOOL)
#define QING_IS_NULL(value)         ((value).type == QING_TYPE_NULL)
#define QING_IS_FLOAT(value)        ((value).type == QING_TYPE_FLOAT)
#define QING_IS_INTEGER(value)      ((value).type == QING_TYPE_INT)

#define QING_AS_BOOL(value)     ((value).as.boolean)
#define QING_AS_FLOAT(value)   ((value).as.number)
#define QING_AS_INT(value)      ((value).as.integer)

#define BOOL_VAL(value)     ((qing_hash_value_t){QING_TYPE_BOOL,    {.boolean = value}})
#define NULL_VAL            ((qing_hash_value_t){QING_TYPE_NULL,    {.integer = 0}})
#define FLOAT_VAL(value)    ((qing_hash_value_t){QING_TYPE_FLOAT,   {.number  = value}})
#define INT_VAL(value)      ((qing_hash_value_t){QING_TYPE_INT,     {.integer = value}})

typedef struct qing_hash_value {
    qing_vtype_t type;
    union {
        double number;
        int integer;
        int boolean;
        char* string;
    } as;
} qing_hash_value_t;

typedef struct {
    qing_string_t key;
    qing_hash_value_t value;
} qing_hash_entry_t;

typedef struct {
    int count;
    int capacity;
    qing_hash_entry_t* entries;
} qing_hash_table_t;

void qing_hash_alloc(qing_hash_table_t* table);
void qing_hash_free(qing_hash_table_t* table);
bool qing_hash_get(qing_hash_table_t* table, qing_string_t key, qing_hash_value_t* value);
bool qing_hash_set(qing_hash_table_t* table, qing_string_t key, qing_hash_value_t value);
bool qing_hash_delete(qing_hash_table_t* table, qing_string_t key);
void qing_hash_copy(qing_hash_table_t* from, qing_hash_table_t* to);
qing_string_t qing_hash_find_string(qing_hash_table_t* table, const char* chars,
                                 int length, uint32_t hash);
void qing_hash_remove_white(qing_hash_table_t* table);
void qing_hash_mark(qing_hash_table_t* table);

#endif // QING_HASH_H