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

static inline qing_json_value_t* qing_json_parse_float(const char** str) {
    char* end;
    double val = strtod(*str, &end);
    if (*str == end) return NULL;

    qing_json_value_t* result = malloc(sizeof(qing_json_value_t));
    result->type = QING_JSON_FLOAT;
    result->number = val;
    *str = end;
    return result;
}

static inline qing_json_value_t* qing_json_parse_int(const char** str) {
    char* end;
    long val = strtol(*str, &end, 10);
    if (*str == end) return NULL;

    qing_json_value_t* result = malloc(sizeof(qing_json_value_t));
    result->type = QING_JSON_INT;
    result->integer = (int)val;
    *str = end;
    return result;
}

static qing_json_value_t* json_parse_number(const char** str) {
    const char* s = *str;

    // 查找是否是 float 格式（含 '.' 或 e/E）
    while (*s && (isdigit(*s) || *s == '+' || *s == '-' || *s == '.' || *s == 'e' || *s == 'E')) {
        if (*s == '.' || *s == 'e' || *s == 'E') {
            return qing_json_parse_float(str);
        }
        s++;
    }
    return qing_json_parse_int(str);
}

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
            qing_json_free(val); 
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
            qing_json_free(val);
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

qing_json_value_t* qing_json_get_value(const qing_json_value_t* json, const char* key) {
    if (!json || json->type != QING_JSON_OBJECT || !key) return NULL;

    for (size_t i = 0; i < json->count; i++) {
        if (strcmp(json->object.keys[i], key) == 0) {
            return json->object.values[i];
        }
    }
    return NULL;
}

// qing_json_value_t* qing_json_get_nested(const qing_json_value_t* json, ...) {
//     if (!json) return NULL;
    
//     const qing_json_value_t* current = json;
//     va_list args;
//     va_start(args, json);
    
//     const char* key;
//     while ((key = va_arg(args, const char*)) != NULL && current) {
//         if (current->type != QING_JSON_OBJECT) {
//             va_end(args);
//             return NULL;
//         }
        
//         current = qing_json_get_value(current, key);
//     }
    
//     va_end(args);
//     return (qing_json_value_t*)current;
// }

qing_json_value_t* qing_json_parse(const char** str) {
    return json_parse_value(str);
}

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

static inline void json_print_indent(int indent_level) {
    for (int i = 0; i < indent_level; i++) {
        printf("  "); // 每层缩进2个空格
    }
}

static void json_print_pretty(const qing_json_value_t* json, int indent_level) {
    if (!json) return;

    switch (json->type) {
        case QING_JSON_NULL:
            printf("null");
            break;
        case QING_JSON_BOOL:
            printf(json->boolean ? "true" : "false");
            break;
        case QING_JSON_FLOAT:
            printf("%f", json->number);
            break;
        case QING_JSON_INT:
            printf("%d", json->integer);
            break;
        case QING_JSON_STRING:
            printf("\"%s\"", json->string);
            break;
        case QING_JSON_ARRAY:
            printf("[\n");
            for (size_t i = 0; i < json->count; i++) {
                json_print_indent(indent_level + 1);
                json_print_pretty(json->array[i], indent_level + 1);
                if (i < json->count - 1) printf(",");
                printf("\n");
            }
            json_print_indent(indent_level);
            printf("]");
            break;
        case QING_JSON_OBJECT:
            printf("{\n");
            for (size_t i = 0; i < json->count; i++) {
                json_print_indent(indent_level + 1);
                printf("\"%s\": ", json->object.keys[i]);
                json_print_pretty(json->object.values[i], indent_level + 1);
                if (i < json->count - 1) printf(",");
                printf("\n");
            }
            json_print_indent(indent_level);
            printf("}");
            break;
    }
}

void qing_json_print_pretty(const qing_json_value_t* json) {
    json_print_pretty(json, 0);
    printf("\n");
}   

void qing_json_print(const qing_json_value_t* json) {
    if (!json) return;

    switch (json->type) {
        case QING_JSON_NULL:
            break;
        case QING_JSON_BOOL:
            printf(json->boolean ? "true" : "false");
            break;
        case QING_JSON_FLOAT:
            printf("%f", json->number);
            break;
        case QING_JSON_INT:
            printf("%d", json->integer);
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