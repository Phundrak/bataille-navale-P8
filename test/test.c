/**
 * \file test.c
 * \brief Micro framework de test
 */
#include <test_framework.h>

/// \brief Pointeurs vers les tests
test_t *tests;
/// \brief Nombre de tests
int ntests = 0;

/**
 * `run_single_test` exécute un test passé en paramètre
 * \return 1 si le test a réussi, 0 sinon.
 */
int run_single_test(test_t t) {
	printf("Running test %s...\n", t.name);
	int s = t.test();
	printf("Test %s:\t\t%s\033[0m\n", t.name, s ? "\033[32msuccess" : "\033[31mfailed");
	return s;
}

/**
 * `run_all_tests` exécute tout les tests.
 * \return 1 si tout les tests ont réussi, 0 sinon.
 */
int run_all_tests() {
	int passed = 0;
	for (int i = 0; i < ntests; ++i) {
		passed += run_single_test(tests[i]);
	}
	printf("%d tests passed out of %d tests\n", passed, ntests);
	return passed == ntests;
}

/**
 * `find_test` renvoie un pointeur vers un test à partir de son nom
 * ou nul si non-trouvé
 * \param n Nom du test.
 * \return Pointeur vers le test recherché, ou nul
 */
test_t *find_test(char *n) {
	for (int i = 0; i < ntests; ++i) {
		if (strcmp(n, tests[i].name) == 0) {
			return tests + i;
		}
	}
	return 0;
}

/**
 * Point d'entrée du programme.
 * L'option -l permet de lister les tests disponible
 * Sans argument: exécute tout les tests disponibles.
 * Si plusieurs arguments: exécute tout les tests dont les noms correspondent aux
 * arguments
 * \return 0 si tout s'est bien passé, 1 si un test a échoué
 */
int main(int argc, char *argv[argc])
{
	if (argc > 1) {
		if (strcmp(argv[1], "-l") == 0) {
			for (int i = 0; i < ntests; ++i) {
				printf("%s ", tests[i].name);
			}
			puts("");
			return 0;
		}
		int total = 0;
		int passed = 0;
		for (int i = 1; i < argc; ++i) {
			test_t *t = find_test(argv[i]);
			if (!t)
				printf("Test '%s' not found, skipping...\n", argv[i]);
			else {
				total++;
				passed += run_single_test(*t);
			}
			printf("%d tests passed out of %d tests\n", passed, ntests);
			return passed != ntests;
		}
	} else
		return run_all_tests();
	return 0;
}
