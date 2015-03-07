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
#include <stdarg.h>
#include <stdlib.h>
#include "private/_RESTc_Class.h"

void const* (RESTc_typeof)(void const *self)
{
    if(NULL == self) {
        return NULL;
    }
    struct RESTc_Class const * const * const cp = self;
    return (*cp);
}

void* (RESTc_new)(void const * const _class, ...)
{
    struct RESTc_Class const * const class = *((struct RESTc_Class const * const *)_class);
    if(true == class->abstract) {
        return NULL;
    }
    void * const pc = malloc(class->size);
    void *p = pc;
    if(NULL != pc) {
        * (void const **) pc = _class;
        if(NULL != class->constructor) {
            va_list ap;
            va_start(ap, _class);
            p = class->constructor(pc, &ap);
            va_end(ap);
            if(NULL == p) {
                free(pc);
                return NULL;
            }
        }
    }
    return pc;
}

void (RESTc_delete)(void * self)
{
    struct RESTc_Class const * const * const * const cp = self;
    if(NULL != self && NULL != *cp && NULL != **cp && NULL != (**cp)->destructor) {
        self = (**cp)->destructor(self);
    }
    free(self);
}

void* (RESTc_clone)(void const *self)
{
    struct RESTc_Class const * const * const * const cp = self;
    if(NULL != self && NULL != *cp && NULL != **cp && NULL != (**cp)->clone) {
        return (**cp)->clone(self);
    }
    return NULL;
}

int (RESTc_differ)(void const * self, void const * b)
{
    struct RESTc_Class const * const * const * cp = self;
    assert(NULL != self && NULL != *cp && NULL != **cp);
    do {
        if(NULL == (**cp)->differ) {
            cp = (struct RESTc_Class const * const * const *) & ((**cp)->super);
        } else {
            break;
        }
    } while(NULL != (**cp)->super);
    assert(NULL != (**cp)->differ);
    return (**cp)->differ(self, b);
}

unsigned long(RESTc_hash)(void const *self)
{
    struct RESTc_Class const * const * const * cp = self;
    assert(NULL != self && NULL != *cp);
    do {
        if(NULL == (**cp)->hash) {
            cp = (struct RESTc_Class const * const * const *) & ((**cp)->super);
        } else {
            break;
        }
    } while(NULL != (**cp)->super);
    assert(NULL != (**cp)->hash);
    return (**cp)->hash(self);
}
