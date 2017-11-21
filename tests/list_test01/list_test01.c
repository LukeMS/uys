#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

#include "list.h"

#define TEST_SIZE 32000

typedef struct ListData {
    int val;
} ListData;

int main(void) {
    List *list;

    list = malloc(sizeof(*list));
    list_init(list, free);
    for (int i = 0; i < TEST_SIZE; ++i) {
        ListData *data = malloc(sizeof(*data));
        data->val = i;
        /*****************************************************************
         * insertion at tail
         ****************************************************************/
        if (list_ins_next(list, list_tail(list), data)) {
            exit(EXIT_FAILURE);
        }
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
        if (list_rem_next(list, NULL, (void**)&data)) {
            exit(EXIT_FAILURE);
        }
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
        if (list_ins_next(list, NULL, data)) {
            exit(EXIT_FAILURE);
        }
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
        if (list_rem_next(list, NULL, (void**)&data)) {
            exit(EXIT_FAILURE);
        }
        assert(data->val == i);
        free(data);
    }
    list_destroy(list);
    free(list);

    return 0;
}
