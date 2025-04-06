#include "qing_transformers.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>


// 测试加载safetensors文件
void test_safetensors_load() {
    const char* test_file = "/Users/king/gitdir/Qwen2.5-0.5B-Instruct/model.safetensors";
    
    printf("file: %s\n", test_file);
    int result = qing_safetensors_load(test_file);
    
    assert(result == 0);
    printf("test_safetensors_load!\n");
}

int main() {

    test_safetensors_load();
    
    printf("All tests passed!\n");
    return 0;
}