#include "utils/qing_hash.h"
#include "utils/qing_string.h"
#include "qing_memory.h"

#include <stdlib.h>
#include <string.h>

#define HASH_MAX_LOAD 0.75
#define TABLE_INIT_CAPACITY 8

// FNV-1a hash function
static uint32_t hash_string(const char* key, int length) {
    uint32_t hash = 2166136261u;
    for (int i = 0; i < length; i++) {
        hash ^= (uint8_t)key[i];
        hash *= 16777619;
    }
    return hash;
}

void qing_hash_alloc(qing_hash_table_t* table) {
    table->count = 0;
    table->capacity = 0;
    table->entries = NULL;
}

void qing_hash_free(qing_hash_table_t* table) {
    QING_FREE_ARRAY(qing_hash_entry_t, table->entries, table->capacity);
    qing_hash_alloc(table);
}

static qing_hash_entry_t* hash_find_entry(qing_hash_entry_t* entries, int capacity,
    qing_string_t key) {
    uint32_t index = qing_string_hash(key) % capacity;
    qing_hash_entry_t* tombstone = NULL;

    for (;;) {
        qing_hash_entry_t* entry = &entries[index];
        if (entry->key == NULL) {
            if (QING_IS_NULL(entry->value)) {
                // Empty entry.
                return tombstone != NULL ? tombstone : entry;
            } else {
                // We found a tombstone.
                if (tombstone == NULL) tombstone = entry;
            }
        } else if (strcmp(entry->key, key) == 0) { //TODO 优化比较
            // We found the key.
            return entry;
        }

        index = (index + 1) % capacity;
    }
}

bool qing_hash_get(qing_hash_table_t* table, qing_string_t key, qing_hash_value_t* value) {
    if (table->count == 0) return false;
    
    qing_hash_entry_t* entry = hash_find_entry(table->entries, table->capacity, key);
    if (entry->key == NULL) return false;

    *value = entry->value;
    return true;
}

static void hash_adjust_capacity(qing_hash_table_t* table, int capacity) {
    qing_hash_entry_t* entries = QING_ALLOCATE(qing_hash_entry_t, capacity);
    for (int i = 0; i < capacity; i++) {
        entries[i].key = NULL;
        entries[i].value = NULL_VAL;
    }

    table->count = 0;
    for (int i = 0; i < table->capacity; i++) {
        qing_hash_entry_t* entry = &table->entries[i];
        if (entry->key == NULL) continue;

        qing_hash_entry_t* dest = hash_find_entry(entries, capacity, entry->key);
        dest->key = entry->key;
        dest->value = entry->value;
        table->count++;
    }

    QING_FREE_ARRAY(qing_hash_entry_t, table->entries, table->capacity);
    table->entries = entries;
    table->capacity = capacity;
}

bool qing_hash_set(qing_hash_table_t* table, qing_string_t key, qing_hash_value_t value) {
    if (table->count + 1 > table->capacity * HASH_MAX_LOAD) {
        int capacity = QING_GROW_CAPACITY(table->capacity);
        hash_adjust_capacity(table, capacity);
    }
    qing_hash_entry_t* entry = hash_find_entry(table->entries, table->capacity, key);
    bool is_new_key = entry->key == NULL;
    if (is_new_key && QING_IS_NULL(entry->value)) table->count++;

    entry->key = key;
    entry->value = value;
    return is_new_key;
}

bool qing_hash_delete(qing_hash_table_t* table, qing_string_t key) {
    if (table->count == 0) return false;

    // Find the entry.
    qing_hash_entry_t* entry = hash_find_entry(table->entries, table->capacity, key);
    if (entry->key == NULL) return false;

    // Place a tombstone in the entry.
    entry->key = NULL;
    entry->value = BOOL_VAL(true);
    return true;
}

void qing_hash_copy(qing_hash_table_t* from, qing_hash_table_t* to) {
    qing_hash_alloc(to);
    for (int i = 0; i < from->capacity; i++) {
        qing_hash_entry_t* entry = &from->entries[i];
        if (entry->key != NULL) {
            qing_hash_set(to, entry->key, entry->value);
        }
    }
}

qing_string_t qing_hash_find_string(qing_hash_table_t* table, const char* chars, int length, uint32_t hash) {
    if (table->count == 0) return NULL;

    uint32_t index = hash % table->capacity;
    for (;;) {
        qing_hash_entry_t* entry = &table->entries[index];
        if (entry->key == NULL) {
            // Stop if we find an empty non-tombstone entry.
            if (QING_IS_NULL(entry->value)) return NULL;
        } else if (qing_string_len(entry->key) == length && qing_string_hash(entry->key) == hash &&
                    memcmp(entry->key, chars, length) == 0) {
            // We found it.
            return entry->key;
        }

        index = (index + 1) % table->capacity;
    }
}