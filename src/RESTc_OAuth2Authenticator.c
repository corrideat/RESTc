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
#include "utility.h"
#include "private/_RESTc_OAuth2Authenticator.h"

static void* constructor(void * const _self, va_list * const app);
static void* destructor(void * const _self);

static struct RESTc_Requestor_result onBeforeRequest(void const *, void * request);
static struct RESTc_Requestor_result onAfterRequest(void const *, void * request, void * response);

static struct RESTc_Class const _RESTc_OAuth2Authenticator = {
    sizeof(struct RESTc_Authenticator),
    true,
    &RESTc_Authenticator,
    constructor,
    destructor,
    NULL,
    NULL,
    NULL
};

struct PR_RESTc_OAuth2Authenticator const RESTc_OAuth2Authenticator = {
    {
        &_RESTc_OAuth2Authenticator,
        onBeforeRequest,
        onAfterRequest
    }
};

static void* constructor(void * const _self, va_list * const app)
{
    struct RESTc_OAuth2Authenticator * const self = _self;
    void const * const service = va_arg(*app, void const *);
    if(NULL == service) {
        return NULL;
    }
    /* TODO */
    self->service = service;
    return self;
}

static void* destructor(void * const _self)
{
    struct RESTc_OAuth2Authenticator * const self = _self;
    /* TODO */
    return self;
}

static struct RESTc_Requestor_result onBeforeRequest(void const * const _self, void * const request) {
    /* TODO */
}

static struct RESTc_Requestor_result onAfterRequest(void const * const _self, void * const request, void * const response) {
    /* TODO */
}
