#include "qing_tokenizer.h"
#include "utils/qing_json.h"
// 加载词汇表
qing_vocab_t* qing_vocab_load(const char* filename) {
    
    return NULL;
}


void qing_vocab_add_token(qing_vocab_t* vocab, const char* token, int id) {
    // 创建token的副本，因为原始token可能是临时的
    qing_string_t new_token = qing_string_new(token);
    
    // 添加到token_to_id映射
    qing_hash_set(&vocab->token_to_id, new_token, INT_VAL(id));
    
}

void qing_tokenize_bpe(qing_tokenizer_t* tokenizer, const char* text, int* tokens, int max_length) {
    
}