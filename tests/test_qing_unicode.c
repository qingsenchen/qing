#include "utils/qing_unicode.h"
#include <stdio.h>
#include <string.h>

int main() {
    // 测试用例1：ASCII + 中文 + emoji
    const char *str1 = "A中😀";
    size_t len1 = strlen(str1);
    size_t offset = 0;

    printf("测试用例1 - 混合字符串 \"A中😀\":\n");
    while (offset < len1) {
        uint32_t cp = unicode_cpt_from_utf8(str1, len1, &offset);
        printf("codepoint: U+%04X\n", cp);
    }

    // 测试用例2：纯ASCII
    printf("\n测试用例2 - ASCII字符串 \"Hello\":\n");
    const char *str2 = "Hello";
    size_t len2 = strlen(str2);
    offset = 0;
    while (offset < len2) {
        uint32_t cp = unicode_cpt_from_utf8(str2, len2, &offset);
        printf("codepoint: U+%04X\n", cp);
    }

    // 测试用例3：中文字符串
    printf("\n测试用例3 - 中文字符串 \"你好世界\":\n");
    const char *str3 = "你好世界";
    size_t len3 = strlen(str3);
    offset = 0;
    while (offset < len3) {
        uint32_t cp = unicode_cpt_from_utf8(str3, len3, &offset);
        printf("codepoint: U+%04X\n", cp);
    }

    // 测试用例4：emoji字符串
    printf("\n测试用例4 - Emoji字符串 \"😀😊🎉\":\n");
    const char *str4 = "😀😊🎉";
    size_t len4 = strlen(str4);
    offset = 0;
    while (offset < len4) {
        uint32_t cp = unicode_cpt_from_utf8(str4, len4, &offset);
        printf("codepoint: U+%04X\n", cp);
    }

    // 测试用例5：日文字符串
    printf("\n测试用例5 - 日文字符串 \"こんにちは世界\":\n");
    const char *str5 = "こんにちは世界";
    size_t len5 = strlen(str5);
    offset = 0;
    while (offset < len5) {
        uint32_t cp = unicode_cpt_from_utf8(str5, len5, &offset);
        printf("codepoint: U+%04X\n", cp);
    }

    // 测试用例6：韩文字符串
    printf("\n测试用例6 - 韩文字符串 \"안녕하세요\":\n");
    const char *str6 = "안녕하세요";
    size_t len6 = strlen(str6);
    offset = 0;
    while (offset < len6) {
        uint32_t cp = unicode_cpt_from_utf8(str6, len6, &offset);
        printf("codepoint: U+%04X\n", cp);
    }

    // 测试用例7：混合多语言
    printf("\n测试用例7 - 混合多语言 \"Hello你好こんにちは안녕😊\":\n");
    const char *str7 = "Hello你好こんにちは안녕😊";
    size_t len7 = strlen(str7);
    offset = 0;
    while (offset < len7) {
        uint32_t cp = unicode_cpt_from_utf8(str7, len7, &offset);
        printf("codepoint: U+%04X\n", cp);
    }

    return 0;
}