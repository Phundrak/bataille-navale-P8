#include <test_framework.h>

test_t *tests;
int ntests = 0;

int run_single_test(test_t test) {
	printf("Running test %s...\n", test.name);
	int success = test.test();
	printf("Test %s:\t\t%s\033[0m\n", test.name, success ? "\033[32msuccess" : "\033[31mfailed");
	return success;
}

int run_all_tests() {
	int passed = 0;
	for (int test_nr = 0; test_nr < ntests; ++test_nr) {
		passed += run_single_test(tests[test_nr]);
	}
	printf("%d tests passed out of %d tests\n", passed, ntests);
	return passed == ntests;
}

test_t *find_test(char *test_name) {
	for (int test_nr = 0; test_nr < ntests; ++test_nr) {
		if (strcmp(test_name, tests[test_nr].name) == 0) {
			return tests + test_nr;
		}
	}
	return 0;
}

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
		int total = 0;
		int passed = 0;
		for (int arg_nr = 1; arg_nr < argc; ++arg_nr) {
			test_t *t = find_test(argv[arg_nr]);
			if (!t)
				printf("Test '%s' not found, skipping...\n", argv[arg_nr]);
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
