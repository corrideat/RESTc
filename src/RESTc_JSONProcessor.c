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
#include <jansson.h>
#include "bool.h"
#include "utility.h"
#include "private/_RESTc_JSONProcessor.h"

#include "RESTc_StringBucket.h"
#include "RESTc_Request.h"
#include "RESTc_ResponseStub.h"

#ifndef JSON_ALLOW_NUL
#define JSON_ALLOW_NUL 0
#endif

static struct RESTc_Requestor_result onBeforeRequest(void const *, void * request);
static struct RESTc_Requestor_result onAfterRequest(void const *, void * request, void * response);

static struct RESTc_Class const _RESTc_JSONProcessor = {
    sizeof(struct RESTc_JSONProcessor),
    false,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL
};

struct PR_RESTc_JSONProcessor const RESTc_JSONProcessor = {
    {
        &_RESTc_JSONProcessor,
        onBeforeRequest,
        onAfterRequest
    }
};

static struct RESTc_Requestor_result onBeforeRequest(void const * const UNUSED(_self), void * const request) {
    void * accept;
    struct RESTc_Requestor_result ret = {
        PROCESSOR_INTERNAL_ERROR,
        NULL
    };
    bool res = false;
    if(NULL != (accept = RESTc_new(RESTc_StringBucket, "Accept", 6, "application/json", 16))) {
        res = RESTc_Request.addReplaceHeader(request, accept);
        RESTc_delete(accept);
    }
    if(true == res) {
        ret.error = OK;
    }
    return ret;
}

static struct RESTc_Requestor_result onAfterRequest(void const * const UNUSED(_self), void * const UNUSED(request), void * const response) {
    struct RESTc_Requestor_result ret = {
        PROCESSOR_UNPROCESSABLE,
        NULL
    };
    void const * body = RESTc_ResponseStub.getBody(response);
    json_t *json;
    json = json_loadb(RESTc_String.get(body), RESTc_String.length(body), JSON_ALLOW_NUL | JSON_REJECT_DUPLICATES, NULL);
    if(NULL != json) {
        ret.error = OK;
        ret.result = json;
    }
    RESTc_delete(response);
    return ret;
}
