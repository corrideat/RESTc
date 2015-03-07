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
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <string.h>
#include "private/_RESTc_String.h"

static void* constructor(void * const _self, va_list * const app);
static void* destructor(void * const _self);
static void* clone(void const * const _self);
static int differ(void const * const _self, void const * const b);
static uint64_t hash(void const * const _self);

static char const * get(void const * const _self);
static size_t length(void const * const _self);
static bool set(void * const _self, char const * const string, size_t length);
static bool setter(void * const _self, char const * const string, size_t length);

static struct RESTc_Class const _RESTc_String = {
    sizeof(struct RESTc_String),
    false,
    NULL,
    constructor,
    destructor,
    clone,
    differ,
    hash
};

struct PR_RESTc_String const RESTc_String = {
    &_RESTc_String,
    get,
    length,
    SUPER,
    set
};

static void* constructor(void * const _self, va_list * const app)
{
    struct RESTc_String * const self = _self;
    char const * const string = va_arg(*app, char const*);
    self->string = NULL;
    if(NULL != string) {
        size_t const length = va_arg(*app, size_t);
        if(false == setter(_self, string, length)) {
            return NULL;
        }
    } else {
        self->string = NULL;
        self->length = 0;
    }
    return self;
}

static void* destructor(void * const _self)
{
    struct RESTc_String * const self = _self;
    free(((void*)(self->string)));
    return self;
}

static void* clone(void const * const _self)
{
    struct RESTc_String const * const self = _self;
    return RESTc_new(RESTc_String, self->string, self->length);
}

static int differ(void const * const _self, void const * const _b)
{
    struct RESTc_String const * const self = _self;
    struct RESTc_String const * const b = _b;
    if(NULL == self->string || NULL == b->string) {
        if(NULL == self->string && NULL == b->string) {
            return 0;
        } else if(NULL == self->string) {
            return -1;
        } else {
            return 1;
        }
    }
    return strncmp(self->string, b->string, self->length);
}

static uint64_t hash(void const * const _self)
{
    struct RESTc_String const * const self = _self;
    size_t const length = self->length;
    char const * const string = self->string;

    uint64_t a = 0x7FFFFFFFL;
    uint64_t const b = 0xFFFFFFFBL;
    uint64_t hash = 0x3B9AC9FFL;

    if(NULL != string) {
        size_t i;
        for(i = 0; i < length; i++) {
            hash *= a;
            hash += ((unsigned long)string[i]) ^ (a >> 2L);
            a *= b;
            a += i;
        }
    }

    return hash;
}

static bool setter(void * const _self, char const * const string, size_t length)
{
    struct RESTc_String * const self = _self;
    if(NULL != string) {
        if(0 == length) {
            length = strlen(string);
        }
        if(0 != length) {
            char * const new_string = malloc(sizeof(char) * (length + 1));
            if(NULL == new_string) {
                return false;
            } else {
                memcpy(new_string, string, sizeof(char)*length);
                new_string[length] = '\0';
                free(((void*)(self->string)));
                self->string = new_string;
                self->length = length;
                return true;
            }
        }
    }
    free(((void*)(self->string)));
    self->string = NULL;
    self->length = 0;
    return true;
}

static char const * get(void const * const _self)
{
    struct RESTc_String const * const self = _self;
    return self->string;
}

static size_t length(void const * const _self)
{
    struct RESTc_String const * const self = _self;
    return self->length;
}

static bool set(void * const _self, char const * const string, size_t length)
{
    struct PR_RESTc_String const * const * const PR_self = _self;
    bool ret;
    if(SUPER == (*PR_self)->_super_set || EXTEND == (*PR_self)->_super_set) {
        ret = setter(_self, string, length);
    } else {
        ret = true;
    }
    if(ret == true && (OVERRIDE == (*PR_self)->_super_set || EXTEND == (*PR_self)->_super_set)) {
        assert(NULL != (*PR_self)->set);
        ret = (*PR_self)->set(_self, string, length);
    }
    return ret;
}
