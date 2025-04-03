#ifndef QING_OPS_H
#define QING_OPS_H

#include "qing_tensor.h"

// 创建算子的通用函数
qing_tensor_t* qing_op_create(qing_op_type_t op, qing_tensor_t* input1, qing_tensor_t* input2);

// 算子函数声明
qing_tensor_t* qing_op_add(qing_tensor_t* a, qing_tensor_t* b);
qing_tensor_t* qing_op_acc(qing_tensor_t* a, qing_tensor_t* b);
qing_tensor_t* qing_op_sub(qing_tensor_t* a, qing_tensor_t* b);
qing_tensor_t* qing_op_mul(qing_tensor_t* a, qing_tensor_t* b);
qing_tensor_t* qing_op_div(qing_tensor_t* a, qing_tensor_t* b);
qing_tensor_t* qing_op_sqr(qing_tensor_t* a);
qing_tensor_t* qing_op_sqrt(qing_tensor_t* a);
qing_tensor_t* qing_op_log(qing_tensor_t* a);
qing_tensor_t* qing_op_sin(qing_tensor_t* a);
qing_tensor_t* qing_op_cos(qing_tensor_t* a);
qing_tensor_t* qing_op_sum(qing_tensor_t* a);
qing_tensor_t* qing_op_sum_rows(qing_tensor_t* a);
qing_tensor_t* qing_op_mean(qing_tensor_t* a);
qing_tensor_t* qing_op_argmax(qing_tensor_t* a);
qing_tensor_t* qing_op_count_equal(qing_tensor_t* a, qing_tensor_t* b);
qing_tensor_t* qing_op_repeat(qing_tensor_t* a);
qing_tensor_t* qing_op_repeat_back(qing_tensor_t* a);
qing_tensor_t* qing_op_concat(qing_tensor_t* a, qing_tensor_t* b);
qing_tensor_t* qing_op_silu_back(qing_tensor_t* a);
qing_tensor_t* qing_op_norm(qing_tensor_t* a);
qing_tensor_t* qing_op_rms_norm(qing_tensor_t* a);
qing_tensor_t* qing_op_rms_norm_back(qing_tensor_t* a);
qing_tensor_t* qing_op_group_norm(qing_tensor_t* a);
qing_tensor_t* qing_op_l2_norm(qing_tensor_t* a);
qing_tensor_t* qing_op_mul_mat(qing_tensor_t* a, qing_tensor_t* b);
qing_tensor_t* qing_op_mul_mat_id(qing_tensor_t* a, qing_tensor_t* b);
qing_tensor_t* qing_op_out_prod(qing_tensor_t* a, qing_tensor_t* b);

#endif // QING_OPS_H
