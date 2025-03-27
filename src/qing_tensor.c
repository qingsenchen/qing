#include "qing_tensor.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdint.h>

#define PRINT_MAX_SIZE 5  // 超过该大小的维度会被省略部分

// 计算 stride 数组（每个维度的步长）
void compute_stride(const int* shape, int ndim, int* stride) {
    stride[ndim - 1] = 1;
    for (int i = ndim - 2; i >= 0; i--) {
        stride[i] = stride[i + 1] * shape[i + 1];
    }
}

// 创建张量，支持指定数据类型
qing_tensor_t* qing_tensor_create(const int* shape, int ndim, qing_dtype_t dtype) {
    qing_tensor_t* tensor = (qing_tensor_t*)malloc(sizeof(qing_tensor_t));
    if (!tensor) {
        printf("Error: Memory allocation failed for tensor.\n");
        return NULL;
    }

    // 分配内存并复制维度信息
    tensor->shape = (int*)malloc(sizeof(int) * ndim);
    if (!tensor->shape) {
        printf("Error: Memory allocation failed for shape.\n");
        free(tensor);
        return NULL;
    }

    memcpy(tensor->shape, shape, sizeof(int) * ndim);
    tensor->ndim = ndim;
    tensor->size = 1;

    // 计算张量元素的总数
    for (int i = 0; i < ndim; i++) {
        tensor->size *= shape[i];
    }

    // 为 stride 分配内存并计算步长
    tensor->stride = (int*)malloc(sizeof(int) * ndim);
    if (!tensor->stride) {
        printf("Error: Memory allocation failed for stride.\n");
        free(tensor->shape);
        free(tensor);
        return NULL;
    }
    compute_stride(shape, ndim, tensor->stride);

    // 设置数据类型
    tensor->dtype = dtype;
    tensor->ref_count = 1; // 默认引用计数为 1
    tensor->is_view = 0;

    // 根据数据类型分配内存
    size_t elem_size = 0;
    switch (dtype) {
        case QING_DTYPE_FLOAT32:
            elem_size = sizeof(float);
            break;
        case QING_DTYPE_FLOAT16:
            elem_size = sizeof(uint16_t); // 16-bit half precision
            break;
        case QING_DTYPE_BFLOAT16:
            elem_size = sizeof(uint16_t); // 16-bit bfloat16
            break;
        case QING_DTYPE_INT32:
            elem_size = sizeof(int32_t);
            break;
        case QING_DTYPE_UINT32:
            elem_size = sizeof(uint32_t);
            break;
        case QING_DTYPE_INT16:
            elem_size = sizeof(int16_t);
            break;
        case QING_DTYPE_UINT16:
            elem_size = sizeof(uint16_t);
            break;
        case QING_DTYPE_INT8:
            elem_size = sizeof(int8_t);
            break;
        case QING_DTYPE_UINT8:
            elem_size = sizeof(uint8_t);
            break;
        case QING_DTYPE_UINT4:
        case QING_DTYPE_INT4:
            elem_size = sizeof(uint8_t); // 4-bit stored as a byte
            break;
        default:
            printf("Error: Unsupported dtype.\n");
            free(tensor->shape);
            free(tensor->stride);
            free(tensor);
            return NULL;
    }

    tensor->data = malloc(tensor->size * elem_size);
    if (!tensor->data) {
        printf("Error: Memory allocation failed for tensor data.\n");
        free(tensor->shape);
        free(tensor->stride);
        free(tensor);
        return NULL;
    }

    memset(tensor->data, 0, tensor->size * elem_size);

    return tensor;
}


// 释放张量
void qing_tensor_free(qing_tensor_t* tensor) {
    if (tensor) {
        if (tensor->ref_count > 1) {
            // 共享内存，引用计数减 1
            tensor->ref_count--;
        } else {
            // 唯一引用，释放内存
            if (tensor->data) {
                free(tensor->data);
            }
            if (tensor->shape) {
                free(tensor->shape);
            }
            free(tensor);
        }
    }
}

// 获取张量数据指针
void* qing_tensor_data(qing_tensor_t* tensor) {
    if (!tensor) return NULL;
    return tensor->data;
}

// 打印张量数据（递归）
void print_recursive(void *data, int *shape, int dim, int current_dim, int *index, int stride, qing_dtype_t dtype) {
    if (current_dim == dim - 1) {
        printf("[");
        for (int i = 0; i < shape[current_dim]; i++) {
            if (i >= PRINT_MAX_SIZE / 2 && i < shape[current_dim] - PRINT_MAX_SIZE / 2) {
                if (i == PRINT_MAX_SIZE / 2) {
                    printf(" ...");
                }
                continue;
            }
            int flat_index = 0;
            for (int d = 0; d < dim; d++) {
                flat_index += index[d] * stride;
            }
            flat_index += i;

            // 根据数据类型格式化输出
            switch (dtype) {
                case QING_DTYPE_FLOAT32:
                    printf("%8.3f", ((float *)data)[flat_index]);
                    break;
                case QING_DTYPE_FLOAT16:
                    printf("%8.3f", ((float *)data)[flat_index]);
                    break;
                case QING_DTYPE_INT32:
                    printf("%6d", ((int *)data)[flat_index]);
                    break;
                case QING_DTYPE_INT16:
                    printf("%6d", ((short *)data)[flat_index]);
                    break;
                case QING_DTYPE_INT8:
                    printf("%4d", ((int8_t *)data)[flat_index]);
                    break;
                case QING_DTYPE_UINT8:
                    printf("%4u", ((uint8_t *)data)[flat_index]);
                    break;
                case QING_DTYPE_UINT4: {
                    // 位操作提取 4-bit 值
                    uint8_t byte = ((uint8_t *)data)[flat_index];
                    int shift = (flat_index % 2) * 4;  // 判断是高4位还是低4位
                    uint8_t value = (byte >> shift) & 0xF;
                    printf("%1X", value);  // 打印 4-bit 十六进制值
                    break;
                }
                case QING_DTYPE_INT4: {
                    // 位操作提取 4-bit 有符号值
                    uint8_t byte = ((uint8_t *)data)[flat_index];
                    int shift = (flat_index % 2) * 4;  // 判断是高4位还是低4位
                    int8_t value = (int8_t)((byte >> shift) & 0xF); // 4-bit 有符号值
                    printf("%1d", value);  // 打印 4-bit 十进制值
                    break;
                }
                default:
                    printf("%8s", "?");
            }

            if (i < shape[current_dim] - 1) {
                printf(", ");
            }
        }
        printf("]");
    } else {
        printf("[\n");
        for (int i = 0; i < shape[current_dim]; i++) {
            if (i >= PRINT_MAX_SIZE / 2 && i < shape[current_dim] - PRINT_MAX_SIZE / 2) {
                if (i == PRINT_MAX_SIZE / 2) {
                    printf("  ...\n");
                }
                continue;
            }
            index[current_dim] = i;
            for (int j = 0; j < current_dim; j++) {
                printf(" ");
            }
            print_recursive(data, shape, dim, current_dim + 1, index, stride, dtype);
            if (i < shape[current_dim] - 1) {
                printf(",\n");
            }
        }
        printf("\n");
        for (int j = 0; j < current_dim; j++) {
            printf(" ");
        }
        printf("]");
    }
}

// 打印 QingTensor
void qing_tensor_print(const qing_tensor_t* tensor) {
    if (!tensor || !tensor->data) {
        printf("tensor(None)\n");
        return;
    }

    int dim = tensor->ndim;
    int *shape = tensor->shape;
    int total_size = 1;
    for (int i = 0; i < dim; i++) {
        total_size *= shape[i];
    }

    printf("tensor(");
    int *index = (int *)calloc(dim, sizeof(int));
    print_recursive(tensor->data, shape, dim, 0, index, total_size / shape[0], tensor->dtype);
    free(index);
    printf(")\n");
}

// 张量赋值（可以是标量或者另一个张量）
void qing_tensor_fill(qing_tensor_t* tensor, float value) {
    if (tensor && tensor->data) {
        for (int i = 0; i < tensor->size; i++) {
            switch (tensor->dtype) {
                case QING_DTYPE_FLOAT32:
                    ((float*)tensor->data)[i] = value;
                    break;
                case QING_DTYPE_FLOAT16:
                    ((uint16_t*)tensor->data)[i] = (uint16_t)(value * 32768); // 简单转换为 16-bit float
                    break;
                case QING_DTYPE_INT8:
                    ((int8_t*)tensor->data)[i] = (int8_t)value;
                    break;
                default:
                    printf("Error: Unsupported dtype for fill operation.\n");
                    return;
            }
        }
    }
}
