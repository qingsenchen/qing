#include "qing_transformers.h"
#include "utils/qing_json.h"
#include "qing_model.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

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