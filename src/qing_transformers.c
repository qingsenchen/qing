#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "qing_transformers.h"
#include "qing_tokenizer.h"
#include "qing_model.h"
#include "utils/qing_json.h"

bool qing_safetensors_can_load(const char* path) {
    return strstr(path, ".safetensors") != NULL;
}

void* qing_hf_load(const char* path) {
    return NULL;
}

qing_model_loader_t safetensors_loader = {
    .format_name = "safetensors",
    .can_load = qing_safetensors_can_load,
    .load = qing_hf_load,
    .free = NULL
};

static uint64_t read_u64_le(FILE* fp) {
    uint8_t buf[8];
    fread(buf, 1, 8, fp);
    uint64_t len = 0;
    for (int i = 0; i < 8; ++i) {
        len |= ((uint64_t)buf[i]) << (8 * i);
    }
    return len;
}

int qing_safetensors_load(const char* path) {
    FILE* fp = fopen(path, "rb");
    if (!fp) return -1;

    uint64_t header_len = read_u64_le(fp);

    char* header_buf = (char*)malloc(header_len + 1);
    fread(header_buf, 1, header_len, fp);
    fclose(fp);

    header_buf[header_len] = '\0';

    const char* json_ptr = header_buf;
    qing_json_value_t* header = qing_json_parse(&json_ptr);
    free(header_buf);
    
    if (header) {
        qing_json_print_pretty(header);
        qing_json_free(header);
    }

    return 0; // TODO: implement
}

bool qing_safetensors_vocab_load(const char* path) {
    FILE *fp = fopen(path, "r");
    if (fp == NULL) {
        printf("Failed to open file: %s\n", path);
        return false;
    }
    
    fseek(fp, 0, SEEK_END);
    long file_size = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    char *json_str = malloc(file_size + 1);
    if (json_str == NULL) {
        printf("Failed to allocate memory for JSON string.\n");
        fclose(fp);
        return false;
    }

    // 读取文件内容到json_str
    size_t read_size = fread(json_str, 1, file_size, fp);
    json_str[read_size] = '\0';  // 添加字符串结束符
    fclose(fp);
    
    const char* ptr = json_str;
    qing_json_value_t* json = qing_json_parse(&ptr);

    // // 创建新的词汇表
    qing_vocab_t* vocab = malloc(sizeof(qing_vocab_t));
    vocab->token_to_id.count = 0;

    // 遍历JSON对象，将词汇添加到词汇表中
    for (int i = 0; i < json->count; i++) {
        const char* token = json->object.keys[i];
        int id = json->object.values[i]->integer;
        // 添加到token_to_id映射
        //qing_vocab_add_token(vocab, token, id);
    }



    return vocab;
}