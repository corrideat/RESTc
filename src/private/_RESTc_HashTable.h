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

#pragma once
#ifndef _RESTC_HASHTABLE_H
#define _RESTC_HASHTABLE_H
#ifdef __cplusplus
extern "C" {
#endif
#include <stddef.h>
#include <string.h>
#include "RESTc_HashTable.h"
#include "_RESTc_Class.h"

    /* TODO: figure out a type for query_parameters, body_parameters. Ideally, we should use a hash table or something similar, but let's find third party implementations */
    struct RESTc_HashTable {
        void const* class;
        int (*comparator_function)(void *, void *);
        size_t n_buckets;
        void** buckets;
        size_t n_elements;
    };

#ifdef __cplusplus
}
#endif
#endif
