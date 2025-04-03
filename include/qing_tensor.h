#ifndef QING_TENSOR_H
#define QING_TENSOR_H

#include <stdint.h>
#include <stddef.h>  // for size_t

#define QING_MAX_INPUTS 4  // 最多支持 4 个输入

// 张量的计算类型
typedef enum {
    QING_OP_NONE = 0,     // 无操作

    QING_OP_ADD,          // 加法
    QING_OP_ACC,          // 累加
    QING_OP_SUB,          // 减法
    QING_OP_MUL,          // 乘法
    QING_OP_DIV,          // 除法
    QING_OP_SQR,          // 平方
    QING_OP_SQRT,         // 平方根
    QING_OP_LOG,          // 对数
    QING_OP_SIN,          // 正弦
    QING_OP_COS,          // 余弦
    QING_OP_SUM,          // 求和
    QING_OP_SUM_ROWS,     // 行求和
    QING_OP_MEAN,         // 平均值
    QING_OP_ARGMAX,       // 最大值索引
    QING_OP_COUNT_EQUAL,  // 相等计数
    QING_OP_REPEAT,       // 重复
    QING_OP_REPEAT_BACK,  // 反向重复
    QING_OP_CONCAT,       // 拼接
    QING_OP_RELU,    // SiLU反向
    QING_OP_SILU_BACK,    // SiLU反向
    QING_OP_NORM,         // 归一化
    QING_OP_RMS_NORM,     // RMS归一化
    QING_OP_RMS_NORM_BACK,// RMS归一化反向
    QING_OP_GROUP_NORM,   // 组归一化
    QING_OP_L2_NORM,      // L2归一化
    QING_OP_MUL_MAT,      // 矩阵乘法
    QING_OP_MUL_MAT_ID,   // 矩阵ID乘法
    QING_OP_OUT_PROD      // 外积
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
