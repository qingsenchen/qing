#ifndef QING_TOKENIZER_H
#define QING_TOKENIZER_H

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "utils/qing_hash.h"

typedef struct {
    size_t max_token_len;
    qing_hash_table_t vocab;
    qing_hash_table_t merges;
} qing_tokenizer_t;

qing_tokenizer_t* qing_tokenizer_new();
int qing_tokenizer_vocab_lookup(qing_tokenizer_t* tokenizer, const char* key);
qing_tokenizer_t* qing_tokenizer_load(const char* filename);

int qing_tokenizer_vocab_size(qing_tokenizer_t* tokenizer);

#endif // QING_TOKENIZER_H
