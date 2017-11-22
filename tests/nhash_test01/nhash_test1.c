#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "list.h"
#include "nhash.h"
#include "words_alpha.h"

#define WORDS 100
#define VERBOSE 1
#if VERBOSE >= 1
/*************************************************************
* inttypes.h should provide extended integer type specifiers for use in
* printf and scanf function calls...
*************************************************************/
#include <inttypes.h>
#endif


int main(void) {
    NHash dict;
    ListElmt *element;
    List words;

    nhash_init(&dict, 370101, free);
    list_init(&words, free);
    words_alpha(&words, WORDS);

    puts("adding");
    element = list_head(&words);
    for (int i = 0; i < WORDS; i++, element=list_next(element)) {
#if VERBOSE >= 1
        char *tmp = element->data;
        printf("word %d: \"%s\" (size %d)\n", i, tmp, strlen(tmp));
        /*************************************************************
        * Supposedly a false warning should appear:
        *   warning: too many arguments for format [-Wformat-extra-args]
        * Nothing can be done about that at the time...
        *************************************************************/
        printf("%llu\n", (long long unsigned) i);
#endif
        nhash_insert(&dict, element->data, 2,
                     element->data, strlen(element->data));
    }


    puts("getting");
    element = list_head(&words);
    for (int i = 0; i < WORDS; i++, element=list_next(element)) {
        char *tmp = nhash_remove(&dict, 2,
                                 element->data, strlen(element->data));
        assert(strcmp(tmp, element->data) == 0);
#if VERBOSE >= 1
        printf("tmp %s == element->data %s\n", (char*)tmp,
                                               (char*)element->data);
#endif
    }

    list_destroy(&words);
    nhash_destroy(&dict);

    return 0;
}
