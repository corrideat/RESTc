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

#include <inttypes.h>
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include "utility.h"
#include "RESTc_HashTable.h"
#include "../src/private/_RESTc_Class.h"

// TODO: test rehashing, remove, insertReplace

struct test_value_object {
    void const* class;
    unsigned long key;
    unsigned long value;
};

static void* test_value_object_constructor(void * const _self, va_list * const app);
static void* test_value_object_clone(void const * const _self);
static int test_value_object_differ(void const * const _self, void const * const _b);
static uint64_t test_value_object_hash(void const * const _self);

static struct RESTc_Class const __test_value_object = {
    sizeof(struct test_value_object),
    false,
    NULL,
    test_value_object_constructor,
    NULL,
    test_value_object_clone,
    test_value_object_differ,
    test_value_object_hash
};

static struct RESTc_Class const * const _test_value_object = &__test_value_object;

static void* test_value_object_constructor(void * const _self, va_list * const app)
{
    struct test_value_object * const self = _self;

    self->key = va_arg(*app, unsigned long);
    self->value = va_arg(*app, unsigned long);

    return self;
}

static void* test_value_object_clone(void const * const _self)
{
    struct test_value_object const * const self = _self;
    return RESTc_new(_test_value_object, self->key, self->value);
}

static int test_value_object_differ(void const * const _self, void const * const _b)
{
    struct test_value_object const * const self = _self;
    struct test_value_object const * const b = _b;
    return (self->key == b->key) ? 0L : 1L;
}

static uint64_t test_value_object_hash(void const * const _self)
{
    struct test_value_object const * const self = _self;
    return self->key;
}

/* A test case that does nothing and succeeds. */
static void test_insert(void ** UNUSED(state))
{
    void *hash_table = RESTc_new(RESTc_HashTable, 10);
    assert_non_null(hash_table);
    struct test_value_object *elementa = (struct test_value_object *) RESTc_new(_test_value_object, 9, 9);
    struct test_value_object *elementb = (struct test_value_object *) RESTc_new(_test_value_object, 19, 19);
    struct test_value_object *elementc = (struct test_value_object *) RESTc_new(_test_value_object, 0, 0);
    struct test_value_object *elementd = (struct test_value_object *) RESTc_new(_test_value_object, 6, 6);
    assert_non_null(elementa);
    assert_non_null(elementb);
    assert_non_null(elementc);
    assert_non_null(elementd);
    assert_int_equal(RESTc_HashTable.insert(hash_table, elementa), true);
    assert_int_equal(RESTc_HashTable.insert(hash_table, elementb), true);
    assert_int_equal(RESTc_HashTable.insert(hash_table, elementc), true);
    assert_int_equal(RESTc_HashTable.insert(hash_table, elementd), true);
    struct test_value_object const* helementa = (struct test_value_object const*) RESTc_HashTable.find(hash_table, elementa);
    struct test_value_object const* helementb = (struct test_value_object const*) RESTc_HashTable.find(hash_table, elementb);
    struct test_value_object const* helementc = (struct test_value_object const*) RESTc_HashTable.find(hash_table, elementc);
    struct test_value_object const* helementd = (struct test_value_object const*) RESTc_HashTable.find(hash_table, elementd);
    assert_non_null(helementa);
    assert_non_null(helementb);
    assert_non_null(helementc);
    assert_non_null(helementd);
    assert_true(elementa != helementa);
    assert_true(elementb != helementb);
    assert_true(elementc != helementc);
    assert_true(elementd != helementd);
    assert_int_equal(elementa->value, helementa->value);
    assert_int_equal(elementb->value, helementb->value);
    assert_int_equal(elementc->value, helementc->value);
    assert_int_equal(elementd->value, helementd->value);
    RESTc_delete(elementa);
    RESTc_delete(elementb);
    RESTc_delete(elementc);
    RESTc_delete(elementd);
    RESTc_delete(hash_table);
}

static void test_remove(void ** UNUSED(state))
{
    struct test_value_object const *helementa, *helementb, *helementc, *helementd;
    void *hash_table = RESTc_new(RESTc_HashTable, 10);
    assert_non_null(hash_table);
    struct test_value_object *elementa = (struct test_value_object *) RESTc_new(_test_value_object, 99, 99);
    struct test_value_object *elementb = (struct test_value_object *) RESTc_new(_test_value_object, 19, 19);
    struct test_value_object *elementc = (struct test_value_object *) RESTc_new(_test_value_object, 0, 0);
    struct test_value_object *elementd = (struct test_value_object *) RESTc_new(_test_value_object, 36, 36);
    assert_non_null(elementa);
    assert_non_null(elementb);
    assert_non_null(elementc);
    assert_non_null(elementd);
    assert_int_equal(RESTc_HashTable.insert(hash_table, elementa), true);
    assert_int_equal(RESTc_HashTable.insert(hash_table, elementb), true);
    assert_int_equal(RESTc_HashTable.insert(hash_table, elementc), true);
    assert_int_equal(RESTc_HashTable.insert(hash_table, elementd), true);
    assert_int_equal(RESTc_HashTable.remove(hash_table, elementd), true);
    helementa = (struct test_value_object const*) RESTc_HashTable.find(hash_table, elementa);
    helementb = (struct test_value_object const*) RESTc_HashTable.find(hash_table, elementb);
    helementc = (struct test_value_object const*) RESTc_HashTable.find(hash_table, elementc);
    helementd = (struct test_value_object const*) RESTc_HashTable.find(hash_table, elementd);
    assert_non_null(helementa);
    assert_non_null(helementb);
    assert_non_null(helementc);
    assert_null(helementd);
    assert_true(elementa != helementa);
    assert_true(elementb != helementb);
    assert_true(elementc != helementc);
    assert_int_equal(elementa->value, helementa->value);
    assert_int_equal(elementb->value, helementb->value);
    assert_int_equal(elementc->value, helementc->value);
    assert_int_equal(RESTc_HashTable.remove(hash_table, elementd), false);
    assert_int_equal(RESTc_HashTable.remove(hash_table, elementa), true);
    helementa = (struct test_value_object const*) RESTc_HashTable.find(hash_table, elementa);
    helementb = (struct test_value_object const*) RESTc_HashTable.find(hash_table, elementb);
    helementc = (struct test_value_object const*) RESTc_HashTable.find(hash_table, elementc);
    helementd = (struct test_value_object const*) RESTc_HashTable.find(hash_table, elementd);
    assert_null(helementa);
    assert_non_null(helementb);
    assert_non_null(helementc);
    assert_null(helementd);
    assert_true(elementb != helementb);
    assert_true(elementc != helementc);
    assert_int_equal(elementb->value, helementb->value);
    assert_int_equal(elementc->value, helementc->value);
    assert_int_equal(RESTc_HashTable.remove(hash_table, elementa), false);
    assert_int_equal(RESTc_HashTable.remove(hash_table, elementd), false);
    assert_int_equal(RESTc_HashTable.remove(hash_table, elementc), true);
    helementa = (struct test_value_object const*) RESTc_HashTable.find(hash_table, elementa);
    helementb = (struct test_value_object const*) RESTc_HashTable.find(hash_table, elementb);
    helementc = (struct test_value_object const*) RESTc_HashTable.find(hash_table, elementc);
    helementd = (struct test_value_object const*) RESTc_HashTable.find(hash_table, elementd);
    assert_null(helementa);
    assert_non_null(helementb);
    assert_null(helementc);
    assert_null(helementd);
    assert_true(elementb != helementb);
    assert_int_equal(elementb->value, helementb->value);
    assert_int_equal(RESTc_HashTable.remove(hash_table, elementd), false);
    assert_int_equal(RESTc_HashTable.remove(hash_table, elementc), false);
    assert_int_equal(RESTc_HashTable.remove(hash_table, elementa), false);
    assert_int_equal(RESTc_HashTable.remove(hash_table, elementb), true);
    helementa = (struct test_value_object const*) RESTc_HashTable.find(hash_table, elementa);
    helementb = (struct test_value_object const*) RESTc_HashTable.find(hash_table, elementb);
    helementc = (struct test_value_object const*) RESTc_HashTable.find(hash_table, elementc);
    helementd = (struct test_value_object const*) RESTc_HashTable.find(hash_table, elementd);
    assert_null(helementa);
    assert_null(helementb);
    assert_null(helementc);
    assert_null(helementd);
    RESTc_delete(elementa);
    RESTc_delete(elementb);
    RESTc_delete(elementc);
    RESTc_delete(elementd);
    RESTc_delete(hash_table);
}

static void test_rehash(void ** UNUSED(state))
{
    void *hash_table = RESTc_new(RESTc_HashTable, 2);
    assert_non_null(hash_table);
    struct test_value_object *elementf = (struct test_value_object *) RESTc_new(_test_value_object, 7, 7);
    struct test_value_object *elementa = (struct test_value_object *) RESTc_new(_test_value_object, 2, 2);
    struct test_value_object *elementb = (struct test_value_object *) RESTc_new(_test_value_object, 4, 4);
    struct test_value_object *elementc = (struct test_value_object *) RESTc_new(_test_value_object, 8, 8);
    struct test_value_object *elementd = (struct test_value_object *) RESTc_new(_test_value_object, 16, 16);
    struct test_value_object *elemente = (struct test_value_object *) RESTc_new(_test_value_object, 1, 1);
    assert_non_null(elementa);
    assert_non_null(elementb);
    assert_non_null(elementc);
    assert_non_null(elementd);
    assert_non_null(elemente);
    assert_non_null(elementf);
    assert_int_equal(RESTc_HashTable.insert(hash_table, elementa), true);
    assert_int_equal(RESTc_HashTable.insert(hash_table, elementb), true);
    assert_int_equal(RESTc_HashTable.insert(hash_table, elementc), true);
    assert_int_equal(RESTc_HashTable.insert(hash_table, elementd), true);
    assert_int_equal(RESTc_HashTable.insert(hash_table, elemente), true);
    assert_int_equal(RESTc_HashTable.insert(hash_table, elementf), true);
    struct test_value_object const* helementa = (struct test_value_object const*) RESTc_HashTable.find(hash_table, elementa);
    struct test_value_object const* helementb = (struct test_value_object const*) RESTc_HashTable.find(hash_table, elementb);
    struct test_value_object const* helementc = (struct test_value_object const*) RESTc_HashTable.find(hash_table, elementc);
    struct test_value_object const* helementd = (struct test_value_object const*) RESTc_HashTable.find(hash_table, elementd);
    struct test_value_object const* helemente = (struct test_value_object const*) RESTc_HashTable.find(hash_table, elemente);
    struct test_value_object const* helementf = (struct test_value_object const*) RESTc_HashTable.find(hash_table, elementf);
    assert_non_null(helementa);
    assert_non_null(helementb);
    assert_non_null(helementc);
    assert_non_null(helementd);
    assert_non_null(helemente);
    assert_non_null(helementf);
    assert_true(elementa != helementa);
    assert_true(elementb != helementb);
    assert_true(elementc != helementc);
    assert_true(elementd != helementd);
    assert_true(elemente != helemente);
    assert_true(elementf != helementf);
    assert_int_equal(elementa->value, helementa->value);
    assert_int_equal(elementb->value, helementb->value);
    assert_int_equal(elementc->value, helementc->value);
    assert_int_equal(elementd->value, helementd->value);
    assert_int_equal(elemente->value, helemente->value);
    assert_int_equal(elementf->value, helementf->value);
    RESTc_delete(elementa);
    RESTc_delete(elementb);
    RESTc_delete(elementc);
    RESTc_delete(elementd);
    RESTc_delete(elemente);
    RESTc_delete(elementf);
    RESTc_delete(hash_table);
}

int main(void)
{
    struct CMUnitTest const tests[] = {
        cmocka_unit_test(test_insert),
        cmocka_unit_test(test_remove),
        cmocka_unit_test(test_rehash),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
