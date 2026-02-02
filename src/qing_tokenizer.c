#include "qing.h"
#include <stdio.h>
#include "qing_tokenizer.h"
#include "utils/qing_json.h"
#include "utils/qing_string.h"
#include "utils/qing_hash.h"

qing_tokenizer_t* qing_tokenizer_new() {
    qing_tokenizer_t* tok = (qing_tokenizer_t*)malloc(sizeof(qing_tokenizer_t));
    qing_hash_alloc(&tok->vocab); 
    return tok;
}

int qing_tokenizer_vocab_lookup(qing_tokenizer_t* tokenizer, const char* key) {
    qing_string_t token = qing_string_new(key);
    qing_value_t val;
    qing_hash_get(&tokenizer->vocab, token, &val); 
    qing_string_free(token);
    return QING_AS_INT(val);
}

int qing_tokenizer_encode(qing_tokenizer_t *tokenizer, const char* text, int *tokens, int *n_tokens) {
    // create a temporary buffer that will store merge candidates of always two consecutive tokens
    // *2 for concat, +1 for null terminator +2 for UTF8 (in case max_token_length is 1)
    char* str_buffer = malloc((tokenizer->max_token_length*2 +1 +2) * sizeof(char));
    size_t str_len = 0;

    // start at 0 tokens
    *n_tokens = 0;

    // process the raw (UTF-8) byte sequence of the input string
    for (char *c = text; *c != '\0'; c++) {

        // reset buffer if the current byte is ASCII or a leading byte
        // 0xC0 is 11000000, so (*c & 0xC0) keeps the first 2 bits and zeros the rest
        // 0x80 is 10000000
        // in UTF-8, all continuation bytes start with "10" in first two bits
        // so in English this is: "if this byte is not a continuation byte"
        if ((*c & 0xC0) != 0x80) {
            // this byte must be either a leading byte (11...) or an ASCII char (0x...)
            // => reset our location, as we're starting a new UTF-8 codepoint
            str_len = 0;
        }

        // append the current byte to the buffer
        str_buffer[str_len++] = *c; // ++ is post-increment, incremented after this line
        str_buffer[str_len] = '\0';

        // while the next character is a continuation byte, continue appending
        // but if there are too many of them, just stop to avoid overruning str_buffer size.
        if ((*(c+1) & 0xC0) == 0x80 && str_len < 4) {
            continue;
        }

        // ok c+1 is not a continuation byte, so we've read in a full codepoint
        int id = qing_tokenizer_vocab_lookup(tokenizer, str_buffer);

        if (id != -1) {
            // we found this codepoint in vocab, add it as a token
            tokens[(*n_tokens)++] = id;
        } else {
            // byte_fallback encoding: just encode each byte as a token
            // +3 is here because the first 3 vocab elements are <unk>, <s>, </s>
            // so the individual bytes only start at index 3
            for (int i=0; i < str_len; i++) {
                tokens[(*n_tokens)++] = (unsigned char)str_buffer[i] + 3;
            }
        }
        str_len = 0; // protect against a sequence of stray UTF8 continuation bytes
    }

}

int qing_tokenizer_vocab_size(qing_tokenizer_t* tokenizer) {
    return tokenizer->vocab.count;
}

void qing_tokenizer_free(qing_tokenizer_t* tokenizer) {
    qing_hash_free(tokenizer->vocab);
    qing_hash_free(tokenizer->merges);
    free(tokenizer);
}
    

qing_tokenizer_t* qing_tokenizer_load(const char* filename) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        printf("Failed to open file: %s\n", filename);
        return NULL;
    }

    fseek(fp, 0, SEEK_END);
    long file_size = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    char *json_str = (char*)malloc(file_size + 1);
    if (!json_str) {
        fclose(fp);
        return NULL;
    }

    if (fread(json_str, 1, file_size, fp) != (size_t)file_size) {
        fclose(fp);
        free(json_str);
        return NULL;
    }

    json_str[file_size] = '\0';
    fclose(fp);

    const char* ptr = json_str;
    qing_json_value_t* json = qing_json_parse(&ptr);
    if (!json) {
        free(json_str);
        return NULL;
    }

    qing_json_value_t* json_model = qing_json_get_value(json, "model");
    if (!json_model || !QING_JSON_IS_OBJECT(json_model)) {
        qing_json_free(json);
        free(json_str);
        return NULL;
    }

    qing_json_value_t* json_vocab = qing_json_get_value(json_model, "vocab");
    if (!json_vocab || !QING_JSON_IS_OBJECT(json_vocab)) {
        qing_json_free(json);
        free(json_str);
        return NULL;
    }

    qing_json_value_t* json_merges = qing_json_get_value(json_model, "merges");
    if (!json_merges || !QING_JSON_IS_ARRAY(json_vocab)) {
        qing_json_free(json);
        free(json_str);
        return NULL;
    }

    qing_tokenizer_t* tokenizer = qing_tokenizer_new();
    if (!tokenizer) {
        qing_json_free(json);
        free(json_str);
        return NULL;
    }

    /* vocab: { "token": id } */
    for (int i = 0; i < json_vocab->count; i++) {
        char* token = json_vocab->object.keys[i];
        qing_json_value_t* val = json_vocab->object.values[i];

        if (!QING_JSON_IS_INT(val)) continue;
	        
        qing_string_t key = qing_string_new(token);
        if (qing_string_len(key) > tokenizer->max_token_len) {
            tokenizer->max_token_len = qing_string_len(key);
        } 

        qing_hash_set(&tokenizer->vocab, key, INT_VAL(QING_JSON_AS_INT(val)));
    }

    qing_json_value_t* first_merge = json_merges->array[0]; 

    if (QING_JSON_IS_STRING(first_merge)) {
        for (int i = 0; i < json_merges->count; i++) {
            qing_json_value_t* val = json_merges->array[i];
            qing_string_t key = qing_string_new(STRING_VAL(val));
            qing_hash_set(&tokenizer->merges, key, INT_VAL(i));
        }
    }

    qing_json_free(json);
    free(json_str);
    return tokenizer;
}

