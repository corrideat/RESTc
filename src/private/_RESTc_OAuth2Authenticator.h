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
#ifndef _RESTC_OAUTH2AUTHENTICATOR_H
#define _RESTC_OAUTH2AUTHENTICATOR_H
#ifdef __cplusplus
extern "C" {
#endif
#include "_RESTc_Class.h"
#include "_RESTc_Authenticator.h"
#include "RESTc_OAuth2Authenticator.h"

    struct RESTc_OAuth2Authenticator {
        struct RESTc_Authenticator super;
        void const * service;
        /* Authorization Mechanism (RFC6749 §4) */
        enum RESTc_OAuth2Authenticator_authorization_mechanism authorization_type;
        void * authorization_parameters;
        /* Token Type (RFC6749 §5.1, §7.1) */
        enum RESTc_OAuth2Authenticator_token_type token_type;
        /* Access Token (RFC6749 §4.2.2) */
        void * access_token;
        /* Expiration time */
        unsigned long expiry_time;
        /* Access Token Scope (RFC6749 §3.3) */
        void * scope;
        /* Parameters (RFC6749 §5.1) */
        void * token_parameters;
    };

#ifdef __cplusplus
}
#endif
#endif
