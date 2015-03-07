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
#include "RESTc_String.h"
#include "RESTc_HashTable.h"
#include "private/_RESTc_Request.h"

static void* constructor(void * const _self, va_list * const app);
static void* destructor(void * const _self);

static enum RESTc_Request_methods getRequestMethod(void const *);
static void setRequestMethod(void *, enum RESTc_Request_methods);
static enum RESTc_Request_content_type getContentType(void const *);
static void setContentType(void *, enum RESTc_Request_content_type);
static void const * getURI(void const *);
static void const ** getQueryParameters(void const *);
static size_t getQueryParametersCount(void const *);
static bool addQueryParameter(void *, void const * bucket);
static bool addReplaceQueryParameter(void *, void const * bucket);
static bool removeQueryParameter(void *, void const * key);
static void const ** getHeaders(void const *);
static size_t getHeadersCount(void const *);
static bool addHeader(void *, void const * bucket);
static bool addReplaceHeader(void *, void const * bucket);
static bool removeHeader(void *, void const * key);
static void const ** getBodyParameters(void const *);
static size_t getBodyParametersCount(void const *);
static bool addBodyParameter(void *, void const * bucket);
static bool addReplaceBodyParameter(void *, void const * bucket);
static bool removeBodyParameter(void *, void const * key);

static struct RESTc_Class const _RESTc_Request = {
    sizeof(struct RESTc_Request),
    false,
    NULL,
    constructor,
    destructor,
    NULL,
    NULL,
    NULL
};


struct PR_RESTc_Request const RESTc_Request = {
    &_RESTc_Request,
    getRequestMethod,
    setRequestMethod,
    getContentType,
    setContentType,
    getURI,
    getQueryParameters,
    getQueryParametersCount,
    addQueryParameter,
    addReplaceQueryParameter,
    removeQueryParameter,
    getHeaders,
    getHeadersCount,
    addHeader,
    addReplaceHeader,
    removeHeader,
    getBodyParameters,
    getBodyParametersCount,
    addBodyParameter,
    addReplaceBodyParameter,
    removeBodyParameter,
};

static void* constructor(void * const _self, va_list * const app)
{
    struct RESTc_Request * const self = _self;
    enum RESTc_Request_methods const method = va_arg(*app, enum RESTc_Request_methods);
    enum RESTc_Request_content_type const content_type = va_arg(*app, enum RESTc_Request_content_type);
    void * const uri = va_arg(*app, void *);
    if(NULL == uri) {
        goto ctr_error_cleanup1;
    }
    void * const query_parameters = RESTc_new(RESTc_HashTable, 0);
    if(NULL == query_parameters) {
        goto ctr_error_cleanup2;
    }
    void * const headers = RESTc_new(RESTc_HashTable, 0);
    if(NULL == headers) {
        goto ctr_error_cleanup3;
    }
    void * const body_parameters = RESTc_new(RESTc_HashTable, 0);
    if(NULL == body_parameters) {
        goto ctr_error_cleanup4;
    }
    self->request_method = method;
    self->content_type = content_type;
    self->uri = uri;
    self->query_parameters = query_parameters;
    self->headers = headers;
    self->body_parameters = body_parameters;
    return self;
ctr_error_cleanup4:
    RESTc_delete(headers);
ctr_error_cleanup3:
    RESTc_delete(query_parameters);
ctr_error_cleanup2:
    RESTc_delete(uri);
ctr_error_cleanup1:
    return NULL;
}

static void* destructor(void * const _self)
{
    struct RESTc_Request * const self = _self;
    RESTc_delete(self->uri);
    RESTc_delete(self->query_parameters);
    RESTc_delete(self->headers);
    RESTc_delete(self->body_parameters);
    return self;
}

static enum RESTc_Request_methods getRequestMethod(void const * const _self)
{
    struct RESTc_Request const * const self = _self;
    return self->request_method;
}

static void setRequestMethod(void * const _self, enum RESTc_Request_methods const method)
{
    struct RESTc_Request * const self = _self;
    self->request_method = method;
}

static enum RESTc_Request_content_type getContentType(void const * const _self)
{
    struct RESTc_Request const * const self = _self;
    return self->content_type;
}

static void setContentType(void * const _self, enum RESTc_Request_content_type const content_type)
{
    struct RESTc_Request * const self = _self;
    self->content_type = content_type;
}

static void const * getURI(void const * const _self)
{
    struct RESTc_Request const * const self = _self;
    return self->uri;
}

static void const ** getQueryParameters(void const * const _self)
{
    struct RESTc_Request const * const self = _self;
    return RESTc_HashTable.getAll(self->query_parameters);
}

static size_t getQueryParametersCount(void const * const _self)
{
    struct RESTc_Request const * const self = _self;
    return RESTc_HashTable.getCount(self->query_parameters);
}

static bool addQueryParameter(void * const _self, void const * const bucket)
{
    struct RESTc_Request const * const self = _self;
    return RESTc_HashTable.insert(self->query_parameters, bucket);
}

static bool addReplaceQueryParameter(void * const _self, void const * const bucket)
{
    struct RESTc_Request const * const self = _self;
    return RESTc_HashTable.insertReplace(self->query_parameters, bucket);
}

static bool removeQueryParameter(void * const _self, void const * const key)
{
    struct RESTc_Request const * const self = _self;
    return RESTc_HashTable.remove(self->query_parameters, key);
}

static void const ** getHeaders(void const * const _self)
{
    struct RESTc_Request const * const self = _self;
    return RESTc_HashTable.getAll(self->headers);
}

static size_t getHeadersCount(void const * const _self)
{
    struct RESTc_Request const * const self = _self;
    return RESTc_HashTable.getCount(self->headers);
}

static bool addHeader(void * const _self, void const * const bucket)
{
    struct RESTc_Request const * const self = _self;
    return RESTc_HashTable.insert(self->headers, bucket);
}

static bool addReplaceHeader(void * const _self, void const * const bucket)
{
    struct RESTc_Request const * const self = _self;
    return RESTc_HashTable.insertReplace(self->headers, bucket);
}

static bool removeHeader(void * const _self, void const * const key)
{
    struct RESTc_Request const * const self = _self;
    return RESTc_HashTable.remove(self->headers, key);
}

static void const ** getBodyParameters(void const * const _self)
{
    struct RESTc_Request const * const self = _self;
    return RESTc_HashTable.getAll(self->body_parameters);
}

static size_t getBodyParametersCount(void const * const _self)
{
    struct RESTc_Request const * const self = _self;
    return RESTc_HashTable.getCount(self->body_parameters);
}

static bool addBodyParameter(void * const _self, void const * const bucket)
{
    struct RESTc_Request const * const self = _self;
    return RESTc_HashTable.insert(self->body_parameters, bucket);
}

static bool addReplaceBodyParameter(void * const _self, void const * const bucket)
{
    struct RESTc_Request const * const self = _self;
    return RESTc_HashTable.insertReplace(self->body_parameters, bucket);
}

static bool removeBodyParameter(void * const _self, void const * const key)
{
    struct RESTc_Request const * const self = _self;
    return RESTc_HashTable.remove(self->body_parameters, key);
}
