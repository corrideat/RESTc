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
#include <string.h>
#include "bool.h"
#include "private/_RESTc_String.h"
#include "private/_RESTc_StringConst.h"

static void* constructor(void * const _self, va_list * const app);
static void* clone(void const * const _self);

static char const * get(void const * const _self);
static size_t length(void const * const _self);
static bool set(void * const _self, char const * const string, size_t const length);
static bool setter(void * const _self, char const * const string, size_t const length);

static struct RESTc_Class const _RESTc_StringConst = {
    sizeof(struct RESTc_StringConst),
    false,
    &RESTc_String,
    constructor,
    NULL,
    clone,
    NULL,
    NULL
};

struct PR_RESTc_StringConst const RESTc_StringConst = {
    {
        &_RESTc_StringConst,
        get,
        length,
        OVERRIDE,
        set
    }
};

static void* constructor(void * const _self, va_list * const app)
{
    struct RESTc_String * const self = _self;
    char const * const string = va_arg(*app, char const*);
    if(NULL == string) {
        self->string = NULL;
        self->length = 0;
    } else {
        size_t const length = va_arg(*app, size_t);
        if(false == setter(_self, string, length)) {
            return NULL;
        }
    }
    return self;
}

static void* clone(void const * const _self)
{
    struct RESTc_String const * const self = _self;
    return RESTc_new(RESTc_StringConst, self->string, self->length);
}

static char const * get(void const * const _self)
{
    return RESTc_String.get(_self);
}

static size_t length(void const * const _self)
{
    return RESTc_String.length(_self);
}

static bool set(void * const _self, char const * const string, size_t const length)
{
    void const * const * const PR_self = _self;
    assert(*PR_self == &RESTc_StringConst);
    return setter(_self, string, length);
}

static bool setter(void * const _self, char const * const string, size_t length)
{
    struct RESTc_String * const self = _self;
    if(NULL == string) {
        self->string = NULL;
        self->length = 0;
    } else {
        if(0 == length && 0 == (length = strlen(string))) {
            self->string = NULL;
            self->length = 0;
        } else {
            self->string = string;
            self->length = length;
        }
    }
    return true;
}
