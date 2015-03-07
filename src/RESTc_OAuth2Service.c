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
#include "RESTc_StringConst.h"
#include "private/_RESTc_OAuth2Service.h"

static void* constructor(void * const _self, va_list * const app);
static void* destructor(void * const _self);

static void const * getAuthorizationEndpoint(void const *);
static void const * getTokenEndpoint(void const *);
static void const * getRefreshTokenEndpoint(void const *);
static void const * getScope(void const *);
static void const * getClientId(void const *);
static void const * getClientSecret(void const *);
static void const * getRedirectUri(void const *);

static struct RESTc_Class const _RESTc_OAuth2Service = {
    sizeof(struct RESTc_OAuth2Service),
    false,
    NULL,
    constructor,
    destructor,
    NULL,
    NULL,
    NULL
};


struct PR_RESTc_OAuth2Service const RESTc_OAuth2Service = {
    &_RESTc_OAuth2Service,
    getAuthorizationEndpoint,
    getTokenEndpoint,
    getRefreshTokenEndpoint,
    getScope,
    getClientId,
    getClientSecret,
    getRedirectUri
};

static void* constructor(void * const _self, va_list * const app)
{
    struct RESTc_OAuth2Service * const self = _self;
    size_t n_refresh_token_endpoint, n_client_secret;
    void * srefresh_token_endpoint, * sclient_secret;

    char const * const authorization_endpoint = va_arg(*app, char const *);
    size_t const n_authorization_endpoint = va_arg(*app, size_t);
    char const * const token_endpoint = va_arg(*app, char const *);
    size_t n_token_endpoint = va_arg(*app, size_t);
    char const * const refresh_token_endpoint = va_arg(*app, char const *);
    if(NULL == refresh_token_endpoint) {
        n_refresh_token_endpoint = va_arg(*app, size_t);
    }
    char const * const scope = va_arg(*app, char const *);
    size_t const n_scope = va_arg(*app, size_t);
    char const * const client_id = va_arg(*app, char const *);
    size_t const n_client_id = va_arg(*app, size_t);
    char const * const client_secret = va_arg(*app, char const *);
    if(NULL != client_secret) {
        n_client_secret = va_arg(*app, size_t);
    }
    char const * const redirect_uri = va_arg(*app, char const *);
    size_t const n_redirect_uri = va_arg(*app, size_t);

    if(NULL == authorization_endpoint || NULL == token_endpoint || NULL == scope || NULL == client_id || NULL == redirect_uri) {
        return NULL;
    }

    void * const sauthorization_endpoint = RESTc_new(RESTc_String, authorization_endpoint, n_authorization_endpoint);

    void * const stoken_endpoint = RESTc_new(RESTc_String, token_endpoint, n_token_endpoint);
    if(NULL == stoken_endpoint) {
        goto constructor_err_1;
    }

    if(NULL == refresh_token_endpoint) {
        srefresh_token_endpoint = stoken_endpoint;
    } else if(NULL == (srefresh_token_endpoint = RESTc_new(RESTc_String, refresh_token_endpoint, n_refresh_token_endpoint))) {
        goto constructor_err_2;
    }

    void * const sscope = RESTc_new(RESTc_String, scope, n_scope);
    if(NULL == sscope) {
        goto constructor_err_3;
    }

    void * const sclient_id = RESTc_new(RESTc_String, client_id, n_client_id);
    if(NULL == sclient_id) {
        goto constructor_err_4;
    }

    if(NULL == client_secret) {
        sclient_secret = NULL;
    } else if(NULL == (sclient_secret = RESTc_new(RESTc_String, client_secret, n_client_secret))) {
        goto constructor_err_5;
    }

    void * const sredirect_uri = RESTc_new(RESTc_String, redirect_uri, n_redirect_uri);
    if(NULL == sredirect_uri) {
        goto constructor_err_6;
    }

    self->authorization_endpoint = sauthorization_endpoint;
    self->token_endpoint = stoken_endpoint;
    self->refresh_token_endpoint = srefresh_token_endpoint;
    self->scope = sscope;
    self->client_id = sclient_id;
    self->client_secret = sclient_secret;
    self->redirect_uri = sredirect_uri;

    return self;
constructor_err_6:
    RESTc_delete(sclient_secret);
constructor_err_5:
    RESTc_delete(sclient_id);
constructor_err_4:
    RESTc_delete(sscope);
constructor_err_3:
    if(NULL != refresh_token_endpoint) {
        RESTc_delete(srefresh_token_endpoint);
    }
constructor_err_2:
    RESTc_delete(stoken_endpoint);
constructor_err_1:
    RESTc_delete(sauthorization_endpoint);
    return NULL;
}

static void* destructor(void * const _self)
{
    struct RESTc_OAuth2Service * const self = _self;
    RESTc_delete(self->authorization_endpoint);
    RESTc_delete(self->token_endpoint);
    if(self->token_endpoint != self->refresh_token_endpoint) {
        RESTc_delete(self->refresh_token_endpoint);
    }
    RESTc_delete(self->scope);
    RESTc_delete(self->client_id);
    RESTc_delete(self->client_secret);
    RESTc_delete(self->redirect_uri);
    return self;
}

static void const * getAuthorizationEndpoint(void const * const _self)
{
    struct RESTc_OAuth2Service const * const self = _self;
    return self->authorization_endpoint;
}

static void const * getTokenEndpoint(void const * const _self)
{
    struct RESTc_OAuth2Service const * const self = _self;
    return self->token_endpoint;
}

static void const * getRefreshTokenEndpoint(void const * const _self)
{
    struct RESTc_OAuth2Service const * const self = _self;
    return self->refresh_token_endpoint;
}

static void const * getScope(void const * const _self)
{
    struct RESTc_OAuth2Service const * const self = _self;
    return self->scope;
}

static void const * getClientId(void const * const _self)
{
    struct RESTc_OAuth2Service const * const self = _self;
    return self->client_id;
}

static void const * getClientSecret(void const * const _self)
{
    struct RESTc_OAuth2Service const * const self = _self;
    return self->client_secret;
}

static void const * getRedirectUri(void const * const _self)
{
    struct RESTc_OAuth2Service const * const self = _self;
    return self->redirect_uri;
}
