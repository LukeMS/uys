/*****************************************************************
 *
 * nhash.c
 *****************************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdint.h>

#include "lookup3.h"
#include "list.h"
#include "nhash.h"


/*****************************************************************
 * nhash_init.
 ****************************************************************/
int nhash_init(NHash *htbl, int buckets, void (*destroy)(void*data)) {
    int                i;

    /*****************************************************************
     * Allocate space for the hash table.
     ****************************************************************/
    if ((htbl->table = (List *)malloc(buckets * sizeof(List))) == NULL) {
        return -1;
    }

    /*****************************************************************
     * Encapsulate the destroy function.
     ****************************************************************/
    htbl->destroy = destroy;

    htbl->buckets = buckets;

   /*****************************************************************
    * Initialize the buckets.
    ****************************************************************/
    for (i = 0; i < htbl->buckets; i++) {
       list_init(&htbl->table[i], htbl->destroy);
    }

    /*****************************************************************
     * Initialize the number of elements in the table.
     ****************************************************************/
    htbl->size = 0;

    return 0;
}


/*****************************************************************
 * nhash_destroy
 ****************************************************************/
void nhash_destroy(NHash *htbl) {
    int i;

    /*****************************************************************
     * Destroy each bucket.
     ****************************************************************/
    for (i = 0; i < htbl->buckets; i++) {
       list_destroy(&htbl->table[i]);
    }

    /*****************************************************************
     * Free the storage allocated for the hash table.
     ****************************************************************/
    free(htbl->table);

    /*****************************************************************
     * No operations are allowed now, but clear the structure as a precaution.
     ****************************************************************/
    memset(htbl, 0, sizeof(NHash));

    return;
}

void nhash_vhash(size_t nk, va_list args, uint32_t *h1, uint32_t *h2) {
    for (size_t i = 0; i < nk / 2; ++i) {
        void* data = va_arg(args, void*);
        size_t len = va_arg(args, size_t);
        hashlittle2(data, len, h1, h2);
    }

    return;
}


NHashEntry* nhash_create_entry(uint64_t key, const void *val) {
    NHashEntry *entry = malloc(sizeof(*entry));
    entry->key = key;
    entry->val = val;

    return entry;
}


/*****************************************************************
 * nhash_vget
 ****************************************************************/
NHashEntry* nhash_vget(NHash *htbl, size_t nk, va_list args) {
    NHashEntry         *entry;
    ListElmt           *element;
    int                bucket;
    uint32_t           h1,
                       h2;

    /*****************************************************************
     * Hash the 32 bit keys.
     ****************************************************************/
    h1 = 0;
    h2 = 0;
    nhash_vhash(nk, args, &h1, &h2);

    /*****************************************************************
     * Hash the 64 bit key.
     ****************************************************************/
    uint64_t key = h1 + (((uint64_t)h2)<<32);

    /*****************************************************************
     * Find the bucked
     ****************************************************************/
    bucket = key % htbl->buckets;


    /*****************************************************************
     * Search for the data in the bucket.
     ****************************************************************/
    for (element = list_head(&htbl->table[bucket]); element != NULL;
         element = list_next(element)) {

        entry = list_data(element);
        if (entry->key == key) {
            return entry;
        }
    }

    return NULL;
}

/*****************************************************************
 * nhash_get
 ****************************************************************/
NHashEntry* nhash_get(NHash *htbl, size_t nk, ...) {
    if (nk % 2)
        exit(EXIT_FAILURE);

    va_list args;  // Declare va_list type variable
    va_start(args, nk);  // Initialise va_list after last regular variable
    NHashEntry *retval = nhash_vget(htbl, nk, args);
    va_end(args);  //Close the va_list

    return retval;
}


/*****************************************************************
 * nhash_vinsert
 ****************************************************************/
int nhash_vinsert(NHash *htbl, const void *val, size_t nk, va_list args) {
    NHashEntry         *entry;
    ListElmt           *element;
    int                bucket,
                       retval;
    uint32_t           h1,
                       h2;

    /*****************************************************************
     * Hash the 32 bit keys.
     ****************************************************************/
    h1 = 0;
    h2 = 0;
    nhash_vhash(nk, args, &h1, &h2);

    /*****************************************************************
     * Hash the 64 bit key.
     ****************************************************************/
    uint64_t key = h1 + (((uint64_t)h2)<<32);

    /*****************************************************************
     * Find the bucked
     ****************************************************************/
    bucket = key % htbl->buckets;


    /*****************************************************************
     * Search for the data in the bucket.
     ****************************************************************/
    for (element = list_head(&htbl->table[bucket]); element != NULL;
         element = list_next(element)) {

        entry = list_data(element);
        if (entry->key == key) {
          /*****************************************************************
           * Update `val` if the `key` is already in the table.
           ****************************************************************/
            entry->val = val;
            return 0;
        }
    }

    /*****************************************************************
     * Create new entry
     ****************************************************************/
    entry = nhash_create_entry(key, val);


    /*****************************************************************
     * Insert the entry into the bucket.
     ****************************************************************/
    if ((retval = list_ins_next(&htbl->table[bucket], NULL, entry)) == 0) {
        htbl->size++;
        if (nhash_loadfactor(htbl) > 2) {
            nhash_grow(htbl);
        }
    }

    return retval;
}


/*****************************************************************
 * nhash_insert
 ****************************************************************/
int nhash_insert(NHash *htbl, const void *val, size_t nk, ...) {
    if (nk % 2)
        exit(EXIT_FAILURE);

    va_list args;  // Declare va_list type variable
    va_start(args, nk);  // Initialise va_list after last regular variable
    int retval = nhash_vinsert(htbl, val, nk, args);
    va_end(args);  //Close the va_list


    return retval;
}


/*****************************************************************
 * nhash_vremove
 ****************************************************************/
void* nhash_vremove(NHash *htbl, size_t nk, va_list args) {
    void               *temp,
                       *val;
    ListElmt           *element,
                       *prev;
    int                bucket;
    uint32_t           h1,
                       h2;

    /*****************************************************************
     * Hash the 32 bit keys.
     ****************************************************************/
    h1 = 0;
    h2 = 0;
    nhash_vhash(nk, args, &h1, &h2);

    /*****************************************************************
     * Hash the 64 bit key.
     ****************************************************************/
    uint64_t key = h1 + (((uint64_t)h2)<<32);

    /*****************************************************************
     * Find the bucked
     ****************************************************************/
    bucket = key % htbl->buckets;

    /*****************************************************************
     * Search for the data in the bucket.
     ****************************************************************/
    prev = NULL;
    for (element = list_head(&htbl->table[bucket]); element != NULL;
            element = list_next(element)) {
        NHashEntry *entry = list_data(element);
        if (entry->key == key) {

            /*************************************************************
            * Remove the data from the bucket.
            *************************************************************/
            if (list_rem_next(&htbl->table[bucket], prev,
                              &temp) == 0) {
                val = (void*)entry->val;
                free(temp);
                htbl->size--;
                return val;
            }
            else {
                return NULL;
            }
        }
       prev = element;
    }

    /*****************************************************************
     * Return that the data was not found.
     ****************************************************************/
    return NULL;
}


/*****************************************************************
 * nhash_remove
 ****************************************************************/
void* nhash_remove(NHash *htbl, size_t nk, ...) {
    if (nk % 2) {
        exit(EXIT_FAILURE);
    }

    void* data;

    va_list args;  // Declare va_list type variable
    va_start(args, nk);  // Initialise va_list after last regular variable
    data = nhash_vremove(htbl, nk, args);
    va_end(args);  //Close the va_list

    return data;
}


void* nhash_pop(NHash *htbl) {
    List *bucket;
    ListElmt *element;

    for (int i = 0; i < htbl->buckets; i++) {
        bucket = &htbl->table[i];
        element = list_head(bucket);
        if (element != NULL) {
            NHashEntry *entry = element->data;
            return (void*)entry->val;
        }
    }
    return NULL;
}

/*****************************************************************
 * nhash_grow
 ****************************************************************/
void nhash_grow(NHash *htbl) {
    printf("nhash_grow %d -> %d (size %d, load factor %.2f)...",
           htbl->buckets, htbl->buckets * 2, htbl->size,
           nhash_loadfactor(htbl));

    size_t old_size = htbl->buckets;
    List  *old_table = htbl->table;
    htbl->buckets *= 2;

    htbl->table = calloc(htbl->buckets, sizeof(List*));

   /*****************************************************************
    * Move the buckets.
    ****************************************************************/
    for (size_t i = 0; i < old_size; i++) {
        List *bucket = &old_table[i];
        if (!list_is_empty(bucket)) {
            ListElmt *head = list_head(bucket);
            NHashEntry *entry = head->data;
            int bucket_index = entry->key % htbl->buckets;
            htbl->table[bucket_index] = *bucket;
        }
    }
    free(old_table);

   /*****************************************************************
    * Initialize the remaining buckets.
    ****************************************************************/
    for (int i = 0; i < htbl->buckets; i++) {
        if (&htbl->table[i] != NULL) {
            list_init(&htbl->table[i], htbl->destroy);
        }
    }

    return;
}
