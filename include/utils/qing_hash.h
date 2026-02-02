#ifndef QING_HASH_H
#define QING_HASH_H

#include "qing.h"
#include "utils/qing_string.h"

typedef struct {
    qing_string_t key;
    qing_value_t value;
} qing_hash_entry_t;

typedef struct {
    int count;
    int capacity;
    qing_hash_entry_t* entries;
} qing_hash_table_t;

void qing_hash_alloc(qing_hash_table_t* table);
void qing_hash_free(qing_hash_table_t* table);
bool qing_hash_get(qing_hash_table_t* table, qing_string_t key, qing_value_t* value);
bool qing_hash_set(qing_hash_table_t* table, qing_string_t key, qing_value_t value);
bool qing_hash_delete(qing_hash_table_t* table, qing_string_t key);
void qing_hash_copy(qing_hash_table_t* from, qing_hash_table_t* to);
qing_string_t qing_hash_find_string(qing_hash_table_t* table, const char* chars,
                                 int length, uint32_t hash);
void qing_hash_remove_white(qing_hash_table_t* table);
void qing_hash_mark(qing_hash_table_t* table);

#endif // QING_HASH_H
