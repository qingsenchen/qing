#ifndef QING_JSON_H
#define QING_JSON_H

#include <stdlib.h>

typedef enum {
    QING_JSON_NULL, 
    QING_JSON_BOOL, 
    QING_JSON_NUMBER, 
    QING_JSON_STRING, 
    QING_JSON_ARRAY, 
    QING_JSON_OBJECT
} qing_json_type_t;

typedef struct qing_json_value {
    qing_json_type_t type;
    union {
        double number;
        int boolean;
        char* string;
        struct qing_json_value** array;
        struct {
            char** keys;
            struct qing_json_value** values;
        } object;
    };
    size_t count;
} qing_json_value_t;

qing_json_value_t* qing_json_parse(const char** str);
void qing_json_free(qing_json_value_t* json);

void qing_json_print(const qing_json_value_t* json);

#endif // QING_JSON_H