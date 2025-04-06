#include "utils/qing_json.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define QING_JSON_MAX_KEY 1024  // 添加前缀

static qing_json_value_t* json_parse_value(const char** str);

static const char* json_skip_whitespace(const char* str) {
    while (*str && isspace((unsigned char)*str)) str++;
    return str;
}

static qing_json_value_t* json_parse_null(const char** str) {
    if (strncmp(*str, "null", 4) == 0) {
        *str += 4;
        qing_json_value_t* val = malloc(sizeof(qing_json_value_t));
        val->type = QING_JSON_NULL;
        return val;
    }
    return NULL;
}

// 解析 true / false
static qing_json_value_t* json_parse_bool(const char** str) {
    qing_json_value_t* val = malloc(sizeof(qing_json_value_t));
    if (strncmp(*str, "true", 4) == 0) {
        val->type = QING_JSON_BOOL;
        val->boolean = 1;
        *str += 4;
    } else if (strncmp(*str, "false", 5) == 0) {
        val->type = QING_JSON_BOOL;
        val->boolean = 0;
        *str += 5;
    } else {
        free(val);
        return NULL;
    }
    return val;
}

// 解析 number
static qing_json_value_t* json_parse_number(const char** str) {
    char* end;
    double num = strtod(*str, &end);
    if (*str == end) return NULL;

    qing_json_value_t* val = malloc(sizeof(qing_json_value_t));
    val->type = QING_JSON_NUMBER;
    val->number = num;
    *str = end;
    return val;
}

// 解析 string
static qing_json_value_t* json_parse_string(const char** str) {
    if (**str != '"') return NULL;
    (*str)++;

    const char* start = *str;
    while (**str && **str != '"') (*str)++;
    if (**str != '"') return NULL;

    size_t len = *str - start;
    char* s = malloc(len + 1);
    strncpy(s, start, len);
    s[len] = '\0';

    (*str)++;
    qing_json_value_t* val = malloc(sizeof(qing_json_value_t));
    val->type = QING_JSON_STRING;
    val->string = s;
    return val;
}

// 解析数组
static qing_json_value_t* json_parse_array(const char** str) {
    if (**str != '[') return NULL;
    (*str)++;

    qing_json_value_t* val = malloc(sizeof(qing_json_value_t));
    val->type = QING_JSON_ARRAY;
    val->count = 0;
    val->array = NULL;

    *str = json_skip_whitespace(*str);
    if (**str == ']') {
        (*str)++;
        return val;
    }

    while (**str) {
        val->array = realloc(val->array, (val->count + 1) * sizeof(qing_json_value_t*));
        val->array[val->count] = json_parse_value(str);
        if (!val->array[val->count]) {
            qing_json_free(val);  // 新增：解析失败时释放整个数组
            return NULL;
        }
        val->count++;

        while (**str) {
            *str = json_skip_whitespace(*str);
            if (**str == ']') {
                (*str)++;
                return val;
            } else if (**str == ',') {
                (*str)++;
                break;
            } else 
                break;
        }
    }
    qing_json_free(val); 
    return NULL;
}

// 解析对象
static qing_json_value_t* json_parse_object(const char** str) {
    if (**str != '{') return NULL;
    (*str)++;

    qing_json_value_t* val = malloc(sizeof(qing_json_value_t));
    val->type = QING_JSON_OBJECT;
    val->count = 0;
    val->object.keys = NULL;
    val->object.values = NULL;

    *str = json_skip_whitespace(*str);
    if (**str == '}') {
        (*str)++;
        return val;
    }

    while (**str) {
        qing_json_value_t* key = json_parse_string(str);
        if (!key) {
            qing_json_free(val);  // 新增：key解析失败释放对象
            return NULL;
        }

        *str = json_skip_whitespace(*str);
        if (**str != ':') {
            free(key->string);
            free(key);
            qing_json_free(val);
            return NULL;
        }
        (*str)++;

        *str = json_skip_whitespace(*str);
        qing_json_value_t* value = json_parse_value(str);
        if (!value) {
            free(key->string);
            free(key);         
            qing_json_free(val); 
            return NULL;
        }

        val->object.keys = realloc(val->object.keys, (val->count + 1) * sizeof(char*));
        val->object.values = realloc(val->object.values, (val->count + 1) * sizeof(qing_json_value_t*));
        val->object.keys[val->count] = key->string;
        val->object.values[val->count] = value;
        val->count++;
        
        while (**str) {
            *str = json_skip_whitespace(*str);
            if (**str == '}') {
                (*str)++;
                return val;
            } else if (**str == ',') {
                (*str)++;
            } else 
                break;
        }
    }
    qing_json_free(val);
    return NULL;
}

// 解析 JSON 入口
static inline qing_json_value_t* json_parse_value(const char** str) {
    *str = json_skip_whitespace(*str);
    if (**str == 'n') return json_parse_null(str);
    if (**str == 't' || **str == 'f') return json_parse_bool(str);
    if (**str == '-' || isdigit((unsigned char)**str)) return json_parse_number(str);
    if (**str == '"') return json_parse_string(str);
    if (**str == '[') return json_parse_array(str);
    if (**str == '{') return json_parse_object(str);
    return NULL;
}

qing_json_value_t* qing_json_parse(const char** str) {
    return json_parse_value(str);
}

// 释放 JSON 结构体
void qing_json_free(qing_json_value_t* json) {
    if (!json) return;

    if (json->type == QING_JSON_STRING) free(json->string);
    else if (json->type == QING_JSON_ARRAY) {
        for (size_t i = 0; i < json->count; i++) qing_json_free(json->array[i]);
        free(json->array);
    } else if (json->type == QING_JSON_OBJECT) {
        for (size_t i = 0; i < json->count; i++) {
            free(json->object.keys[i]);
            qing_json_free(json->object.values[i]);
        }
        free(json->object.keys);
        free(json->object.values);
    }
    free(json);
}


void qing_json_print(const qing_json_value_t* json) {
    if (!json) return;

    switch (json->type) {
        case QING_JSON_NULL:
            printf("null");
            break;
        case QING_JSON_BOOL:
            printf(json->boolean ? "true" : "false");
            break;
        case QING_JSON_NUMBER:
            printf("%f", json->number);
            break;
        case QING_JSON_STRING:
            printf("\"%s\"", json->string);
            break;
        case QING_JSON_ARRAY:
            printf("[");
            for (size_t i = 0; i < json->count; i++) {
                if (i > 0) printf(", ");
                qing_json_print(json->array[i]);
            }
            printf("]");
            break;
        case QING_JSON_OBJECT:
            printf("{");
            for (size_t i = 0; i < json->count; i++) {
                if (i > 0) printf(", ");
                printf("\"%s\": ", json->object.keys[i]);
                qing_json_print(json->object.values[i]);
            }
            printf("}");
            break;
    }
}