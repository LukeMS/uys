#ifndef UYS_ALLOC_H
#define UYS_ALLOC_H

#include <stdint.h>
#include <stdlib.h>

#if UYS_TEST_MALLOC==1
#define uys_malloc uys_malloc_test_fn
#else
#define uys_malloc malloc
#endif  // #if UYS_TEST_MALLOC==1




void* uys_malloc_test_fn(size_t size);

void uys_set_malloc_to_fail(int status);


#endif  // #ifndef UYS_ALLOC_H
