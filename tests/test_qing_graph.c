#include <stdio.h>
#include <assert.h>
#include "qing_graph.h"
#include "qing_tensor.h"

// 测试计算图创建和销毁
void test_graph_create_free() {
    qing_graph_t* graph = qing_graph_alloc();
    assert(graph != NULL);
    assert(graph->nodes != NULL);
    assert(graph->nb_nodes == 0);
    assert(graph->capacity > 0);
    
    qing_graph_free(graph);
    printf("test_graph_create_free passed!\n");
}

// 测试添加节点
void test_graph_add() {
    qing_graph_t* graph = qing_graph_alloc();
    
    // 创建几个测试张量
    int shape[] = {2, 3};
    qing_tensor_t* t1 = qing_tensor_create(shape, 2, QING_DTYPE_FLOAT32);
    qing_tensor_t* t2 = qing_tensor_create(shape, 2, QING_DTYPE_FLOAT32);
    qing_tensor_t* t3 = qing_tensor_create(shape, 2, QING_DTYPE_FLOAT32);
    
    // 添加到图中
    assert(qing_graph_add_node(graph, t1) == 0);
    assert(qing_graph_add_node(graph, t2) == 1);
    assert(qing_graph_add_node(graph, t3) == 2);
    
    assert(graph->nb_nodes == 3);
    assert(graph->nodes[0] == t1);
    assert(graph->nodes[1] == t2);
    assert(graph->nodes[2] == t3);
    
    qing_tensor_free(t1);
    qing_tensor_free(t2);
    qing_tensor_free(t3);
    qing_graph_free(graph);
    printf("test_graph_add passed!\n");
}

// 测试前向传播
void test_graph_forward() {
    qing_graph_t* graph = qing_graph_alloc();
    
    // 创建输入张量
    int shape[] = {2, 2};
    qing_tensor_t* t1 = qing_tensor_create(shape, 2, QING_DTYPE_FLOAT32);
    qing_tensor_t* t2 = qing_tensor_create(shape, 2, QING_DTYPE_FLOAT32);
    qing_tensor_t* t3 = qing_tensor_create(shape, 2, QING_DTYPE_FLOAT32);
    
    // 设置操作类型和输入关系
    t3->op = QING_OP_ADD;
    t3->inputs[0] = t1;
    t3->inputs[1] = t2;
    t3->num_inputs = 2;
    
    // 填充输入数据
    qing_tensor_fill(t1, 1.0f);
    qing_tensor_fill(t2, 2.0f);
    
    // 添加到图中
    qing_graph_add_node(graph, t1);
    qing_graph_add_node(graph, t2);
    qing_graph_add_node(graph, t3);
    
    // 执行前向传播
    assert(qing_graph_forward(graph) == 0);
    
    // TODO: 验证计算结果
    
    qing_tensor_free(t1);
    qing_tensor_free(t2);
    qing_tensor_free(t3);
    qing_graph_free(graph);
    printf("test_graph_forward passed!\n");
}

// 运行所有测试
int main() {
    test_graph_create_free();
    test_graph_add();
    test_graph_forward();
    
    printf("All graph tests passed!\n");
    return 0;
}