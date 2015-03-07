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

#include <stdarg.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "bool.h"
#include "debug.h"
#include "RESTc_String.h"
#include "private/_RESTc_Class.h"
#include "private/_RESTc_StringBucket.h"

static void* constructor(void * const _self, va_list * const app);
static void* destructor(void * const _self);
static void* clone(void const * const _self);
static char const * get(void const * const _self);
static size_t length(void const * const _self);
void const * getValue(void const * const _self);
bool setValue(void * const _self, void const * const value);

static struct RESTc_Class _RESTc_StringBucket = {
    sizeof(struct RESTc_StringBucket),
    false,
    &RESTc_StringConst,
    constructor,
    destructor,
    clone,
    NULL,
    NULL
};

struct PR_RESTc_StringBucket const RESTc_StringBucket = {
    {
        {
            &_RESTc_StringBucket,
            get,
            length,
            OVERRIDE,
            NULL
        }
    },
    getValue,
    setValue
};

static void* constructor(void * const _self, va_list * const app)
{
    struct RESTc_Class const * const * const pc = _RESTc_StringBucket.super;
    struct RESTc_StringBucket * const self = (*pc)->constructor(_self, app);
    char const * const value_str = va_arg(*app, char const*);
    size_t const value_length = (NULL == value_str) ? 0 : va_arg(*app, size_t);
    void * const value = RESTc_new(RESTc_String, value_str, value_length);
    if(NULL == value) {
        RESTc_delete(self);
        return NULL;
    }
    self->value = value;
    return self;
}

static void* destructor(void * const _self)
{
    struct RESTc_StringBucket * const self = _self;
    RESTc_delete(self->value);
    return self;
}

static void* clone(void const * const _self)
{
    struct RESTc_StringBucket const * const self = _self;
    char const * const key = RESTc_String.get(_self);
    size_t const key_length = RESTc_String.length(_self);
    char const * const value = RESTc_String.get(self->value);
    size_t const value_length = RESTc_String.length(self->value);
    void * clone;
    if(key == NULL) {
        if(value == NULL) {
            clone = RESTc_new(RESTc_StringBucket, key, value);
        } else {
            clone = RESTc_new(RESTc_StringBucket, key, value, value_length);
        }
    } else if(value == NULL) {
        clone = RESTc_new(RESTc_StringBucket, key, key_length, value);
    } else {
        clone = RESTc_new(RESTc_StringBucket, key, key_length, value, value_length);
    }

    return clone;
}

static char const * get(void const * const _self)
{
    return RESTc_String.get(_self);
}

static size_t length(void const * const _self)
{
    return RESTc_String.length(_self);
}

void const * getValue(void const * const _self)
{
    struct RESTc_StringBucket const * const self = _self;
    return self->value;
}

bool setValue(void * const _self, void const * const value)
{
    struct RESTc_StringBucket * const self = _self;
    void * const cvalue = RESTc_clone(value);
    if(NULL == cvalue) {
        return false;
    }
    RESTc_delete(self->value);
    self->value = cvalue;
    return true;
}
