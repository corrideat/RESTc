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
#ifndef RESTc_OAUTH2AUTHENTICATOR_H
#define RESTc_OAUTH2AUTHENTICATOR_H
#include <stddef.h>
#include "RESTc_Authenticator.h"
#ifdef __cplusplus
extern "C" {
#endif

    enum RESTc_OAuth2Authenticator_authorization_mechanism {
        AUTHORIZATION_CODE,
        IMPLICIT,
        RESOURCE_OWNER_PASSWORD,
        CLIENT_CREDENTIALS
    };

    enum RESTc_OAuth2Authenticator_token_type {
        BEARER
    };

    extern struct PR_RESTc_OAuth2Authenticator {
        struct PR_RESTc_Authenticator _;
    } const RESTc_OAuth2Authenticator;

#ifdef __cplusplus
}
#endif
#endif
