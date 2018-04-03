/**
 * \file test_framework.h
 * \brief Déclaration des types et macro pour le micro framework de tests
 */

#ifndef TEST_FRAMEWORK_H
#define TEST_FRAMEWORK_H

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

/// Définition du type de fonction de test.
typedef int (*test_f)();

/// \struct test_t Définition d'un test.
typedef struct {
	char *name; /*!< Nom du test */
	test_f test; /*!< Fonction d'évaluation du test*/
} test_t;

/// \brief Pointeur vers les tests
extern test_t *tests;
/// \brief Nombre de tests
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
