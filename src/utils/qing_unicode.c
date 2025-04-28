#include <stdint.h>
#include <stddef.h>  // for size_t
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

/*
 * UTF-8编码规则：
 * - 1字节: 0xxxxxxx                    (0x00-0x7F)
 * - 2字节: 110xxxxx 10xxxxxx           (0xC0-0xDF) (0x80-0xBF)
 * - 3字节: 1110xxxx 10xxxxxx 10xxxxxx  (0xE0-0xEF) (0x80-0xBF) (0x80-0xBF)
 * - 4字节: 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
 *         (0xF0-0xF7) (0x80-0xBF) (0x80-0xBF) (0x80-0xBF)
 */
uint32_t unicode_cpt_from_utf8(const char *utf8, size_t utf8_size, size_t *offset) {
    assert(offset != NULL);
    assert(*offset < utf8_size);

    uint8_t byte0 = (uint8_t)utf8[*offset];

    if ((byte0 & 0x80) == 0) {
        uint32_t result = byte0;
        (*offset) += 1;
        return result;
    }
    if ((byte0 & 0x40) == 0) {
        fprintf(stderr, "invalid character\n");
        exit(EXIT_FAILURE);
    }
    if ((byte0 & 0x20) == 0) {
        if ((*offset) + 1 >= utf8_size) {
            fprintf(stderr, "unexpected end of input\n");
            exit(EXIT_FAILURE);
        }
        uint8_t byte1 = (uint8_t)utf8[(*offset) + 1];
        if ((byte1 & 0xc0) != 0x80) {
            fprintf(stderr, "invalid continuation byte\n");
            exit(EXIT_FAILURE);
        }
        uint32_t result = ((byte0 & 0x1f) << 6) | (byte1 & 0x3f);
        (*offset) += 2;
        return result;
    }
    if ((byte0 & 0x10) == 0) {
        if ((*offset) + 2 >= utf8_size) {
            fprintf(stderr, "unexpected end of input\n");
            exit(EXIT_FAILURE);
        }
        uint8_t byte1 = (uint8_t)utf8[(*offset) + 1];
        uint8_t byte2 = (uint8_t)utf8[(*offset) + 2];
        if ((byte1 & 0xc0) != 0x80 || (byte2 & 0xc0) != 0x80) {
            fprintf(stderr, "invalid continuation bytes\n");
            exit(EXIT_FAILURE);
        }
        uint32_t result = ((byte0 & 0x0f) << 12) | ((byte1 & 0x3f) << 6) | (byte2 & 0x3f);
        (*offset) += 3;
        return result;
    }
    if ((byte0 & 0x08) == 0) {
        if ((*offset) + 3 >= utf8_size) {
            fprintf(stderr, "unexpected end of input\n");
            exit(EXIT_FAILURE);
        }
        uint8_t byte1 = (uint8_t)utf8[(*offset) + 1];
        uint8_t byte2 = (uint8_t)utf8[(*offset) + 2];
        uint8_t byte3 = (uint8_t)utf8[(*offset) + 3];
        if ((byte1 & 0xc0) != 0x80 || (byte2 & 0xc0) != 0x80 || (byte3 & 0xc0) != 0x80) {
            fprintf(stderr, "invalid continuation bytes\n");
            exit(EXIT_FAILURE);
        }
        uint32_t result = ((byte0 & 0x07) << 18) | ((byte1 & 0x3f) << 12) | ((byte2 & 0x3f) << 6) | (byte3 & 0x3f);
        (*offset) += 4;
        return result;
    }

    fprintf(stderr, "failed to convert utf8 to codepoint\n");
    exit(EXIT_FAILURE);
}
