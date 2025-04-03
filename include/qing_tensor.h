#ifndef QING_TENSOR_H
#define QING_TENSOR_H

#include <stdint.h>
#include <stddef.h>  // for size_t

#define QING_MAX_INPUTS 4  // 最多支持 4 个输入

// 张量的计算类型
typedef enum {
    QING_OP_NONE,  // 普通张量（数据）
    QING_OP_ADD,   // 加法
    QING_OP_MUL,   // 乘法
    QING_OP_RELU   // ReLU
} qing_op_type_t;

typedef enum {
    QING_DTYPE_FLOAT32 = 0,
    QING_DTYPE_INT32,
    QING_DTYPE_UINT32,
    QING_DTYPE_FLOAT16,
    QING_DTYPE_BFLOAT16,
    QING_DTYPE_INT16,
    QING_DTYPE_UINT16,
    QING_DTYPE_INT8,
    QING_DTYPE_UINT8,
    QING_DTYPE_UINT4,
    QING_DTYPE_INT4
} qing_dtype_t;

typedef struct qing_tensor {
    int* shape;          // 形状数组
    int* stride;         // 步长数组（支持不同存储布局）
    int ndim;            // 维度数
    size_t size;         // 总元素数
    void* data;          // 指向数据
    int ref_count;       // 引用计数
    int is_view;         // 是否是视图
    qing_dtype_t dtype;  // 数据类型

    // 计算图相关
    qing_op_type_t op;          // 计算类型
    struct qing_tensor* inputs[QING_MAX_INPUTS];  // 输入张量
    int num_inputs;             // 当前输入张量数量
} qing_tensor_t;

// 创建张量（自动计算 stride）
qing_tensor_t* qing_tensor_create(const int* shape, int ndim, qing_dtype_t dtype);

// 释放张量
void qing_tensor_free(qing_tensor_t* tensor);

// 获取张量数据指针
void* qing_tensor_data(qing_tensor_t* tensor);

// 打印张量信息（格式类似 PyTorch）
void qing_tensor_print(const qing_tensor_t* tensor);

// 填充张量（支持广播）
void qing_tensor_fill(qing_tensor_t* tensor, float value);

// 获取张量某个元素（索引形式）
float qing_tensor_get(const qing_tensor_t* tensor, const int* indices);

// 设置张量某个元素
void qing_tensor_set(qing_tensor_t* tensor, const int* indices, float value);

#endif // QING_TENSOR_H
