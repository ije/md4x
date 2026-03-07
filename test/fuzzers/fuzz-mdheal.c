
#include <stdint.h>
#include <stdlib.h>
#include "md4x-heal.h"
#include "fuzz-common.h"


static void
process_output(const char* text, unsigned size, void* userdata)
{
   return;
}

int
LLVMFuzzerTestOneInput(const uint8_t *data, size_t size)
{
    if(size == 0 || !is_valid_utf8(data, size))
        return -1;

    md_heal((const char*)data, (unsigned)size, process_output, NULL);
    return 0;
}
