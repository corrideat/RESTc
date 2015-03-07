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
#ifndef RESTc_REQUEST_H
#define RESTc_REQUEST_H
#include <stddef.h>
#include "bool.h"
#ifdef __cplusplus
extern "C" {
#endif

    enum RESTc_Request_methods {
        GET, HEAD, POST, PUT, DELETE, PATCH
    };
    enum RESTc_Request_content_type {NONE, APPLICATION_X_WWW_FORM_URLENCODED, MULTIPART_FORM_DATA};

    extern struct PR_RESTc_Request {
        void const * const _;
        enum RESTc_Request_methods(* const getRequestMethod)(void const *);
        void (* const setRequestMethod)(void *, enum RESTc_Request_methods);
        enum RESTc_Request_content_type(* const getContentType)(void const *);
        void (* const setContentType)(void *, enum RESTc_Request_content_type);
        void const * (* const getURI)(void const *);
        void const ** (* const getQueryParameters)(void const *);
        size_t (* const getQueryParametersCount)(void const *);
        bool (* const addQueryParameter)(void *, void const * bucket);
        bool (* const addReplaceQueryParameter)(void *, void const * bucket);
        bool (* const removeQueryParameter)(void *, void const * key);
        void const ** (* const getHeaders)(void const *);
        size_t (* const getHeadersCount)(void const *);
        bool (* const addHeader)(void *, void const * bucket);
        bool (* const addReplaceHeader)(void *, void const * bucket);
        bool (* const removeHeader)(void *, void const * key);
        void const ** (* const getBodyParameters)(void const *);
        size_t (* const getBodyParametersCount)(void const *);
        bool (* const addBodyParameter)(void *, void const * bucket);
        bool (* const addReplaceBodyParameter)(void *, void const * bucket);
        bool (* const removeBodyParameter)(void *, void const * key);
    } const RESTc_Request;

#ifdef __cplusplus
}
#endif
#endif
