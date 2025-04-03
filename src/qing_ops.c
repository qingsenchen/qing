#include "qing_ops.h"
#include <stdlib.h>
#include <stdio.h>

qing_tensor_t* qing_op_create(qing_op_type_t op, qing_tensor_t* input1, qing_tensor_t* input2) {
    // 确保至少有一个输入
    if (!input1) {
        printf("Error: Operation requires at least one input tensor.\n");
        return NULL;
    }

    // 使用第一个输入的形状和数据类型创建新的 tensor
    qing_tensor_t* result = qing_tensor_create(input1->shape, input1->ndim, input1->dtype);
    if (!result) {
        printf("Error: Failed to create tensor for operation.\n");
        return NULL;
    }

    // 记录操作类型和输入
    result->op = op;
    result->inputs[0] = input1;
    result->inputs[1] = input2;

    return result;
}

// 定义所有算子
qing_tensor_t* qing_op_add(qing_tensor_t* a, qing_tensor_t* b) {
    return qing_op_create(QING_OP_ADD, a, b);
}

qing_tensor_t* qing_op_acc(qing_tensor_t* a, qing_tensor_t* b) {
    return qing_op_create(QING_OP_ACC, a, b);
}

qing_tensor_t* qing_op_sub(qing_tensor_t* a, qing_tensor_t* b) {
    return qing_op_create(QING_OP_SUB, a, b);
}

qing_tensor_t* qing_op_mul(qing_tensor_t* a, qing_tensor_t* b) {
    return qing_op_create(QING_OP_MUL, a, b);
}

qing_tensor_t* qing_op_div(qing_tensor_t* a, qing_tensor_t* b) {
    return qing_op_create(QING_OP_DIV, a, b);
}

qing_tensor_t* qing_op_sqr(qing_tensor_t* a) {
    return qing_op_create(QING_OP_SQR, a, NULL);
}

qing_tensor_t* qing_op_sqrt(qing_tensor_t* a) {
    return qing_op_create(QING_OP_SQRT, a, NULL);
}

qing_tensor_t* qing_op_log(qing_tensor_t* a) {
    return qing_op_create(QING_OP_LOG, a, NULL);
}

qing_tensor_t* qing_op_sin(qing_tensor_t* a) {
    return qing_op_create(QING_OP_SIN, a, NULL);
}

qing_tensor_t* qing_op_cos(qing_tensor_t* a) {
    return qing_op_create(QING_OP_COS, a, NULL);
}

qing_tensor_t* qing_op_sum(qing_tensor_t* a) {
    return qing_op_create(QING_OP_SUM, a, NULL);
}

qing_tensor_t* qing_op_sum_rows(qing_tensor_t* a) {
    return qing_op_create(QING_OP_SUM_ROWS, a, NULL);
}

qing_tensor_t* qing_op_mean(qing_tensor_t* a) {
    return qing_op_create(QING_OP_MEAN, a, NULL);
}

qing_tensor_t* qing_op_argmax(qing_tensor_t* a) {
    return qing_op_create(QING_OP_ARGMAX, a, NULL);
}

qing_tensor_t* qing_op_count_equal(qing_tensor_t* a, qing_tensor_t* b) {
    return qing_op_create(QING_OP_COUNT_EQUAL, a, b);
}

qing_tensor_t* qing_op_repeat(qing_tensor_t* a) {
    return qing_op_create(QING_OP_REPEAT, a, NULL);
}

qing_tensor_t* qing_op_repeat_back(qing_tensor_t* a) {
    return qing_op_create(QING_OP_REPEAT_BACK, a, NULL);
}

qing_tensor_t* qing_op_concat(qing_tensor_t* a, qing_tensor_t* b) {
    return qing_op_create(QING_OP_CONCAT, a, b);
}

qing_tensor_t* qing_op_silu_back(qing_tensor_t* a) {
    return qing_op_create(QING_OP_SILU_BACK, a, NULL);
}

qing_tensor_t* qing_op_norm(qing_tensor_t* a) {
    return qing_op_create(QING_OP_NORM, a, NULL);
}

qing_tensor_t* qing_op_rms_norm(qing_tensor_t* a) {
    return qing_op_create(QING_OP_RMS_NORM, a, NULL);
}

qing_tensor_t* qing_op_rms_norm_back(qing_tensor_t* a) {
    return qing_op_create(QING_OP_RMS_NORM_BACK, a, NULL);
}

qing_tensor_t* qing_op_group_norm(qing_tensor_t* a) {
    return qing_op_create(QING_OP_GROUP_NORM, a, NULL);
}

qing_tensor_t* qing_op_l2_norm(qing_tensor_t* a) {
    return qing_op_create(QING_OP_L2_NORM, a, NULL);
}

qing_tensor_t* qing_op_mul_mat(qing_tensor_t* a, qing_tensor_t* b) {
    return qing_op_create(QING_OP_MUL_MAT, a, b);
}

qing_tensor_t* qing_op_mul_mat_id(qing_tensor_t* a, qing_tensor_t* b) {
    return qing_op_create(QING_OP_MUL_MAT_ID, a, b);
}

qing_tensor_t* qing_op_out_prod(qing_tensor_t* a, qing_tensor_t* b) {
    return qing_op_create(QING_OP_OUT_PROD, a, b);
}
