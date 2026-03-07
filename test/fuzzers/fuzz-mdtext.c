
#include <stdint.h>
#include <stdlib.h>
#include "md4x-text.h"


static void
process_output(const MD_CHAR* text, MD_SIZE size, void* userdata)
{
   return;
}

int
LLVMFuzzerTestOneInput(const uint8_t *data, size_t size)
{
    if(size == 0)
        return 0;

    md_text((const MD_CHAR*)data, size, process_output, NULL, MD_DIALECT_ALL, 0);
    return 0;
}
