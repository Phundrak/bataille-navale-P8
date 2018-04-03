/**
 * \file
 */

#ifndef TEST_FRAMEWORK_H
#define TEST_FRAMEWORK_H

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

typedef int (*test_f)();

typedef struct {
	char *name;
	test_f test;
} test_t;

extern test_t *tests;
extern int ntests;

#define CONSTR __attribute__((constructor))
#define TEST(name)													\
	int test_impl_##name();											\
	CONSTR void test_##name() {											\
		void *d = realloc(tests, (ntests + 1) * sizeof(*tests));		\
		assert(d);														\
		tests = d;														\
		tests[ntests++] = (test_t){#name, &test_impl_##name};			\
	}																\
	int test_impl_##name()											\


#endif /* TEST_FRAMEWORK_H */