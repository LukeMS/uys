#include <assert.h>

#include "uys_alloc.h"


#define TEST_SIZE 100


int main(void) {
    /*****************************************************************
     * test both cases of UYS_MAKE_MALLOC_FAIL (defined by
     * `uys_set_malloc_to_fail`)
     ****************************************************************/
    for (int fail = 0; fail <= 1; ++fail) {
        uys_set_malloc_to_fail(fail);
        for (int i = 0; i < TEST_SIZE; ++i){
            int *int_ptr = uys_malloc(sizeof(*int_ptr));
            assert(fail ? (int_ptr == NULL) : (int_ptr != NULL));
            free(int_ptr);
        }
    }

    return 0;
}
