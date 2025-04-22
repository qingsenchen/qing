#include "utils/qing_hash.h"
#include "utils/qing_string.h"
#include "qing_memory.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

// Test helper macro for assertions
#define TEST_ASSERT(condition, message) \
    do { \
        if (!(condition)) { \
            printf("Test failed: %s\n", message); \
            exit(1); \
        } \
    } while (0)

// Test hash table initialization and cleanup
void test_hash_init_free() {
    printf("Testing hash table initialization and cleanup...\n");
    
    qing_hash_table_t table;
    qing_hash_alloc(&table);
    
    TEST_ASSERT(table.count == 0, "Initial count should be 0");
    TEST_ASSERT(table.capacity == 0, "Initial capacity should be 0");
    TEST_ASSERT(table.entries == NULL, "Initial entries should be NULL");
    
    qing_hash_free(&table);
    
    printf("Hash table initialization and cleanup tests passed!\n");
}

// Test setting and getting values
void test_hash_set_get() {
    printf("Testing hash table set and get operations...\n");
    
    qing_hash_table_t table;
    qing_hash_alloc(&table);
    
    // Create some test keys
    qing_string_t key1 = qing_string_new("key1");
    qing_string_t key2 = qing_string_new("key2");
    qing_string_t key3 = qing_string_new("key3");
    
    // Test setting values
    qing_hash_value_t value1 = INT_VAL(100);
    qing_hash_value_t value2 = FLOAT_VAL(3.14);
    qing_hash_value_t value3 = BOOL_VAL(true);
    
    bool is_new1 = qing_hash_set(&table, key1, value1);
    bool is_new2 = qing_hash_set(&table, key2, value2);
    bool is_new3 = qing_hash_set(&table, key3, value3);
    
    TEST_ASSERT(is_new1, "First insertion should be new");
    TEST_ASSERT(is_new2, "Second insertion should be new");
    TEST_ASSERT(is_new3, "Third insertion should be new");
    TEST_ASSERT(table.count == 3, "Table should have 3 entries");
    
    // Test getting values
    qing_hash_value_t result1, result2, result3, result4;
    bool found1 = qing_hash_get(&table, key1, &result1);
    bool found2 = qing_hash_get(&table, key2, &result2);
    bool found3 = qing_hash_get(&table, key3, &result3);
    
    TEST_ASSERT(found1, "Key1 should be found");
    TEST_ASSERT(found2, "Key2 should be found");
    TEST_ASSERT(found3, "Key3 should be found");
    
    TEST_ASSERT(QING_AS_INT(result1) == 100, "Value1 should be 100");
    TEST_ASSERT(QING_AS_FLOAT(result2) == 3.14, "Value2 should be 3.14");
    TEST_ASSERT(QING_AS_BOOL(result3) == true, "Value3 should be true");
    
    // Test getting non-existent key
    qing_string_t non_existent = qing_string_new("non_existent");
    bool found4 = qing_hash_get(&table, non_existent, &result4);
    TEST_ASSERT(!found4, "Non-existent key should not be found");
    
    // Test updating existing key
    qing_hash_value_t new_value = INT_VAL(200);
    bool is_new4 = qing_hash_set(&table, key1, new_value);
    TEST_ASSERT(!is_new4, "Updating existing key should not be new");
    
    bool found5 = qing_hash_get(&table, key1, &result1);
    TEST_ASSERT(found5, "Updated key should be found");
    TEST_ASSERT(QING_AS_INT(result1) == 200, "Updated value should be 200");
    
    // Cleanup
    qing_string_free(key1);
    qing_string_free(key2);
    qing_string_free(key3);
    qing_string_free(non_existent);
    qing_hash_free(&table);
    
    printf("Hash table set and get tests passed!\n");
}

// Test deleting entries
void test_hash_delete() {
    printf("Testing hash table delete operation...\n");
    
    qing_hash_table_t table;
    qing_hash_alloc(&table);
    
    // Create some test keys
    qing_string_t key1 = qing_string_new("key1");
    qing_string_t key2 = qing_string_new("key2");
    
    // Set values
    qing_hash_set(&table, key1, INT_VAL(100));
    qing_hash_set(&table, key2, INT_VAL(200));
    
    TEST_ASSERT(table.count == 2, "Table should have 2 entries");
    
    // Delete key1
    bool deleted = qing_hash_delete(&table, key1);
    TEST_ASSERT(deleted, "Key1 should be deleted");
    TEST_ASSERT(table.count == 2, "Count doesn't decrease due to tombstone");
    
    // Try to get deleted key
    qing_hash_value_t result;
    bool found = qing_hash_get(&table, key1, &result);
    TEST_ASSERT(!found, "Deleted key should not be found");
    
    // Delete non-existent key
    qing_string_t non_existent = qing_string_new("non_existent");
    deleted = qing_hash_delete(&table, non_existent);
    TEST_ASSERT(!deleted, "Non-existent key should not be deleted");
    
    // Cleanup
    qing_string_free(key1);
    qing_string_free(key2);
    qing_string_free(non_existent);
    qing_hash_free(&table);
    
    printf("Hash table delete tests passed!\n");
}

// Test table resizing
void test_hash_resize() {
    printf("Testing hash table resizing...\n");
    
    qing_hash_table_t table;
    qing_hash_alloc(&table);
    
    // Add enough entries to trigger resize
    char key_buffer[32];
    int initial_capacity = 0;
    
    for (int i = 0; i < 20; i++) {
        sprintf(key_buffer, "key%d", i);
        
        qing_string_t key = qing_string_new(key_buffer);
        printf("Key: %s %d\n", key, qing_string_hash(key));
        qing_hash_set(&table, key, INT_VAL(i));
        
        // Remember the first non-zero capacity
        if (initial_capacity == 0 && table.capacity > 0) {
            initial_capacity = table.capacity;
        }
    }
    
    TEST_ASSERT(table.count == 20, "Table should have 20 entries");
    TEST_ASSERT(table.capacity > initial_capacity, "Table should have resized");
    
    // Verify all entries are still accessible
    qing_hash_value_t result;
    for (int i = 0; i < 20; i++) {
        sprintf(key_buffer, "key%d", i);
        printf("table.count: %d\n", table.count);
        qing_string_t key = qing_string_new(key_buffer);
        printf("Key: %s %d\n", key, qing_string_hash(key));

        bool found = qing_hash_get(&table, key, &result);
        
        TEST_ASSERT(found, "Key should be found after resize");
        TEST_ASSERT(QING_AS_INT(result) == i, "Value should be correct after resize");
        
        qing_string_free(key);
    }
    
    // Cleanup - free all keys in the table
    for (int i = 0; i < table.capacity; i++) {
        if (table.entries[i].key != NULL) {
            qing_string_free(table.entries[i].key);
        }
    }
    qing_hash_free(&table);
    
    printf("Hash table resizing tests passed!\n");
}

// Test hash table copy
void test_hash_copy() {
    printf("Testing hash table copy...\n");
    
    qing_hash_table_t source;
    qing_hash_table_t destination;
    
    qing_hash_alloc(&source);
    
    // Add some entries to source
    qing_string_t key1 = qing_string_new("key1");
    qing_string_t key2 = qing_string_new("key2");
    qing_string_t key3 = qing_string_new("key3");
    
    qing_hash_set(&source, key1, INT_VAL(100));
    qing_hash_set(&source, key2, FLOAT_VAL(3.14));
    qing_hash_set(&source, key3, BOOL_VAL(true));
    
    // Copy source to destination
    qing_hash_copy(&source, &destination);
    
    TEST_ASSERT(destination.count == 3, "Destination should have 3 entries");
    
    // Verify all entries were copied correctly
    qing_hash_value_t result;
    bool found1 = qing_hash_get(&destination, key1, &result);
    TEST_ASSERT(found1, "Key1 should be found in destination");
    TEST_ASSERT(QING_AS_INT(result) == 100, "Value1 should be correct in destination");
    
    bool found2 = qing_hash_get(&destination, key2, &result);
    TEST_ASSERT(found2, "Key2 should be found in destination");
    TEST_ASSERT(QING_AS_FLOAT(result) == 3.14, "Value2 should be correct in destination");
    
    bool found3 = qing_hash_get(&destination, key3, &result);
    TEST_ASSERT(found3, "Key3 should be found in destination");
    TEST_ASSERT(QING_AS_BOOL(result) == true, "Value3 should be correct in destination");
    
    // Cleanup
    qing_string_free(key1);
    qing_string_free(key2);
    qing_string_free(key3);
    qing_hash_free(&source);
    qing_hash_free(&destination);
    
    printf("Hash table copy tests passed!\n");
}

// Test string finding
// void test_hash_find_string() {
//     printf("Testing hash find string...\n");
    
//     qing_hash_table_t table;
//     qing_hash_alloc(&table);
    
//     // Add some entries
//     qing_string_t key1 = qing_string_new("hello");
//     qing_string_t key2 = qing_string_new("world");
    
//     qing_hash_set(&table, key1, INT_VAL(1));
//     qing_hash_set(&table, key2, INT_VAL(2));
    
//     // Find existing strings
//     uint32_t hash1 = hash_string("hello", 5);
//     qing_string_t found1 = qing_hash_find_string(&table, "hello", 5, hash1);
//     TEST_ASSERT(found1 == key1, "Should find existing string 'hello'");
    
//     uint32_t hash2 = hash_string("world", 5);
//     qing_string_t found2 = qing_hash_find_string(&table, "world", 5, hash2);
//     TEST_ASSERT(found2 == key2, "Should find existing string 'world'");
    
//     // Try to find non-existent string
//     uint32_t hash3 = hash_string("nonexistent", 11);
//     qing_string_t found3 = qing_hash_find_string(&table, "nonexistent", 11, hash3);
//     TEST_ASSERT(found3 == NULL, "Should not find non-existent string");
    
//     // Cleanup
//     qing_string_free(key1);
//     qing_string_free(key2);
//     qing_hash_free(&table);
    
//     printf("Hash find string tests passed!\n");
// }

int main() {
    printf("Starting qing_hash tests...\n");
    
    test_hash_init_free();
    test_hash_set_get();
    test_hash_delete();
    test_hash_resize();
    test_hash_copy();
    //test_hash_find_string();
    
    printf("All hash table tests passed!\n");
    return 0;
}