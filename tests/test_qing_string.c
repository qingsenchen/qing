#include "utils/qing_string.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

// Test helper macro for simplifying assertions
#define TEST_ASSERT(condition, message) \
    do { \
        if (!(condition)) { \
            printf("Test failed: %s\n", message); \
            exit(1); \
        } \
    } while (0)

// Test string creation functionality
void test_string_creation() {
    printf("Testing string creation...\n");
    
    // Test empty string creation
    qing_string_t empty = qing_string_new(NULL);
    TEST_ASSERT(empty != NULL, "Failed to create empty string");
    TEST_ASSERT(qing_string_len(empty) == 0, "Empty string length is not 0");
    TEST_ASSERT(empty[0] == '\0', "Empty string not properly terminated");
    
    // Test creation from C string
    const char* test_str = "Hello, World!";
    qing_string_t str = qing_string_new(test_str);
    TEST_ASSERT(str != NULL, "Failed to create from C string");
    
    // Add debug output
    printf("Expected length: %zu, Actual length: %zu\n", 
           strlen(test_str), qing_string_len(str));

    printf("qing_string_hdr_t: %lu\n", sizeof(qing_string_hdr_t));

    
    TEST_ASSERT(qing_string_len(str) == strlen(test_str), "String length is incorrect");
    TEST_ASSERT(strcmp(str, test_str) == 0, "String content doesn't match");
    
    // Test creation with specified length
    qing_string_t partial = qing_string_new_len(test_str, 5);
    TEST_ASSERT(partial != NULL, "Failed to create string with specified length");
    TEST_ASSERT(qing_string_len(partial) == 5, "Length of string with specified length is incorrect");
    TEST_ASSERT(strncmp(partial, test_str, 5) == 0, "Content of string with specified length doesn't match");
    TEST_ASSERT(partial[5] == '\0', "String with specified length not properly terminated");
    
    // Cleanup
    qing_string_free(empty);
    qing_string_free(str);
    qing_string_free(partial);
    
    printf("String creation tests passed!\n");
}

// Test string concatenation functionality
void test_string_concatenation() {
    printf("Testing string concatenation...\n");
    
    // Prepare test strings
    qing_string_t str = qing_string_new("Hello");
    const char* append1 = ", ";
    const char* append2 = "World!";
    
    // Test concatenating full string
    str = qing_string_concat(str, append1);
    TEST_ASSERT(str != NULL, "String concatenation failed");
    TEST_ASSERT(strcmp(str, "Hello, ") == 0, "Concatenation result is incorrect");
    TEST_ASSERT(qing_string_len(str) == strlen("Hello, "), "Length after concatenation is incorrect");
    
    // Test concatenating with specified length
    str = qing_string_concat_len(str, append2, 5);  // Only concatenate "World", not "!"
    TEST_ASSERT(str != NULL, "Concatenation with specified length failed");
    TEST_ASSERT(strcmp(str, "Hello, World") == 0, "Result of concatenation with specified length is incorrect");
    TEST_ASSERT(qing_string_len(str) == strlen("Hello, World"), "Length after concatenation with specified length is incorrect");
    
    // Test automatic capacity expansion
    size_t original_cap = qing_string_cap(str);
    // Concatenate a very long string to trigger capacity expansion
    char long_str[1000];
    memset(long_str, 'a', 999);
    long_str[999] = '\0';
    
    str = qing_string_concat(str, long_str);
    TEST_ASSERT(str != NULL, "Concatenation with capacity expansion failed");
    TEST_ASSERT(qing_string_cap(str) > original_cap, "Capacity not properly expanded");
    TEST_ASSERT(strncmp(str, "Hello, World", 12) == 0, "Original content unchanged after expansion");
    TEST_ASSERT(str[12] == 'a', "New content incorrect after expansion");
    
    // Cleanup
    qing_string_free(str);
    
    printf("String concatenation tests passed!\n");
}

// Test string clear functionality
void test_string_clear() {
    printf("Testing string clear...\n");
    
    qing_string_t str = qing_string_new("Test string");
    size_t original_cap = qing_string_cap(str);
    
    qing_string_clear(str);
    TEST_ASSERT(qing_string_len(str) == 0, "Length not 0 after clear");
    TEST_ASSERT(str[0] == '\0', "String not properly terminated after clear");
    TEST_ASSERT(qing_string_cap(str) == original_cap, "Clear should not change capacity");
    
    // Cleanup
    qing_string_free(str);
    
    printf("String clear tests passed!\n");
}

// Test hash value calculation
void test_string_hash() {
    printf("Testing string hash...\n");
    
    // Create two strings with the same content, they should have the same hash value
    qing_string_t str1 = qing_string_new("test hash");
    qing_string_t str2 = qing_string_new("test hash");
    
    // Get hash values (requires accessing internal structure)
    uint32_t hash1 = ((qing_string_hdr_t*)(str1 - sizeof(qing_string_hdr_t)))->hash;
    uint32_t hash2 = ((qing_string_hdr_t*)(str2 - sizeof(qing_string_hdr_t)))->hash;
    
    TEST_ASSERT(hash1 == hash2, "Strings with the same content should have the same hash value");
    
    // Modify str1, hash value should be reset
    str1 = qing_string_concat(str1, " modified");
    hash1 = ((qing_string_hdr_t*)(str1 - sizeof(qing_string_hdr_t)))->hash;
    TEST_ASSERT(hash1 == 0, "Hash value should be reset to 0 after modification");
    
    // Cleanup
    qing_string_free(str1);
    qing_string_free(str2);
    
    printf("String hash tests passed!\n");
}

int main() {
    printf("Starting qing_string module tests...\n");
    
    test_string_creation();
    test_string_concatenation();
    test_string_clear();
    test_string_hash();
    
    printf("All tests passed!\n");
    return 0;
}