#ifndef QING_MODEL_H
#define QING_MODEL_H
#include <stdbool.h>
#include "qing_model.h"
#define QING_MAX_MODEL_LOADERS 8

typedef struct {
    const char format_name[64];

    // 判断能否加载该格式（通过文件名、magic bytes等）
    bool (*can_load)(const char* path);

    // 加载模型结构+权重
    void* (*load)(const char* path);

    // 推理框架中的内部模型结构（如 Transformer）
    void (*free)(void* model);
} qing_model_loader_t;

static qing_model_loader_t* g_loaders[QING_MAX_MODEL_LOADERS];
static int g_loader_count = 0;

void qing_model_loader_register(qing_model_loader_t* loader) {
    if (g_loader_count < QING_MAX_MODEL_LOADERS) {
        g_loaders[g_loader_count++] = loader;
    }
}

#endif // QING_MODEL_H