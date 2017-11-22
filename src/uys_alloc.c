#include <stdint.h>
#include <stdlib.h>

#include "uys_alloc.h"


int UYS_MAKE_MALLOC_FAIL = 0;
void* uys_malloc_test_fn(size_t size) {
    if (UYS_MAKE_MALLOC_FAIL)
        return NULL;
    return malloc(size);
}

void uys_set_malloc_to_fail(int status) {
    UYS_MAKE_MALLOC_FAIL = status;
}
