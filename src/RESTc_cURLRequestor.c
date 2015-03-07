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
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include "utility.h"
#include "RESTc_ResponseStub.h"
#include "RESTc_Request.h"
#include "RESTc_HashTable.h"
#include "RESTc_String.h"
#include "RESTc_StringBucket.h"
#include "private/_RESTc_cURLRequestor.h"

/* TODO:
 . Support for MULTIPART_FORM_DATA (including file upload)
 . More descriptive errors (especially for cURL errors)
 */

struct data_helper {
    CURL * handle;
    void * response_stub;
};

#define HEADER_BUFFER_SIZE 1024

static void* constructor(void * const _self, va_list * const app);
static void* destructor(void * const _self);

static CURLcode globalInit(long const options);
static void globalCleanup(void);
static struct RESTc_Requestor_result effect(void const *, void const *request);

static size_t write_callback(char *ptr, size_t size, size_t nmemb, void *userdata);

static struct RESTc_Class const _RESTc_cURLRequestor = {
    sizeof(struct RESTc_cURLRequestor),
    false,
    &RESTc_Requestor,
    constructor,
    destructor,
    NULL,
    NULL,
    NULL
};

struct PR_RESTc_cURLRequestor const RESTc_cURLRequestor = {
    {
        &_RESTc_cURLRequestor,
        (struct RESTc_Requestor_result(*const)(const void *, void *))effect
    },
    globalInit,
    globalCleanup
};

static void* constructor(void * const _self, va_list * const app)
{
    struct RESTc_Class const * const * const pc = _RESTc_cURLRequestor.super;
    struct RESTc_cURLRequestor * const self = (*pc)->constructor(_self, app);
    if(NULL == (self->handle = curl_easy_init())) {
        RESTc_delete(self);
        return NULL;
    }
    return self;
}

static void* destructor(void * const _self)
{
    struct RESTc_cURLRequestor * const self = _self;
    curl_easy_cleanup(self->handle);
    return self;
}

static CURLcode globalInit(long const options)
{
    return curl_global_init(options);
}

static void globalCleanup(void)
{
    curl_global_cleanup();
}

static bool escape_args(void const * const * const buckets, size_t const n_buckets, char *** const tkeys, size_t ** const tn_keys, char *** const tvalues, size_t ** const tn_values, size_t * const tcombined_length, CURL * handle)
{
    char ** const keys = malloc(sizeof(char *)*n_buckets);
    char ** const values = malloc(sizeof(char *)*n_buckets);
    size_t * const n_keys = malloc(sizeof(size_t) * n_buckets);
    size_t * const n_values = malloc(sizeof(size_t) * n_buckets);
    size_t i, j, combined_length = 0;

    char const *temp_key, *temp_value;
    char *temp_e_key, *temp_e_value;
    size_t n_temp_key, n_temp_value;

    if(NULL == keys || NULL == values || NULL == n_keys || NULL == n_values) {
        goto RESTc_StringBucket_to_char_kv_err_1;
    }

    for(i = 0; i < n_buckets; i++) {
        temp_key = RESTc_String.get(buckets[i]);
        n_temp_key = RESTc_String.length(buckets[i]);
        temp_value = RESTc_String.get(RESTc_StringBucket.getValue(buckets[i]));
        n_temp_value = RESTc_String.length(RESTc_StringBucket.getValue(buckets[i]));
        if(NULL != (temp_e_key = curl_easy_escape(handle, (NULL == temp_key) ? "" : temp_key, n_temp_key))) {
            if(NULL != (temp_e_value = curl_easy_escape(handle, (NULL == temp_value) ? "" : temp_value, n_temp_value))) {
                keys[i] = temp_e_key;
                n_keys[i] = strlen(temp_e_key);
                values[i] = temp_e_value;
                n_values[i] = strlen(temp_e_value);
                combined_length += n_keys[i] + n_values[i];
                continue;
            }
        }
        temp_e_value = NULL;
        goto RESTc_StringBucket_to_char_kv_err_2;
    }

    *tkeys = keys;
    *tn_keys = n_keys;
    *tvalues = values;
    *tn_values = n_values;
    if(NULL != tcombined_length) {
        *tcombined_length = combined_length;
    }

    return true;

RESTc_StringBucket_to_char_kv_err_2:
    for(j = 0; j < i; j++) {
        curl_free(keys[i]);
        curl_free(values[i]);
    }
    curl_free(temp_e_key);
    curl_free(temp_e_value);
RESTc_StringBucket_to_char_kv_err_1:
    free(keys);
    free(n_keys);
    free(values);
    free(n_values);
    return false;
}

static struct RESTc_Requestor_result effect(void const * const _self, void const * const request) {
    struct RESTc_Requestor_result ret = {
        REQUESTOR_INTERNAL_ERROR,
        NULL
    };
    CURLcode res;
    struct RESTc_cURLRequestor const * const self = _self;
    enum RESTc_Request_methods const emethod = RESTc_Request.getRequestMethod(request);
    char const * method;
    enum RESTc_Request_content_type const content_type = RESTc_Request.getContentType(request);
    void const * const wuri = RESTc_Request.getURI(request);
    char * uri;
    size_t const n_wquery_parameters = RESTc_Request.getQueryParametersCount(request);
    void const ** const wquery_parameters = (0 == n_wquery_parameters) ? NULL : RESTc_Request.getQueryParameters(request);
    char **query_parameters_names = NULL, **query_parameters_values = NULL, **headers = NULL, **body_parameters_names = NULL, **body_parameters_values = NULL, *body_parameters = NULL;
    size_t *n_query_parameters_names = NULL, *n_query_parameters_values = NULL, *n_body_parameters_names = NULL, *n_body_parameters_values = NULL;
    size_t const n_wheaders = RESTc_Request.getHeadersCount(request);
    void const ** const wheaders = (0 == n_wheaders) ? NULL : RESTc_Request.getHeaders(request);
    size_t const n_wbody_parameters = RESTc_Request.getBodyParametersCount(request);
    void const ** const wbody_parameters = (0 == n_wbody_parameters) ? NULL : RESTc_Request.getBodyParameters(request);
    CURL * const handle = self->handle;
    struct curl_slist *header_list = NULL;
    /* Set to 1 for being able to call CURLOPT_POSTFIELDSIZE with body_length - 1 when the request body is empty */
    size_t body_length = 1;
    size_t i, pos;
    void * response_stub;

    if(NULL == wquery_parameters && 0 != n_wquery_parameters) {
        goto effect_err_1;
    }

    if(NULL == wheaders && 0 != n_wheaders) {
        goto effect_err_2;
    }

    if(NULL == wbody_parameters && 0 != n_wbody_parameters) {
        goto effect_err_3;
    }

    /* Get full URL */
    size_t uri_length = RESTc_String.length(wuri) + 1;

    if((size_t) - 1 == uri_length) {
        goto effect_err_4;
    }

    if(0 != n_wquery_parameters) {
        uri_length += 2 * n_wquery_parameters;
        size_t query_length;
        if(false == escape_args(wquery_parameters, n_wquery_parameters, &query_parameters_names, &n_query_parameters_names, &query_parameters_values, &n_query_parameters_values, &query_length, handle)) {
            goto effect_err_4;
        }
        uri_length += query_length;
    }

    pos = 0;
    if(NULL == (uri = malloc(sizeof(char) * uri_length))) {
        goto effect_err_5;
    }
    uri_length--;
    pos = RESTc_String.length(wuri);
    memcpy(uri, RESTc_String.get(wuri), pos);
    uri[pos++] = '?';
    for(i = 0; i < n_wquery_parameters; i++) {
        memcpy(uri + pos, query_parameters_names[i], n_query_parameters_names[i]);
        pos += n_query_parameters_names[i];
        uri[pos++] = '=';
        memcpy(uri + pos, query_parameters_values[i], n_query_parameters_values[i]);
        pos += n_query_parameters_values[i];
        uri[pos++] = '&';
    }
    uri[uri_length] = '\0';

    /* Headers */
    if(0 != n_wheaders) {
        size_t n_temp_key, n_temp_value;
        char const * temp_key, * temp_value;

        headers = malloc(sizeof(char*)*n_wheaders);
        if(NULL == headers) {
            goto effect_err_5;
        }
        for(i = 0; i < n_wheaders; i++) {
            n_temp_key = RESTc_String.length(wheaders[i]);
            n_temp_value = RESTc_String.length(RESTc_StringBucket.getValue(wheaders[i]));
            temp_key = RESTc_String.get(wheaders[i]);
            temp_value = RESTc_String.get(RESTc_StringBucket.getValue(wheaders[i]));
            if(NULL == (headers[i] = malloc(sizeof(char) * (3 + n_temp_key + n_temp_value)))) {
                goto effect_err_6;
            }
            memcpy(headers[i], temp_key, n_temp_key);
            memcpy(headers[i] + n_temp_key, ": ", 2);
            memcpy(headers[i] + n_temp_key + 2, NULL == (temp_value) ? "" : temp_value, n_temp_value + 1);
            header_list = curl_slist_append(header_list, headers[i]);
        }
    }

    /* Request body */
    if(APPLICATION_X_WWW_FORM_URLENCODED == content_type) {
        if(0 != n_wbody_parameters) {
            if(false == escape_args(wbody_parameters, n_wbody_parameters, &body_parameters_names, &n_body_parameters_names, &body_parameters_values, &n_body_parameters_values, &body_length, handle)) {
                goto effect_err_6;
            }
            body_length += 2 * n_wbody_parameters;
            if(NULL == (body_parameters = malloc(sizeof(char) * body_length))) {
                goto effect_err_6;
            }
            pos = 0;
            for(i = 0; i < n_wbody_parameters; i++) {
                memcpy(body_parameters + pos, body_parameters_names[i], n_body_parameters_names[i]);
                pos += n_body_parameters_names[i];
                body_parameters[pos++] = '=';
                memcpy(body_parameters + pos, body_parameters_values[i], n_body_parameters_values[i]);
                pos += n_body_parameters_values[i];
                body_parameters[pos++] = '&';
            }
            body_parameters[body_length - 1] = '\0';
        }
    } else if(MULTIPART_FORM_DATA == content_type) {
        /* TODO: Use curl_formadd... */
    }

    switch(emethod) {
    case GET:
        method = "GET";
        break;
    case POST:
        method = "POST";
        break;
    case PUT:
        method = "PUT";
        break;
    case DELETE:
        method = "DELETE";
        break;
    case PATCH:
        method = "PATCH";
        break;
    case HEAD:
        method = "HEAD";
        break;
    default:
        method = "GET";
        break;
    }

    /* Set options */
    curl_easy_setopt(handle, CURLOPT_URL, uri);
    curl_easy_setopt(handle, CURLOPT_POST, (APPLICATION_X_WWW_FORM_URLENCODED == content_type) ? 1L : 0L);
    curl_easy_setopt(handle, CURLOPT_HTTPPOST, (MULTIPART_FORM_DATA == content_type) ? 1L : 0L);
    curl_easy_setopt(handle, CURLOPT_HTTPGET, (GET == emethod || HEAD == emethod) ? 1L : 0L);
    curl_easy_setopt(handle, CURLOPT_NOBODY, (HEAD == emethod) ? 1L : 0L);
    curl_easy_setopt(handle, CURLOPT_CUSTOMREQUEST, method);
    curl_easy_setopt(handle, CURLOPT_HTTPHEADER, header_list);
    curl_easy_setopt(handle, CURLOPT_POSTFIELDSIZE, body_length - 1);
    curl_easy_setopt(handle, CURLOPT_POSTFIELDS, body_parameters);
    curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, write_callback);

    /* Effect request */
    if(NULL == (response_stub = RESTc_new(RESTc_ResponseStub))) {
        goto effect_err_7;
    }

    struct data_helper const data_helper = {
        handle,
        response_stub
    };

    curl_easy_setopt(handle, CURLOPT_WRITEDATA, &data_helper);

    if(CURLE_OK == (res = curl_easy_perform(handle))) {
        ret.error = OK;
        ret.result = response_stub;
        goto successful_cleanup;
    } else {
        goto effect_err_8;
    }

effect_err_8:
    RESTc_delete(response_stub);
successful_cleanup:
effect_err_7:
    if((APPLICATION_X_WWW_FORM_URLENCODED == content_type)) {
        for(i = 0; i < n_wbody_parameters; i++) {
            curl_free(body_parameters_names[i]);
            curl_free(body_parameters_values[i]);
        }
        free(n_body_parameters_names);
        free(n_body_parameters_values);
        free(body_parameters_names);
        free(body_parameters_values);
        free(body_parameters);
    }
effect_err_6:
    for(i = 0; i < n_wheaders && NULL != headers[i]; i++) {
        free(headers[i]);
    }
    curl_slist_free_all(header_list);
    free(uri);
effect_err_5:
    free(headers);
    for(i = 0; i < n_wquery_parameters; i++) {
        curl_free(query_parameters_names[i]);
        curl_free(query_parameters_values[i]);
    }
    free(query_parameters_names);
    free(query_parameters_values);
    free(n_query_parameters_names);
    free(n_query_parameters_values);
effect_err_4:
    free(wbody_parameters);
effect_err_3:
    free(wheaders);
effect_err_2:
    free(wquery_parameters);
effect_err_1:
    return ret;
}

static size_t write_callback(char * const ptr, size_t const size, size_t const nmemb, void * const _data_helper)
{
    struct data_helper const * const data_helper = _data_helper;
    size_t realsize = size * nmemb;
    void *body;

    long response_code;
    char const ** content_type = NULL, ** location = NULL;

    if(NULL == (body = RESTc_new(RESTc_String, ptr, realsize))) {
        return 0;
    }

    if(CURLE_OK != curl_easy_getinfo(data_helper->handle, CURLINFO_RESPONSE_CODE, &response_code)) {
        response_code = 0;
    }

    if(CURLE_OK == curl_easy_getinfo(data_helper->handle, CURLINFO_CONTENT_TYPE, content_type) && NULL != content_type) {
        void * content_type_bucket = RESTc_new(RESTc_StringBucket, "Content-Type", 12, *content_type, 0);
        if(NULL != content_type_bucket) {
            RESTc_ResponseStub.addHeader(data_helper->response_stub, content_type_bucket);
            RESTc_delete(content_type_bucket);
        }
    }

    if(CURLE_OK == curl_easy_getinfo(data_helper->handle, CURLINFO_REDIRECT_URL, location) && NULL != location) {
        void * location_bucket = RESTc_new(RESTc_StringBucket, "Location", 8, *location, 0);
        if(NULL != location_bucket) {
            RESTc_ResponseStub.addHeader(data_helper->response_stub, location_bucket);
            RESTc_delete(location_bucket);
        }
    }

    RESTc_ResponseStub.setResponseCode(data_helper->response_stub, (unsigned char)response_code);

    RESTc_ResponseStub.setBody(data_helper->response_stub, body);

    return realsize;
}
