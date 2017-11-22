#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

#include "uys_alloc.h"
#include "list.h"


#define TEST_SIZE 100

typedef struct ListData {
    int val;
} ListData;

int main(void) {
    List *list;
    list = malloc(sizeof(*list));
    assert(list != NULL);


    list_init(list, free);
    for (int i = 0; i < TEST_SIZE; ++i) {
        ListData *data = malloc(sizeof(*data));
        data->val = i;
        /*****************************************************************
         * insertion at tail
         ****************************************************************/
        assert(list_ins_next(list, list_tail(list), data) == 0);
        assert(list_size(list) == i + 1);
        assert(((ListData*)(( list_tail(list))->data))->val == i);
    }
    /*****************************************************************
     * iterate i increasingly
     ****************************************************************/
    for (int i = 0; i < TEST_SIZE; ++i) {
        ListData *data = NULL;
        /*****************************************************************
         * removal at head
         ****************************************************************/
        assert(list_rem_next(list, NULL, (void**)&data) == 0);
        assert(data->val == i);
        free(data);
    }
    list_destroy(list);
    free(list);

    list = malloc(sizeof(*list));
    list_init(list, free);
    for (int i = 0; i < TEST_SIZE; ++i) {
        ListData *data = malloc(sizeof(*data));
        data->val = i;
        /*****************************************************************
         * insertion at head
         ****************************************************************/
        assert(list_ins_next(list, NULL, data) == 0);
        assert(list_size(list) == i + 1);
        assert(((ListData*)(( list_head(list))->data))->val == i);
    }
    /*****************************************************************
     * iterate i decreasingly
     ****************************************************************/
    for (int i = TEST_SIZE - 1; i >= 0; --i) {
        ListData *data = NULL;
        /*****************************************************************
         * removal at head
         ****************************************************************/
        assert(list_rem_next(list, NULL, (void**)&data) == 0);
        assert(data->val == i);
        free(data);
    }
    list_destroy(list);
    free(list);


    for (int i = 0; i < TEST_SIZE * 2; ++i) {
        list = malloc(sizeof(*list));
        list_init(list, free);
        ListData *data = malloc(sizeof(*data));

        /*****************************************************************
         * test (failure): removal on empty list
         ****************************************************************/
        assert(list_rem_next(list, NULL, (void**)&data) == -1);
        assert(data);
        data->val = 9;
        /*****************************************************************
         * test: malloc failure on ListElmt allocation
         ****************************************************************/
        uys_set_malloc_to_fail(1);
        assert(list_ins_next(list, NULL, data) != 0);
        /*****************************************************************
         * test: malloc success on ListElmt allocation
         ****************************************************************/
        uys_set_malloc_to_fail(0);
        assert(list_ins_next(list, NULL, data) == 0);

        /*****************************************************************
         * test (failure): list_rem_next on tail
         ****************************************************************/
        assert(list_rem_next(list, list_tail(list), (void**)&data) == -1);

        /*****************************************************************
         * test: removal from middle of list
         ****************************************************************/
        ListData *datb = malloc(sizeof(*datb));
        datb->val = 7;
        assert(list_ins_next(list, list_tail(list), datb) == 0);
        ListData *datc = malloc(sizeof(*datc));
        datc->val = 8;
        assert(list_ins_next(list, list_tail(list), datc) == 0);
        ListData *tmp;
        ListElmt *second = list_next(list_head(list));
        assert(list_rem_next(list, second, (void**)&tmp) == 0);
        assert(tmp->val == datc->val);
        free(datc);
        /*****************************************************************
         * test: destruction of non-empty list
         ****************************************************************/
        list_destroy(list);
        data = NULL;
        free(list);

        /*****************************************************************
         * test: list->destroy == NULL
         ****************************************************************/
        list = malloc(sizeof(*list));
        list_init(list, NULL);
        data = malloc(sizeof(*data));
        assert(list_ins_next(list, NULL, data) == 0);
        list_destroy(list);
        free(list);
        free(data);
    }
}
