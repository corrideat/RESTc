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
#ifndef RESTc_OAUTH2SERVICE_H
#define RESTc_OAUTH2SERVICE_H
#include <stddef.h>
#ifdef __cplusplus
extern "C" {
#endif

    extern struct PR_RESTc_OAuth2Service {
        void const * const _;
        void const * (* const getAuthorizationEndpoint)(void const *);
        void const * (* const getTokenEndpoint)(void const *);
        void const * (* const getRefreshTokenEndpoint)(void const *);
        void const * (* const getScope)(void const *);
        void const * (* const getClientId)(void const *);
        void const * (* const getClientSecret)(void const *);
        void const * (* const getRedirectUri)(void const *);
    } const RESTc_OAuth2Service;

#ifdef __cplusplus
}
#endif
#endif
