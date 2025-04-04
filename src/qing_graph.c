#include "qing_graph.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define QING_GRAPH_DEFAULT_CAPACITY 2048

qing_graph_t* qing_graph_alloc(void) {
    qing_graph_t* graph = (qing_graph_t*)malloc(sizeof(qing_graph_t));
    if (!graph) {
        return NULL;
    }

    graph->nodes = (qing_tensor_t**)malloc(QING_GRAPH_DEFAULT_CAPACITY * sizeof(qing_tensor_t*));
    if (!graph->nodes) {
        free(graph);
        return NULL;
    }

    graph->nb_nodes = 0;
    graph->capacity = QING_GRAPH_DEFAULT_CAPACITY;
    return graph;
}

void qing_graph_free(qing_graph_t* graph) {
    if (!graph) return;
    free(graph->nodes);
    free(graph);
}

int qing_graph_add_node(qing_graph_t* graph, qing_tensor_t* node) {
    if (!graph || !node) return -1;

    if (graph->nb_nodes >= graph->capacity) {
        int new_capacity = graph->capacity * 2;
        qing_tensor_t** new_nodes = (qing_tensor_t**)realloc(graph->nodes, 
                                                            new_capacity * sizeof(qing_tensor_t*));
        if (!new_nodes) return -1;

        graph->nodes = new_nodes;
        graph->capacity = new_capacity;
    }

    graph->nodes[graph->nb_nodes] = node;
    return graph->nb_nodes++;
}

void qing_graph_forward_add(qing_graph_t *graph, qing_tensor_t *out) {
    qing_tensor_t *a = out->inputs[0];
    qing_tensor_t *b = out->inputs[1];
    
    if (!a || !b || a->dtype != b->dtype) {
        fprintf(stderr, "Add operation requires two tensors with same dtype\n");
        return;
    }

    const size_t total = a->size;
    
    // 逐元素加法运算
    switch(a->dtype) {
        case QING_DTYPE_FLOAT32: {
            float* a_data = (float*)a->data;
            float* b_data = (float*)b->data;
            float* out_data = (float*)out->data;
            for (size_t i = 0; i < total; i++) {
                out_data[i] = a_data[i] + b_data[i];
            }
            break;
        }
        case QING_DTYPE_INT32: {
            int32_t* a_data = (int32_t*)a->data;
            int32_t* b_data = (int32_t*)b->data;
            int32_t* out_data = (int32_t*)out->data;
            for (size_t i = 0; i < total; i++) {
                out_data[i] = a_data[i] + b_data[i];
            }
            break;
        }
        case QING_DTYPE_UINT8: {
            uint8_t* a_data = (uint8_t*)a->data;
            uint8_t* b_data = (uint8_t*)b->data;
            uint8_t* out_data = (uint8_t*)out->data;
            for (size_t i = 0; i < total; i++) {
                out_data[i] = a_data[i] + b_data[i];
            }
            break;
        }
        default:
            fprintf(stderr, "Unsupported dtype for add operation: %d\n", a->dtype);
            break;
    }
}

int qing_graph_forward(qing_graph_t* graph) {
    if (!graph) return -1;

    for (int i = 0; i < graph->nb_nodes; i++) {
        qing_tensor_t* tensor = graph->nodes[i];
        if (tensor->op != QING_OP_NONE) {
            switch (tensor->op) {
                case QING_OP_ADD:
                    qing_graph_forward_add(graph, tensor);  // 添加这行调用
                    break;
                case QING_OP_MUL:
                    // 执行乘法操作
                    break;
                case QING_OP_RELU:
                    // 执行 ReLU 操作
                    break;
                default:
                    break;
            }
        }
    }

    return 0;
}

int qing_graph_backward(qing_graph_t* graph) {
    if (!graph) return -1;

    for (int i = graph->nb_nodes - 1; i >= 0; i--) {
        qing_tensor_t* tensor = graph->nodes[i];
        if (tensor->op != QING_OP_NONE) {
            // TODO: 实现反向传播逻辑
        }
    }

    return 0;
}

int qing_graph_step(qing_graph_t* graph, float learning_rate) {
    if (!graph) return -1;

    for (int i = 0; i < graph->nb_nodes; i++) {
        qing_tensor_t* tensor = graph->nodes[i];
        // TODO: 实现参数更新逻辑
    }

    return 0;
}