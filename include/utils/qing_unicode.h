#ifndef QING_UNICODE_H
#define QING_UNICODE_H

#include <stdint.h>
#include <stddef.h>

/**
 * @brief 从UTF-8字符串中解码一个Unicode码点
 * 
 * @param utf8 UTF-8编码的字符串
 * @param utf8_size 字符串的总长度（字节数）
 * @param offset 当前处理位置的偏移量，函数会更新这个值
 * @return uint32_t 解码后的Unicode码点
 * 
 * @note 如果遇到无效的UTF-8序列，函数会输出错误信息并退出程序
 * @note offset参数会被更新为下一个字符的起始位置
 */
uint32_t unicode_cpt_from_utf8(const char *utf8, size_t utf8_size, size_t *offset);

#endif // QING_UNICODE_H