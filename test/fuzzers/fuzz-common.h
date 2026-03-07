
#ifndef FUZZ_COMMON_H
#define FUZZ_COMMON_H

#include <stddef.h>
#include <stdint.h>

/* Validate that data is well-formed UTF-8 without null bytes.
 * Returns 1 if valid, 0 otherwise.
 * Rejects null bytes, surrogates (U+D800..U+DFFF), and overlong encodings. */
static int
is_valid_utf8(const uint8_t* data, size_t size)
{
    size_t i = 0;
    while(i < size) {
        if(data[i] == 0x00) {
            return 0;  /* null byte (not possible from JS strings) */
        } else if(data[i] < 0x80) {
            i++;
        } else if((data[i] & 0xE0) == 0xC0) {
            if(data[i] < 0xC2)
                return 0;  /* overlong */
            if(i + 1 >= size || (data[i+1] & 0xC0) != 0x80)
                return 0;
            i += 2;
        } else if((data[i] & 0xF0) == 0xE0) {
            if(i + 2 >= size || (data[i+1] & 0xC0) != 0x80 || (data[i+2] & 0xC0) != 0x80)
                return 0;
            /* reject surrogates U+D800..U+DFFF */
            if(data[i] == 0xED && data[i+1] >= 0xA0)
                return 0;
            /* reject overlong */
            if(data[i] == 0xE0 && data[i+1] < 0xA0)
                return 0;
            i += 3;
        } else if((data[i] & 0xF8) == 0xF0) {
            if(i + 3 >= size || (data[i+1] & 0xC0) != 0x80
                             || (data[i+2] & 0xC0) != 0x80
                             || (data[i+3] & 0xC0) != 0x80)
                return 0;
            /* reject overlong */
            if(data[i] == 0xF0 && data[i+1] < 0x90)
                return 0;
            /* reject > U+10FFFF */
            if(data[i] > 0xF4 || (data[i] == 0xF4 && data[i+1] > 0x8F))
                return 0;
            i += 4;
        } else {
            return 0;
        }
    }
    return 1;
}

#endif /* FUZZ_COMMON_H */
