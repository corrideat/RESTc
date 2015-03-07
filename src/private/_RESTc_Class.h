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
#ifndef _RESTC_CLASS_H
#define _RESTC_CLASS_H
#ifdef __cplusplus
extern "C" {
#endif
#include <stdarg.h>
#include <stddef.h>
#include "bool.h"
#include "RESTc_Class.h"

    struct RESTc_Class {
        size_t size;
        bool abstract;
        void const * super;
        void * (*constructor)(void *, va_list *);
        void * (*destructor)(void *);
        void * (*clone)(void const*);
        int (*differ)(void const*, void const*);
        unsigned long(*hash)(void const*);
    };
#ifdef __cplusplus
}
#endif
#endif

