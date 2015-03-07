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
#ifndef _RESTC_OAUTH2SERVICE_H
#define _RESTC_OAUTH2SERVICE_H
#ifdef __cplusplus
extern "C" {
#endif
#include "_RESTc_Class.h"
#include "RESTc_OAuth2Service.h"

    struct RESTc_OAuth2Service {
        void const * class;
        void * authorization_endpoint;
        /* Token Endpoint (RFC6749 §3.2) */
        void * token_endpoint;
        /* Refresh Token Endpoint (RFC6749 §6) OPTIONAL (defaults to token_endpoint) */
        void * refresh_token_endpoint;
        /* Access Token Scope (RFC6749 §3.3) */
        void * scope;
        /* Client ID (RFC6749 §2.3.1) */
        void * client_id;
        /* Client Secret (RFC6749 §2.3.1) OPTIONAL */
        void * client_secret;
        /* Redirection URI (RFC §3.1.2) */
        void * redirect_uri;
    };

#ifdef __cplusplus
}
#endif
#endif
