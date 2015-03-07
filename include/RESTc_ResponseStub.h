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
#ifndef RESTc_RESPONSESTUB_H
#define RESTc_RESPONSESTUB_H
#include <stddef.h>
#include "bool.h"
#ifdef __cplusplus
extern "C" {
#endif
    extern struct PR_RESTc_ResponseStub {
        void const * const _;
        unsigned char(* const getResponseCode)(void const *);
        void (* const setResponseCode)(void *, unsigned char);
        void const ** (* const getHeaders)(void const *);
        size_t (* const getHeadersCount)(void const *);
        bool (* const addHeader)(void *, void const * bucket);
        bool (* const addReplaceHeader)(void *, void const * bucket);
        bool (* const removeHeader)(void *, void const * key);
        void const * (* const getBody)(void const *);
        bool (* const setBody)(void *, void const * body);
    } const RESTc_ResponseStub;

#ifdef __cplusplus
}
#endif
#endif
