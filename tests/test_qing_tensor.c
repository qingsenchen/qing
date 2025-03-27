#include <stdio.h>
#include <assert.h>
#include "qing_tensor.h"

// 检查张量创建和销毁
void test_tensor_create_free() {
    int shape[] = {2, 3, 4};
    qing_tensor_t* tensor = qing_tensor_create(shape, 3, QING_DTYPE_FLOAT32);
    
    assert(tensor != NULL);
    assert(tensor->shape != NULL);
    assert(tensor->data != NULL);
    assert(tensor->ndim == 3);
    assert(tensor->size == 24);
    
    // 打印张量信息
    qing_tensor_print(tensor);

    // 释放张量
    qing_tensor_free(tensor);
    printf("test_tensor_create_free passed!\n");
}

// 检查张量数据填充
void test_tensor_fill() {
    int shape[] = {3, 3, 3};
    qing_tensor_t* tensor = qing_tensor_create(shape, 3, QING_DTYPE_FLOAT32);

    // 填充张量数据
    qing_tensor_fill(tensor, 1.23f);

    // 检查每个元素是否都为 1.23
    for (int i = 0; i < tensor->size; i++) {
        assert(((float*)tensor->data)[i] == 1.23f);
    }

    // 释放张量
    qing_tensor_free(tensor);
    printf("test_tensor_fill passed!\n");
}

// // 检查张量元素访问
// void test_tensor_get_set() {
//     int shape[] = {2, 3};
//     qing_tensor_t* tensor = qing_tensor_create(shape, 2, QING_DTYPE_FLOAT32);

//     // 设置特定位置的值
//     int indices[] = {0, 1};
//     qing_tensor_set(tensor, indices, 5.67f);

//     // 获取特定位置的值
//     float value = qing_tensor_get(tensor, indices);
    
//     assert(value == 5.67f);
    
//     // 释放张量
//     qing_tensor_free(tensor);
//     printf("test_tensor_get_set passed!\n");
// }

// 检查张量打印
void test_tensor_print() {
    int shape[] = {2, 2};
    qing_tensor_t* tensor = qing_tensor_create(shape, 2, QING_DTYPE_FLOAT32);

    // 填充张量
    qing_tensor_fill(tensor, 9.87f);

    // 打印张量
    qing_tensor_print(tensor);

    // 释放张量
    qing_tensor_free(tensor);
    printf("test_tensor_print passed!\n");
}

// 检查张量的数据类型（16-bit float）
void test_tensor_dtype() {
    int shape[] = {2, 3};
    qing_tensor_t* tensor = qing_tensor_create(shape, 2, QING_DTYPE_FLOAT16);

    // 填充张量
    qing_tensor_fill(tensor, 1.23f);

    // 检查数据类型和填充值
    assert(tensor->dtype == QING_DTYPE_FLOAT16);
    assert(((uint16_t*)tensor->data)[0] == (uint16_t)(1.23f * 32768));

    // 释放张量
    qing_tensor_free(tensor);
    printf("test_tensor_dtype passed!\n");
}

// 运行所有测试
int main() {
    test_tensor_create_free();
    test_tensor_fill();
    //test_tensor_get_set();
    test_tensor_print();
    test_tensor_dtype();

    printf("All tests passed!\n");
    return 0;
}
