/*****************************************************************
 *
 * nhash.h
 *****************************************************************/


#ifndef CHTBL_H
#define CHTBL_H
#include <stdlib.h>
#include <stdint.h>
#include "list.h"


/*************************************************************//**
 * NHash, NHash_: structure for chained hash tables.
 ****************************************************************/
typedef struct NHash_ {
    int                buckets;
    void               (*destroy)(void *data);
    int                size;
    List               *table;
} NHash;


typedef struct NHashEntry {
    uint64_t    key;
    const void *val;
} NHashEntry;



/*****************************************************************
 *
 * PUBLIC INTERFACE
 *
 *****************************************************************/


/*************************************************************//**
 * nhash_init:
 *
 * Initializes the chained hash table specified by htbl. This operation must
 * be called for a chained hash table before the hash table can be used
 * with any other operation.
 * Complexity: O(m), where m is the number of buckets in the hash table.
 *
 * \param buckets The number of buckets allocated in the hash table
 * \param *h a user-defined hash function for hashing keys
 * \param *match specifies a user-defined function to determine whether two
 *     keys match. It should return 1 if key1 is equal to key2, and 0
 *     otherwise.
 * \param *destroy provides a way to free dynamically allocated data when
 *     nhash_destroy is called. For example, if the hash table contains data
 *     dynamically allocated using malloc, destroy should be set to free to
 *     free the data as the hash table is destroyed. For structured data
 *     containing several dynamically allocated members, destroy should be
 *     set to a user-defined function that calls free for each dynamically
 *     allocated member as well as for the structure itself. For a hash table
 *     containing data that should not be freed, destroy should be set to
 *     NULL.
 *
 * \return 0 if initializing the hash table is successful, or –1 otherwise.
 ****************************************************************/
int nhash_init(NHash *htbl, int buckets,
    void (*destroy)(void*data));


/*************************************************************//**
 * nhash_destroy: Destroys the chained hash table specified by htbl.
 *
 * No other operations are permitted after calling nhash_destroy unless
 * nhash_init is called again. The nhash_destroy operation removes all
 * elements from a hash table and calls the function passed as destroy to
 * nhash_init once for each element as it is removed,
 * provided destroy was not set to NULL.
 * Complexity: O(m), where m is the number of buckets in the hash table.
 *
 * \param *htbl pointer to a `NHash` struct
 *
 * \return None
 ****************************************************************/
void nhash_destroy(NHash *htbl);


NHashEntry* nhash_get(NHash *htbl, size_t nk, ...);

/*************************************************************//**
 * nhash_insert: Inserts an element into the chained hash table `htbl`.
 *
 * The new element contains a pointer to data, so the memory referenced by
 * data should remain valid as long as the element remains in the hash table.
 * It is the responsibility of the caller to manage the storage associated
 * with data.
 * Complexity: O(1)
 *
 * \param *htbl pointer to a `NHash` struct
 * \param *data pointer to the data that will be referenced by the new element
 *
 * \return 0 if inserting the element is successful, 1 if the element is
 *     already in the hash table, or –1 otherwise.
 ****************************************************************/
int nhash_insert(NHash *htbl, const void *val, size_t nk, ...);


/*************************************************************//**
 * nhash_remove: Removes the element matching data from `htbl`.
 *
 *  Upon return, data points to the data stored in the element that
 * was removed. It is the responsibility of the caller to manage the storage
 * associated with the data.
 * Complexity: O(1)
 *
 * \param *htbl pointer to a `NHash` struct
 * \param **data pointer to the pointer where a reference to the data from
 *     the removed element will be stored.
 *
 * \return 0 if removing the element is successful, or –1 otherwise.
 ****************************************************************/
void* nhash_remove(NHash *htbl, size_t nk, ...);


void* nhash_pop(NHash *htbl);

/*************************************************************//**
 * nhash_grow: Grow the chained hash table to twice its current size.
 *
 * In practice a new table is created, every element is removed from the
 * former table and insert into the new one with a new hash fitting the
 * current size.
 * Complexity: O(n), where n is the number of elements in the hash table.
 *
 * \param *htbl pointer to a `NHash` struct
 *
 * \return 0 if the element is found in the hash table, or –1 otherwise.
 ****************************************************************/
void nhash_grow(NHash *htbl);


/*************************************************************//**
 * nhash_size: evaluates to the number of elements in `htbl`.
 *
 * Complexity: O(1)
 *
 * \param *htbl pointer to a `NHash` struct
 *
 * \return Number of elements in the hash table.
 ****************************************************************/
#define nhash_size(htbl) ((htbl)->size)


/*************************************************************//**
 * nhash_loadfactor: evaluates to the number of elements in `htbl`.
 *
 * The load factor of a hash table is defined as:
 *     α = n / m
 * where n is the number of elements in the table and m is the number of
 * positions into which elements may be hashed. The load factor of a chained
 * hash table indicates the maximum number of elements we can expect to
 * encounter in a bucket, assuming uniform hashing.
 * Performance degrades significantly if we make the number of buckets in the
 * table small relative to the number of elements we plan to insert.
 * Python's dict implementation, for example, is said to grow when it's load
 * factor reaches 2/3, to prevent performance degradation.
 * Complexity: O(1)
 *
 * \param *htbl pointer to a `NHash` struct
 *
 * \return Load factor.
 ****************************************************************/
#define nhash_loadfactor(htbl) (((float)((htbl)->size)) / ((htbl)->buckets))


#endif
