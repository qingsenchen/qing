#include "utils/qing_string.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define QING_STR_HDR(s) ((qing_string_hdr_t*)((s) - sizeof(qing_string_hdr_t)))


/*
** 哈希函数，FNV-1a算法
*/
static uint32_t hash_string(const char* key, int len) {
    uint32_t hash = 2166136261u;
    for (int i = 0; i < len; i++) {
        hash ^= (uint8_t)key[i];
        hash *= 16777619;
    }
    return hash;
}

qing_string_t qing_string_new(const char* init) {
    return qing_string_new_len(init, init ? strlen(init) : 0);
}

qing_string_t qing_string_new_len(const char* init, size_t len) {
    size_t cap = len < 16 ? 16 : len * 2;
    qing_string_hdr_t* sh = malloc(sizeof(qing_string_hdr_t) + cap + 1);
    if (!sh) return NULL;

    sh->len = len;
    sh->cap = cap;
    sh->hash = init ? hash_string(init, len) : 0; 
    
    if (init && len) {
        memcpy(sh->buf, init, len);
    }
    sh->buf[len] = '\0';

    return sh->buf;
}

void qing_string_free(qing_string_t s) {
    if (s) free(QING_STR_HDR(s));
}

size_t qing_string_len(const qing_string_t s) {
    
    return QING_STR_HDR(s)->len;
}

size_t qing_string_cap(const qing_string_t s) {
    return QING_STR_HDR(s)->cap;
}

qing_string_t qing_string_concat(qing_string_t s, const char* t) {
    return qing_string_concat_len(s, t, strlen(t));
}

qing_string_t qing_string_concat_len(qing_string_t s, const char* t, size_t len) {
    qing_string_hdr_t* sh = QING_STR_HDR(s);
    size_t new_len = sh->len + len;

    if (new_len > sh->cap) {
        size_t new_cap = new_len * 2;
        sh = realloc(sh, sizeof(qing_string_hdr_t) + new_cap + 1);
        if (!sh) return NULL;
        sh->cap = new_cap;
        s = sh->buf;
    }

    memcpy(sh->buf + sh->len, t, len);
    sh->len = new_len;
    sh->buf[new_len] = '\0';
    sh->hash = 0; // Reset hash when string is modified
    return sh->buf;
}

void qing_string_clear(qing_string_t s) {
    QING_STR_HDR(s)->len = 0;
    QING_STR_HDR(s)->hash = 0; // Reset hash when string is cleared
    s[0] = '\0';
}