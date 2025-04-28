#ifndef QING_TOKENIZER_H
#define QING_TOKENIZER_H

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "utils/qing_hash.h"

typedef struct {
    qing_hash_table_t token_to_id;
} qing_vocab_t;

typedef struct {
    qing_vocab_t* vocab;  // 词表
} qing_tokenizer_t;

// 加载词汇表
qing_vocab_t* qing_vocab_load(const char* filename);

// 释放词汇表
void qing_vocab_free(qing_vocab_t* vocab);

#endif // QING_TOKENIZER_H
