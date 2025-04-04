#ifndef QING_JSON_H
#define QING_JSON_H

#include <stdlib.h>

// 使用项目统一前缀的枚举类型
typedef enum {
    QING_JSON_NULL, 
    QING_JSON_BOOL, 
    QING_JSON_NUMBER, 
    QING_JSON_STRING, 
    QING_JSON_ARRAY,  // 行号 12
    QING_JSON_OBJECT
} qing_json_type_t;

// 使用项目统一前缀的结构体
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

// 使用统一前缀的函数声明
qing_json_value_t* qing_json_parse(const char** str);
void qing_json_free(qing_json_value_t* json);

void qing_json_print(const qing_json_value_t* json);

#endif // QING_JSON_H