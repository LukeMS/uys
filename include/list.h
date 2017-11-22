/*****************************************************************
 *
 * list.h
 *
 * Source: "Mastering Algorithms with C" (1999, p. 56) by Kyle Loudon,
 *         published by O'Reilly & Associates.
 *
 * This source can be freely used, adapted, and redistributed in source or
 * binary form, so long as an acknowledgment appears in derived source files.
 * The citation should list that the code comes from the  book  "Mastering
 * Algorithms with C"  by Kyle Loudon,  published by O'Reilly & Associates.
 * This code is under copyright and cannot be included in any other book,
 * publication, or  educational product  without  permission  from  O'Reilly
 * & Associates.  No warranty is attached; we cannot take responsibility for
 * errors or  fitness for use.
 *
 *****************************************************************/


/*****************************************************************
 * NOTE: Set \file so that Doxygen looks for globals
 ****************************************************************/
/*************************************************************//**
 * \file list.h
 * \brief Interface for the linked list implementation.
 ****************************************************************/


#ifndef LIST_H
#define LIST_H


#include <stdlib.h>

#include <stdint.h>
void* uys_malloc_test_fn(size_t size);
#if UYS_TEST_MALLOC==1
#define uys_malloc uys_malloc_test_fn
#else
#define uys_malloc malloc
#endif
void uys_set_malloc_to_fail(int status);


/*****************************************************************
 * struct ListElmt, ListElmt
 ****************************************************************/
typedef struct ListElmt ListElmt;
/*************************************************************//**
 * \brief structure for linked list elements.
 ****************************************************************/
typedef struct ListElmt {
    void               *data;
    ListElmt           *next;
} ListElmt;


/*****************************************************************
 * struct List, List
 ****************************************************************/
typedef struct List List;
/*************************************************************//**
 * \brief structure for linked lists.
 ****************************************************************/
typedef struct List {
    int                size;
    int                (*match)(const void *key1, const void *key2);
    void               (*destroy)(void *data);
    ListElmt           *head;
    ListElmt           *tail;
} List;


/*****************************************************************
 * list_init
 ****************************************************************/
/*************************************************************//**
 * \brief Initializes the linked list specified by `list`.
 *
 * This operation must be called for a linked list before the list can be
 * used with any other operation. The `destroy` argument provides a way to
 * free dynamically allocated data when `list_destroy` is called
 * Complexity: O(1).
 *
 * \param list pointer to a `List` struct.
 * \param destroy pointer to a function to clear allocated memory for data.
 *     If the list contains data dynamically allocated using malloc, destroy
 *     should be set to `free` to free the data as the list is destroyed.
 *     For structured data containing several dynamically allocated members,
 *     destroy should be set to a user-defined function that calls free for
 *     each dynamically allocated member as well as for the structure itself.
 *     For a list containing data that should not be freed, destroy should be
 *     set to `NULL`.
 *
 * \return None.
 ****************************************************************/
void list_init(List *list, void (*destroy)(void *data));


/*****************************************************************
 * list_destroy
 ****************************************************************/
/*************************************************************//**
 * \brief Destroys the linked list specified by `list`.
 *
 * No other operations are permitted after calling list_destroy unless
 * list_init is called again. The list_destroy operation removes all elements
 * from a list and calls the function passed as `destroy` to list_init once
 * for each element as it is removed, provided destroy was not set to `NULL`.
 * Complexity: O(n), where n is the number of elements in the list.
 *
 * \param list pointer to a `List` struct.
 *
 * \return None.
 ****************************************************************/
void list_destroy(List *list);


/*****************************************************************
 * list_ins_next
 ****************************************************************/
/*************************************************************//**
 * \brief Inserts an element just after `element` on `list`.
 *
 * If `element` is `NULL`, the new element is inserted at the head of the
 * list. The new element contains a pointer to `data`, so the memory
 * referenced by `data` should remain valid as long as the element remains in
 * the list. It is the responsibility of the caller to manage the storage
 * associated with `data`.
 * Complexity: O(1).
 *
 * \param list pointer to a `List` struct.
 * \param data pointer to the data that will be referenced by the new element.
 *
 * \return 0 if inserting the element is successful, or –1 otherwise.
 ****************************************************************/
int list_ins_next(List *list, ListElmt *element, const void *data);


/*****************************************************************
 * list_rem_next
 ****************************************************************/
/*************************************************************//**
 * \brief Removes the element just after `element` on `list`.
 *
 * If `element` is `NULL`, the element at the head of the list is removed.
 * Upon return, data points to the data stored in the element that was
 * removed. It is the responsibility of the caller to manage the storage
 * associated with the data.
 * Complexity: O(1).
 *
 * \param list pointer to a `List` struct.
 * \param element pointer to the `ListElmt` after which the removal should
 *     occur.
 * \param **data pointer to pointer where the removed element's data
 *     reference will be stored.
 *
 * \return 0 if removing the element is successful, or –1 otherwise.
 ****************************************************************/
int list_rem_next(List *list, ListElmt *element, void **data);


/*****************************************************************
 * list_size
 ****************************************************************/
/*************************************************************//**
 * \brief evaluates to the number of elements on `list`.
 *
 * Complexity: O(1).
 *
 * \param list pointer to a `List` struct.
 *
 * \return Number of elements in the list.
 ****************************************************************/
#define list_size(list)((list)->size)


/*****************************************************************
 * list_head
 ****************************************************************/
/*************************************************************//**
 * \brief evaluates to the element at the head of `list`.
 *
 * Complexity: O(1).
 *
 * \param *list pointer to a `List` struct.
 *
 * \return Element at the head of the list.
 ****************************************************************/
#define list_head(list) ((list)->head)


/*****************************************************************
 * list_tail
 ****************************************************************/
/*************************************************************//**
 * \brief: evaluates to the element at the tail of `list`.
 *
 * Complexity: O(1).
 *
 * \param *list pointer to a `List` struct.
 *
 * \return Element at the tail of the list.
 ****************************************************************/
#define list_tail(list)((list)->tail)


/*****************************************************************
 * list_is_head
 ****************************************************************/
/*************************************************************//**
 * \brief determines whether `element` is at the head of `list`.
 *
 * \param *list pointer to a `List` struct.
 * \param *element pointer to the `ListElmt` to be checked.
 *
 * \return 1 if the element is at the head of the list, or 0 otherwise.
 ****************************************************************/
#define list_is_head(list, element) ((element) == (list)->head ? 1 : 0)


/*****************************************************************
 * list_is_tail
 ****************************************************************/
/*************************************************************//**
 * \brief determines whether `element` is at the tail of `list`.
 *
 * \param *list pointer to a `List` struct.
 * \param *element pointer to the `ListElmt` to be checked.
 *
 * \return 1 if the element is at the tail of the list, or 0 otherwise.
 ****************************************************************/
#define list_is_tail(element) ((element)->next == NULL ? 1 : 0)


/*****************************************************************
 * list_data
 ****************************************************************/
/*************************************************************//**
 * \brief evaluates to the data stored on `element`.
 *
 * \param *element pointer to the `ListElmt` to be checked.
 *
 * \return Data stored in the element.
 ****************************************************************/
#define list_data(element) ((element)->data)


/*****************************************************************
 * list_next
 ****************************************************************/
/*************************************************************//**
 * \brief evaluates to the element following `element`.
 *
 * \param *element pointer to a `ListElmt` struct.
 *
 * \return Element following `element`.
 ****************************************************************/
#define list_next(element) ((element)->next)


/*****************************************************************
 * list_is_empty
 ****************************************************************/
/*************************************************************//**
 * \brief determines whether `list` is empty.
 *
 * \param *list pointer to a `List` struct.
 *
 * \return 1 if the list is empty, or 0 otherwise.
 ****************************************************************/
#define list_is_empty(list) (list->size == 0 ? 1 : 0)


#endif
