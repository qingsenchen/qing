#ifndef QING_H
#define QING_H

#include <stdbool.h>

typedef enum {
    QING_TYPE_NULL, 
    QING_TYPE_BOOL, 
    QING_TYPE_FLOAT,
    QING_TYPE_INT, 
    QING_TYPE_STRING
} qing_type_t;

#define QING_IS_BOOL(value)         ((value).type == QING_TYPE_BOOL)
#define QING_IS_NULL(value)         ((value).type == QING_TYPE_NULL)
#define QING_IS_FLOAT(value)        ((value).type == QING_TYPE_FLOAT)
#define QING_IS_INTEGER(value)      ((value).type == QING_TYPE_INT)

#define QING_AS_BOOL(value)     ((value).as.boolean)
#define QING_AS_FLOAT(value)   ((value).as.number)
#define QING_AS_INT(value)      ((value).as.integer)

#define BOOL_VAL(value)     ((qing_value_t){QING_TYPE_BOOL,    {.boolean = value}})
#define NULL_VAL            ((qing_value_t){QING_TYPE_NULL,    {.integer = 0}})
#define FLOAT_VAL(value)    ((qing_value_t){QING_TYPE_FLOAT,   {.number  = value}})
#define INT_VAL(value)      ((qing_value_t){QING_TYPE_INT,     {.integer = value}})

typedef struct qing_object {
    qing_type_t type;
    union {
        double number;
        int integer;
        int boolean;
        char* string;
    } as;
} qing_value_t;

#endif //QING_H

