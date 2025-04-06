#ifndef QING_JSON_H
#define QING_JSON_H

#include <stdlib.h>

typedef enum {
    QING_JSON_NULL, 
    QING_JSON_BOOL, 
    QING_JSON_FLOAT,
    QING_JSON_INT, 
    QING_JSON_STRING, 
    QING_JSON_ARRAY, 
    QING_JSON_OBJECT
} qing_json_type_t;

typedef struct qing_json_value {
    qing_json_type_t type;
    union {
        double number;
        int integer;
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

#define QING_JSON_IS_NULL(json)     ((json)->type == QING_JSON_NULL)
#define QING_JSON_IS_BOOL(json)     ((json)->type == QING_JSON_BOOL)
#define QING_JSON_IS_INT(json)      ((json)->type == QING_JSON_INT)
#define QING_JSON_IS_FLOAT(json)    ((json)->type == QING_JSON_FLOAT)
#define QING_JSON_IS_STRING(json)   ((json)->type == QING_JSON_STRING)
#define QING_JSON_IS_ARRAY(json)    ((json)->type == QING_JSON_ARRAY)
#define QING_JSON_IS_OBJECT(json)   ((json)->type == QING_JSON_OBJECT)

#define QING_JSON_AS_ARRAY(json)    ((json)->array)
#define QING_JSON_AS_BOOL(json)     ((json)->boolean)
#define QING_JSON_AS_INT(json)      ((json)->integer)
#define QING_JSON_AS_FLOAT(json)    ((json)->number)
#define QING_JSON_AS_STRING(json)   ((json)->string)

qing_json_value_t* qing_json_parse(const char** str);
void qing_json_free(qing_json_value_t* json);

qing_json_value_t* qing_json_get_value(const qing_json_value_t* json, const char* key);

void qing_json_print(const qing_json_value_t* json);
void qing_json_print_pretty(const qing_json_value_t* json);

#endif // QING_JSON_H