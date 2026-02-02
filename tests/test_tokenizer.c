#include "qing_tokenizer.h"
#include <stdio.h>
#include <string.h>
#include <assert.h>

void test_tokenizer_new() {
    qing_tokenizer_t* tok = qing_tokenizer_new();
    assert(tok != NULL);
}

void test_tokenizer_load(const char* path) {
    qing_tokenizer_t* tok = qing_tokenizer_load(path);
    int token_id = qing_tokenizer_vocab_lookup(tok, "#");
    printf("#:%d\n", token_id);
    assert(tok != NULL);
    int vocab_size = qing_tokenizer_vocab_size(tok);
    printf("vocab_size:%d\n", vocab_size);
}

int main() {
  
    test_tokenizer_new(); 
    test_tokenizer_load("/Users/king/gitdir/Qwen2.5-0.5B-Instruct/tokenizer.json"); 

    printf("\nAll Tokenizer tests passed!\n");
    return 0;
}
