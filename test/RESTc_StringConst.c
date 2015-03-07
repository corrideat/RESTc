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
#include "utility.h"
#include "RESTc_StringConst.h"

static void test_new(void ** UNUSED(state))
{
    void *stringa = RESTc_new(RESTc_StringConst, "123456", 0);
    void *stringb = RESTc_new(RESTc_StringConst, "qwerty", 3);
    void *stringc = RESTc_new(RESTc_StringConst, NULL);
    void *stringd = RESTc_new(RESTc_StringConst, "", 0);
    assert_non_null(stringa);
    assert_non_null(stringb);
    assert_non_null(stringc);
    assert_non_null(stringd);
    assert_int_equal(RESTc_String.length(stringa), 6);
    assert_int_equal(RESTc_String.length(stringb), 3);
    assert_int_equal(RESTc_String.length(stringc), 0);
    assert_int_equal(RESTc_String.length(stringd), 0);
    assert_int_equal(strcmp(RESTc_String.get(stringa), "123456"), 0);
    assert_int_equal(strcmp(RESTc_String.get(stringb), "qwerty"), 0);
    assert_null(RESTc_String.get(stringc));
    assert_null(RESTc_String.get(stringd));
    RESTc_delete(stringa);
    RESTc_delete(stringb);
    RESTc_delete(stringc);
    RESTc_delete(stringd);
}

static void test_set(void ** UNUSED(state))
{
    void *stringa = RESTc_new(RESTc_StringConst, "123456", 0);
    void *stringb = RESTc_new(RESTc_StringConst, "qwerty", 3);
    void *stringc = RESTc_new(RESTc_StringConst, NULL);
    assert_non_null(stringa);
    assert_non_null(stringb);
    assert_non_null(stringc);
    assert_int_equal(RESTc_String.set(stringa, "abcd", 0), true);
    assert_int_equal(RESTc_String.set(stringc, "efghi", 2), true);
    assert_int_equal(RESTc_String.set(stringb, NULL, 0), true);
    assert_int_equal(RESTc_String.length(stringa), 4);
    assert_int_equal(RESTc_String.length(stringb), 0);
    assert_int_equal(RESTc_String.length(stringc), 2);
    assert_int_equal(strcmp(RESTc_String.get(stringa), "abcd"), 0);
    assert_int_equal(strcmp(RESTc_String.get(stringc), "efghi"), 0);
    assert_null(RESTc_String.get(stringb));
    RESTc_delete(stringa);
    RESTc_delete(stringb);
    RESTc_delete(stringc);
}

static void test_hash(void ** UNUSED(state))
{
    void *stringa = RESTc_new(RESTc_StringConst, "123456", 0);
    void *stringb = RESTc_new(RESTc_StringConst, NULL);
    void *mstringa = RESTc_new(RESTc_String, "123456", 0);
    void *mstringb = RESTc_new(RESTc_String, NULL);
    assert_non_null(stringa);
    assert_non_null(stringb);
    assert_non_null(mstringa);
    assert_non_null(mstringb);
    assert_int_equal(RESTc_hash(mstringa), RESTc_hash(stringa));
    assert_int_equal(RESTc_hash(mstringb), RESTc_hash(stringb));
    RESTc_delete(stringa);
    RESTc_delete(stringb);
    RESTc_delete(mstringa);
    RESTc_delete(mstringb);
}

static void test_differ(void ** UNUSED(state))
{
    void *stringa = RESTc_new(RESTc_StringConst, "123456", 0);
    void *stringb = RESTc_new(RESTc_StringConst, "qwerty", 3);
    void *stringc = RESTc_new(RESTc_StringConst, NULL);
    assert_non_null(stringa);
    assert_non_null(stringb);
    assert_non_null(stringc);
    assert_int_equal(RESTc_differ(stringa, stringa), 0);
    assert_int_equal(RESTc_differ(stringb, stringb), 0);
    assert_int_equal(RESTc_differ(stringc, stringc), 0);
    assert_true(RESTc_differ(stringa, stringb) < 0);
    assert_true(RESTc_differ(stringa, stringc) > 0);
    assert_true(RESTc_differ(stringb, stringa) > 0);
    assert_true(RESTc_differ(stringb, stringc) > 0);
    assert_true(RESTc_differ(stringc, stringa) < 0);
    assert_true(RESTc_differ(stringc, stringb) < 0);
    RESTc_delete(stringa);
    RESTc_delete(stringb);
    RESTc_delete(stringc);
}

static void test_clone(void ** UNUSED(state))
{
    void *stringa = RESTc_new(RESTc_StringConst, "123456", 0);
    void *stringb = RESTc_new(RESTc_StringConst, "qwerty", 3);
    void *stringc = RESTc_new(RESTc_StringConst, NULL);
    assert_non_null(stringa);
    assert_non_null(stringb);
    assert_non_null(stringc);
    void *cstringa = RESTc_clone(stringa);
    void *cstringb = RESTc_clone(stringb);
    void *cstringc = RESTc_clone(stringc);
    assert_non_null(cstringa);
    assert_non_null(cstringb);
    assert_non_null(cstringc);

    assert_int_equal(RESTc_String.get(stringa), RESTc_String.get(cstringa));
    assert_int_equal(RESTc_String.get(stringb), RESTc_String.get(cstringb));
    assert_int_equal(RESTc_String.get(stringc), RESTc_String.get(cstringc));

    assert_int_equal(RESTc_differ(stringa, cstringa), 0);
    assert_int_equal(RESTc_differ(stringb, cstringb), 0);
    assert_int_equal(RESTc_differ(stringc, cstringc), 0);
    assert_true(RESTc_differ(cstringa, cstringb) < 0);
    assert_true(RESTc_differ(cstringa, cstringc) > 0);
    assert_true(RESTc_differ(cstringb, cstringa) > 0);
    assert_true(RESTc_differ(cstringb, cstringc) > 0);
    assert_true(RESTc_differ(cstringc, cstringa) < 0);
    assert_true(RESTc_differ(cstringc, cstringb) < 0);
    RESTc_delete(stringa);
    RESTc_delete(stringb);
    RESTc_delete(stringc);
    RESTc_delete(cstringa);
    RESTc_delete(cstringb);
    RESTc_delete(cstringc);
}

int main(void)
{
    struct CMUnitTest const tests[] = {
        cmocka_unit_test(test_new),
        cmocka_unit_test(test_set),
        cmocka_unit_test(test_hash),
        cmocka_unit_test(test_differ),
        cmocka_unit_test(test_clone)
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
