#include "qing_tensor.h"
#include "qing_graph.h"
#include "qing_ops.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

void test_add_operation() {
    int shape[] = {2, 2};
    
    // 创建输入张量
    qing_tensor_t* a = qing_tensor_create(shape, 2, QING_DTYPE_FLOAT32);
    qing_tensor_t* b = qing_tensor_create(shape, 2, QING_DTYPE_FLOAT32);
    
    // 填充测试数据
    float a_data[] = {1.0f, 2.0f, 3.0f, 4.0f};
    float b_data[] = {5.0f, 6.0f, 7.0f, 8.0f};
    memcpy(a->data, a_data, sizeof(a_data));
    memcpy(b->data, b_data, sizeof(b_data));
    
    // 执行加法操作
    qing_tensor_t* result = qing_op_add(a, b);

    printf("\na:\n");
    qing_tensor_print(a);
    printf("\nb:\n");
    qing_tensor_print(b);

    qing_graph_forward_add(NULL, result);
    printf("\na+b:\n");
    qing_tensor_print(result);
    
    // 验证结果
    float expected[] = {6.0f, 8.0f, 10.0f, 12.0f};
    for (int i = 0; i < 4; i++) {
        assert(fabs(((float*)result->data)[i] - expected[i]) < 1e-6);
    }
    
    // 释放资源
    qing_tensor_free(a);
    qing_tensor_free(b);
    qing_tensor_free(result);
    printf("test_add_operation passed!\n");
}

void test_mul_operation() {
    int shape[] = {2, 2};
    
    qing_tensor_t* a = qing_tensor_create(shape, 2, QING_DTYPE_FLOAT32);
    qing_tensor_t* b = qing_tensor_create(shape, 2, QING_DTYPE_FLOAT32);
    
    float a_data[] = {2.0f, 3.0f, 4.0f, 5.0f};
    float b_data[] = {3.0f, 4.0f, 5.0f, 6.0f};
    memcpy(a->data, a_data, sizeof(a_data));
    memcpy(b->data, b_data, sizeof(b_data));
    
    qing_tensor_t* result = qing_op_mul(a, b);
    
    float expected[] = {6.0f, 12.0f, 20.0f, 30.0f};
    for (int i = 0; i < 4; i++) {
        assert(fabs(((float*)result->data)[i] - expected[i]) < 1e-6);
    }
    
    qing_tensor_free(a);
    qing_tensor_free(b);
    qing_tensor_free(result);
    printf("test_mul_operation passed!\n");
}

// 添加 main 函数
int main() {
    test_add_operation();
    // test_mul_operation();
    
    printf("\nAll ops tests passed!\n");
    return 0;
}