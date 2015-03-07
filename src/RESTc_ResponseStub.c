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
#include <string.h>
#include "utility.h"
#include "RESTc_String.h"
#include "RESTc_HashTable.h"
#include "private/_RESTc_ResponseStub.h"

static void* constructor(void * const _self, va_list * const app);
static void* destructor(void * const _self);

static unsigned char getResponseCode(void const *);
static void setResponseCode(void *, unsigned char);
static void const ** getHeaders(void const *);
static size_t getHeadersCount(void const *);
static bool addHeader(void *, void const * bucket);
static bool addReplaceHeader(void *, void const * bucket);
static bool removeHeader(void *, void const * key);
static void const * getBody(void const *);
static bool setBody(void *, void const * body);


static struct RESTc_Class const _RESTc_ResponseStub = {
    sizeof(struct RESTc_ResponseStub),
    false,
    NULL,
    constructor,
    destructor,
    NULL,
    NULL,
    NULL
};


struct PR_RESTc_ResponseStub const RESTc_ResponseStub = {
    &_RESTc_ResponseStub,
    getResponseCode,
    setResponseCode,
    getHeaders,
    getHeadersCount,
    addHeader,
    addReplaceHeader,
    removeHeader,
    getBody,
    setBody,
};

static void* constructor(void * const _self, va_list * const UNUSED(app))
{
    struct RESTc_ResponseStub * const self = _self;
    void * const headers = RESTc_new(RESTc_HashTable, 0);
    if(NULL == headers) {
        return NULL;
    }
    self->response_code = 0;
    self->headers = headers;
    self->body = NULL;
    return self;
}

static void* destructor(void * const _self)
{
    struct RESTc_ResponseStub * const self = _self;
    RESTc_delete(self->headers);
    RESTc_delete((void *)self->body);
    return self;
}

static unsigned char getResponseCode(void const * const _self)
{
    struct RESTc_ResponseStub const * const self = _self;
    return self->response_code;
}

static void setResponseCode(void * const _self, unsigned char const response_code)
{
    struct RESTc_ResponseStub * const self = _self;
    self->response_code = response_code;
}

static void const ** getHeaders(void const * const _self)
{
    struct RESTc_ResponseStub const * const self = _self;
    return RESTc_HashTable.getAll(self->headers);
}

static size_t getHeadersCount(void const * const _self)
{
    struct RESTc_ResponseStub const * const self = _self;
    return RESTc_HashTable.getCount(self->headers);
}

static bool addHeader(void * const _self, void const * const bucket)
{
    struct RESTc_ResponseStub const * const self = _self;
    return RESTc_HashTable.insert(self->headers, bucket);
}

static bool addReplaceHeader(void * const _self, void const * const bucket)
{
    struct RESTc_ResponseStub const * const self = _self;
    return RESTc_HashTable.insertReplace(self->headers, bucket);
}

static bool removeHeader(void * const _self, void const * const key)
{
    struct RESTc_ResponseStub const * const self = _self;
    return RESTc_HashTable.remove(self->headers, key);
}

static void const * getBody(void const * const _self)
{
    struct RESTc_ResponseStub const * const self = _self;
    return self->body;
}

static bool setBody(void * const _self, void const * const body)
{
    struct RESTc_ResponseStub * const self = _self;
    self->body = body;
    return true;
}

