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
#include <setjmp.h>
#include <string.h>
#include <cmocka.h>
#include "bool.h"
#include "utility.h"
#include "RESTc_Class.h"
#include "RESTc_cURLRequestor.h"
#include "RESTc_Request.h"
#include "RESTc_StringBucket.h"

static void test_simpleGET(void ** UNUSED(state))
{
    struct RESTc_Requestor_result res;
    void *uri = RESTc_new(RESTc_StringConst, "http://localhost:5000/get", 0);
    void *request = RESTc_new(RESTc_Request, GET, NONE, uri);
    void *requestor = RESTc_new(RESTc_cURLRequestor, NULL, NULL);
    assert_non_null(uri);
    assert_non_null(request);
    assert_non_null(requestor);
    res = RESTc_Requestor.effect(requestor, request);
    assert_int_equal(res.error, OK);
    assert_non_null(res.result);
    RESTc_delete(request);
    RESTc_delete(requestor);
    RESTc_delete(res.result);
}

static void test_simpleGETquery(void ** UNUSED(state))
{
    struct RESTc_Requestor_result res;
    void *uri = RESTc_new(RESTc_StringConst, "http://localhost:5000/get", 0);
    void *request = RESTc_new(RESTc_Request, GET, NONE, uri);
    void *requestor = RESTc_new(RESTc_cURLRequestor, NULL, NULL);
    void *bucketa = RESTc_new(RESTc_StringBucket, "X", 1, "x", 1);
    void *bucketb = RESTc_new(RESTc_StringBucket, "Y", 1, NULL);
    void *bucketc = RESTc_new(RESTc_StringBucket, NULL, "z", 1);
    assert_non_null(uri);
    assert_non_null(request);
    assert_non_null(requestor);
    assert_non_null(bucketa);
    assert_non_null(bucketb);
    assert_non_null(bucketc);
    assert_int_equal(RESTc_Request.addQueryParameter(request, bucketa), true);
    assert_int_equal(RESTc_Request.addQueryParameter(request, bucketb), true);
    assert_int_equal(RESTc_Request.addQueryParameter(request, bucketc), true);
    res = RESTc_Requestor.effect(requestor, request);
    assert_int_equal(res.error, OK);
    assert_non_null(res.result);
    RESTc_delete(request);
    RESTc_delete(requestor);
    RESTc_delete(bucketa);
    RESTc_delete(bucketb);
    RESTc_delete(bucketc);
    RESTc_delete(res.result);
}

static void test_simpleGETheaders(void ** UNUSED(state))
{
    struct RESTc_Requestor_result res;
    void *uri = RESTc_new(RESTc_StringConst, "http://localhost:5000/get", 0);
    void *request = RESTc_new(RESTc_Request, GET, NONE, uri);
    void *requestor = RESTc_new(RESTc_cURLRequestor, NULL, NULL);
    void *bucketa = RESTc_new(RESTc_StringBucket, "X", 1, "x", 1);
    void *bucketb = RESTc_new(RESTc_StringBucket, "Y", 1, NULL);
    void *bucketc = RESTc_new(RESTc_StringBucket, NULL, "z", 1);
    assert_non_null(uri);
    assert_non_null(request);
    assert_non_null(requestor);
    assert_non_null(bucketa);
    assert_non_null(bucketb);
    assert_non_null(bucketc);
    assert_int_equal(RESTc_Request.addHeader(request, bucketa), true);
    assert_int_equal(RESTc_Request.addHeader(request, bucketb), true);
    assert_int_equal(RESTc_Request.addHeader(request, bucketc), true);
    res = RESTc_Requestor.effect(requestor, request);
    assert_int_equal(res.error, OK);
    assert_non_null(res.result);
    RESTc_delete(request);
    RESTc_delete(requestor);
    RESTc_delete(bucketa);
    RESTc_delete(bucketb);
    RESTc_delete(bucketc);
    RESTc_delete(res.result);
}

static void test_simplePOSTurlencoded(void ** UNUSED(state))
{
    struct RESTc_Requestor_result res;
    void *uri = RESTc_new(RESTc_StringConst, "http://localhost:5000/post", 0);
    void *request = RESTc_new(RESTc_Request, POST, APPLICATION_X_WWW_FORM_URLENCODED, uri);
    void *requestor = RESTc_new(RESTc_cURLRequestor, NULL, NULL);
    void *bucketa = RESTc_new(RESTc_StringBucket, "X", 1, "x", 1);
    void *bucketb = RESTc_new(RESTc_StringBucket, "Y", 1, NULL);
    void *bucketc = RESTc_new(RESTc_StringBucket, NULL, "z", 1);
    assert_non_null(uri);
    assert_non_null(request);
    assert_non_null(requestor);
    assert_non_null(bucketa);
    assert_non_null(bucketb);
    assert_non_null(bucketc);
    assert_int_equal(RESTc_Request.addBodyParameter(request, bucketa), true);
    assert_int_equal(RESTc_Request.addBodyParameter(request, bucketb), true);
    assert_int_equal(RESTc_Request.addBodyParameter(request, bucketc), true);
    res = RESTc_Requestor.effect(requestor, request);
    assert_int_equal(res.error, OK);
    assert_non_null(res.result);
    RESTc_delete(request);
    RESTc_delete(requestor);
    RESTc_delete(bucketa);
    RESTc_delete(bucketb);
    RESTc_delete(bucketc);
    RESTc_delete(res.result);
}

int main(void)
{
    int ret;
    struct CMUnitTest const tests[] = {
        cmocka_unit_test(test_simpleGET),
        cmocka_unit_test(test_simpleGETquery),
        cmocka_unit_test(test_simpleGETheaders),
        cmocka_unit_test(test_simplePOSTurlencoded),
    };

    RESTc_cURLRequestor.globalInit(0L);

    ret = cmocka_run_group_tests(tests, NULL, NULL);

    RESTc_cURLRequestor.globalCleanup();
    return ret;
}
