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
#include <assert.h>
#include "utility.h"
#include <RESTc_Authenticator.h>
#include <RESTc_Processor.h>
#include "private/_RESTc_Requestor.h"

static void* constructor(void * const _self, va_list * const app);
static struct RESTc_Requestor_result effect(void const * const _self, void * const request);

static struct RESTc_Class const _RESTc_Requestor = {
    sizeof(struct RESTc_Requestor),
    true,
    NULL,
    constructor,
    NULL,
    NULL,
    NULL,
    NULL
};

struct PR_RESTc_Requestor const RESTc_Requestor = {
    &_RESTc_Requestor,
    effect
};

static void* constructor(void * const _self, va_list * const app)
{
    struct RESTc_Requestor * const self = _self;
    self->authenticator = va_arg(*app, void *);
    self->processor = va_arg(*app, void *);
    return self;
}

static struct RESTc_Requestor_result effect(void const * const _self, void * const request) {
#define DO_ACTION(x) \
ret = x; \
if (OK != ret.error) return ret
    struct PR_RESTc_Requestor const * const * const PR_self = _self;
    struct RESTc_Requestor const * const self = _self;
    struct RESTc_Requestor_result ret;
    assert(NULL != (*PR_self)->effect);
    if(NULL != self->authenticator) {
        DO_ACTION(RESTc_Authenticator.onBeforeRequest(self->processor, request));
    }
    if(NULL != self->processor) {
        DO_ACTION(RESTc_Processor.onBeforeRequest(self->processor, request));
    }
    DO_ACTION((*PR_self)->effect(_self, request));
    if(NULL != self->authenticator) {
        DO_ACTION(RESTc_Authenticator.onAfterRequest(self->processor, request, ret.result));
    }
    if(NULL != self->processor) {
        DO_ACTION(RESTc_Processor.onAfterRequest(self->processor, request, ret.result));
    }
    return ret;
}
