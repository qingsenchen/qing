#ifndef QING_GRAPH_H
#define QING_GRAPH_H

#include "qing_tensor.h"

// 计算图结构
typedef struct qing_graph {
    qing_tensor_t** nodes;     // 所有张量节点
    int nb_nodes;           // 节点数量
    int capacity;              // 当前分配的容量
} qing_graph_t;

// 创建计算图
qing_graph_t* qing_graph_alloc(void);

// 销毁计算图
void qing_graph_free(qing_graph_t* graph);

// 添加张量到计算图
int qing_graph_add(qing_graph_t* graph, qing_tensor_t* node);

// 前向传播
int qing_graph_forward(qing_graph_t* graph);

// 反向传播
int qing_graph_backward(qing_graph_t* graph);

// 优化器步进
int qing_graph_step(qing_graph_t* graph, float learning_rate);

#endif // QING_GRAPH_H