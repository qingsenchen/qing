#ifndef QING_TRANSFORMERS_H
#define QING_TRANSFORMERS_H

#include "qing_tensor.h"

typedef struct {
    int vocab_size;
    int hidden_size;
    int n_layers;
    int n_heads;
    int intermediate_size;
    int max_position_embeddings;

    qing_tensor_t* tok_embds;
    qing_tensor_t* pos_embds;

    qing_tensor_t* final_norm_weight;
    qing_tensor_t* final_norm_bias;

    qing_tensor_t* o_proj_weight;
} qing_tf_model_t;

// SafeTensors 加载函数
int qing_safetensors_load(const char* path);



#endif // QING_TRANSFORMERS_H