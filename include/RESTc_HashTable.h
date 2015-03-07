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
#ifndef RESTc_HASHTABLE_H
#define RESTc_HASHTABLE_H
#include <stddef.h>
#include "RESTc_Class.h"
#include "bool.h"
#ifdef __cplusplus
extern "C" {
#endif

    extern struct PR_RESTc_HashTable {
        void const * const _;
        void const ** (* const getAll)(void const *);
        size_t (* const getCount)(void const *);
        void const* (* const find)(void * const, void const * const key);
        bool (* const insert)(void *, void const * element);
        bool (* const insertReplace)(void *, void const * element);
        bool (* const remove)(void *, void const * element);
    } const RESTc_HashTable;

#ifndef RESTC_HASHTABLE_DEFAULT_BUCKET_SIZE
#define RESTC_HASHTABLE_DEFAULT_BUCKET_SIZE 35
#endif

#ifndef RESTC_HASHTABLE_FILL_TRESHHOLD
#define RESTC_HASHTABLE_FILL_TRESHHOLD 0.80F
#endif

#ifdef __cplusplus
}
#endif
#endif
