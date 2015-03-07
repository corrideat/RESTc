/*
 Copyright (c) 2015 Ricardo Iván Vieitez Parra

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
*/

#include <assert.h>
#include <inttypes.h>
#include <stdarg.h>
#include <stdlib.h>
#include "bool.h"
#include "debug.h"
#include "private/_RESTc_HashTable.h"

static void* constructor(void * const _self, va_list * const app);
static void* destructor(void * const _self);
static void const * * getAll(void const * const _self);
static size_t getCount(void const * const _self);
static void insert_helper(void * const element, void ** const buckets, size_t const n_buckets);
static void const* find(void * const, void const * const key);
static bool insert(void *, void const * element);
static bool insertReplace(void *, void const * element);
static bool ht_remove(void *, void const * element);

static struct RESTc_Class const _RESTc_HashTable = {
    sizeof(struct RESTc_HashTable),
    false,
    NULL,
    constructor,
    destructor,
    NULL,
    NULL,
    NULL
};

struct PR_RESTc_HashTable const RESTc_HashTable = {
    &_RESTc_HashTable,
    getAll,
    getCount,
    find,
    insert,
    insertReplace,
    ht_remove
};

static void* constructor(void * const _self, va_list * const app)
{
    struct RESTc_HashTable * const self = _self;

    size_t n_buckets = va_arg(*app, size_t);
    n_buckets = (0 < n_buckets) ? n_buckets : RESTC_HASHTABLE_DEFAULT_BUCKET_SIZE;

    void** const buckets = malloc(sizeof(void*)*n_buckets);
    if(NULL == buckets) {
        return NULL;
    }

    size_t i;
    for(i = 0; i < n_buckets; i++) {
        buckets[i] = NULL;
    }

    self->n_buckets = n_buckets;
    self->buckets = buckets;
    self->n_elements = 0;
    return self;
}

static void* destructor(void * const _self)
{
    struct RESTc_HashTable * const self = _self;
    size_t i;
    for(i = 0; i < self->n_buckets; i++) {
        RESTc_delete(self->buckets[i]);
    }
    free(self->buckets);
    return self;
}

static void const * * getAll(void const * const _self)
{
    struct RESTc_HashTable const * const self = _self;
    size_t const n_elements = self->n_elements;
    size_t const n_buckets = self->n_buckets;
    void const * * buckets = malloc(sizeof(void const *) * (n_elements + 1));
    if(NULL != buckets) {
        size_t i, j = 0;
        for(i = 0; i < n_buckets; i++) {
            if(NULL != self->buckets[i]) {
                buckets[j++] = self->buckets[i];
            }
        }
        buckets[j] = NULL;
    }
    return buckets;
}

static size_t getCount(void const * const _self)
{
    struct RESTc_HashTable const * const self = _self;
    return self->n_elements;
}

void const* find(void * const _self, void const * const key)
{
    struct RESTc_HashTable * const self = _self;
    size_t const chash = RESTc_hash(key) % self->n_buckets;
    size_t pos = chash;
    char passes = 0;
    for(; (chash != pos || 0 == passes); pos++) {
        if(pos == self->n_buckets) {
            pos -= self->n_buckets;
            passes++;
        }
        if(NULL == self->buckets[pos]) {
            break;
        } else if(0 == RESTc_differ(self->buckets[pos], key)) {
            return self->buckets[pos];
        }
    }
    return NULL;
}

/* There must be at least one free bucket or else it will loop forever */
static void insert_helper(void * const element, void ** const buckets, size_t const n_buckets)
{
    size_t hash = RESTc_hash(element) % n_buckets;
    bool successful = false;
    for(; successful == false; hash++) {
        if(hash == n_buckets) {
            hash -= n_buckets;
        }
        if(NULL == buckets[hash]) {
            buckets[hash] = element;
            successful = true;
        }
    }
}

bool insert(void * const _self, void const * const element)
{
    struct RESTc_HashTable * const self = _self;
    if(self->n_elements > (self->n_buckets * RESTC_HASHTABLE_FILL_TRESHHOLD)) {
        size_t i;
        size_t const new_n_buckets = 2 * self->n_buckets;
        void ** const new_buckets = malloc(sizeof(void*)*new_n_buckets);
        if(NULL == new_buckets) {
            return false;
        }
        for(i = 0; i < new_n_buckets; i++) {
            new_buckets[i] = NULL;
        }
        for(i = 0; i < self->n_buckets; i++) {
            if(NULL != self->buckets[i]) {
                insert_helper(self->buckets[i], new_buckets, new_n_buckets);
            }
        }
        self->buckets = new_buckets;
        self->n_buckets = new_n_buckets;
    }
    void * const local_element = RESTc_clone(element);
    if(NULL == local_element) {
        return false;
    }
    insert_helper(local_element, self->buckets, self->n_buckets);
    self->n_elements++;
    return true;
}

bool insertReplace(void * const _self, void const * const element)
{
    struct RESTc_HashTable * const self = _self;
    size_t const chash = RESTc_hash(element) % self->n_buckets;
    size_t pos = chash;
    char passes = 0;
    bool successful = false;
    for(; successful == false && (chash != pos || 0 == passes); pos++) {
        if(pos == self->n_buckets) {
            pos -= self->n_buckets;
            passes++;
        }
        if(NULL == self->buckets[pos]) {
            break;
        } else if(0 == RESTc_differ(self->buckets[pos], element)) {
            void * const local_element = RESTc_clone(element);
            if(NULL != local_element) {
                RESTc_delete(self->buckets[pos]);
                successful = true;
                self->buckets[pos] = local_element;
            } else {
                return false;
            }
            break;
        }
    }
    if(true == successful) {
        return true;
    }
    return insert(_self, element);
}

bool ht_remove(void * const _self, void const * const key)
{
    struct RESTc_HashTable * const self = _self;
    size_t const n_buckets = self->n_buckets;
    size_t const chash = RESTc_hash(key) % n_buckets;
    size_t pos = chash;
    char passes = 0;
    bool ret = false;
    for(; (chash != pos || 0 == passes); pos++) {
        if(pos == n_buckets) {
            pos -= n_buckets;
            passes++;
        }
        if(NULL == self->buckets[pos]) {
            break;
        } else if(0 == RESTc_differ(self->buckets[pos], key)) {
            RESTc_delete(self->buckets[pos]);
            self->buckets[pos] = NULL;
            self->n_elements--;
            size_t const rem_pos = pos;
            size_t ppos = rem_pos;
            passes = 0;
            ret = true;
            for(pos++; ; ppos = pos++) {
                if(pos == n_buckets) {
                    pos -= n_buckets;
                }
                if(NULL != self->buckets[pos] && RESTc_hash(self->buckets[pos]) % n_buckets != pos) {
                    self->buckets[ppos] = self->buckets[pos];
                } else {
                    self->buckets[ppos] = NULL;
                    break;
                }
            }
            break;
        }
    }
    return ret;
}
