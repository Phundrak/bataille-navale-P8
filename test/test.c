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
int run_single_test(test_t test) {
	printf("Running test %s...\n", test.name);
	int success = test.test();
	printf("Test %s:\t\t%s\033[0m\n", test.name, success ? "\033[32msuccess" : "\033[31mfailed");
	return success;
}

/**
 * `run_all_tests` exécute tout les tests.
 * \return 1 si tout les tests ont réussi, 0 sinon.
 */
int run_all_tests() {
	int passed = 0;
	for (int test_nr = 0; test_nr < ntests; ++test_nr) {
		passed += run_single_test(tests[test_nr]);
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
test_t *find_test(char *test_name) {
	for (int test_nr = 0; test_nr < ntests; ++test_nr) {
		if (strcmp(test_name, tests[test_nr].name) == 0) {
			return tests + test_nr;
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
			for (int test_nr = 0; test_nr < ntests; ++test_nr) {
				printf("%s ", tests[test_nr].name);
			}
			puts("");
			return 0;
		}
		int passed = 0;
		for (int arg_nr = 1; arg_nr < argc; ++arg_nr) {
			test_t *t = find_test(argv[arg_nr]);
			if (!t)
				printf("Test '%s' not found, skipping...\n", argv[arg_nr]);
			else {
				passed += run_single_test(*t);
			}
			printf("%d tests passed out of %d tests\n", passed, ntests);
			return passed != ntests;
		}
	} else
		return run_all_tests();
	return 0;
}
