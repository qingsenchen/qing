#include "qing_model.h" 
#include <stdio.h>

void* qing_load_model(const char* path) {
    for (int i = 0; i < g_loader_count; ++i) {
        if (g_loaders[i]->can_load(path)) {
            return g_loaders[i]->load(path);
        }
    }
    fprintf(stderr, "Unsupported model format: %s\n", path);
    return NULL;
}