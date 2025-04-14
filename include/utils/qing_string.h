#ifndef QING_STRING_H
#define QING_STRING_H

#include <stddef.h>
#include <stdint.h>

typedef struct {
    size_t len;     // 当前长度（不包含终止符）
    size_t cap;     // 总容量
    uint32_t hash;
    uint32_t pading;
    char buf[];     // 实际字符内容（变长数组）
} qing_string_hdr_t;

typedef char* qing_string_t;

qing_string_t qing_string_new(const char* init);
qing_string_t qing_string_new_len(const char* init, size_t len);
void qing_string_free(qing_string_t s);

size_t qing_string_len(const qing_string_t s);
size_t qing_string_cap(const qing_string_t s);
qing_string_t qing_string_concat(qing_string_t s, const char* t);
qing_string_t qing_string_concat_len(qing_string_t s, const char* t, size_t len);
void qing_string_clear(qing_string_t s);

#endif // QING_STRING_H