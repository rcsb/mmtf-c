#include <stdio.h>
#include <stdlib.h>
#include <check.h>
#include <stdbool.h>

#include "../src/num_converters.c"

// Test the add_one function from num_converters.c
START_TEST (test_add_one)
{
    int x = add_one(1);
    ck_assert_msg(x == 2, "1+1 doesn't equal 2");
}
END_TEST

// Test the add_two function from array_decoders.c
START_TEST (test_add_two)
{
    int x = add_two(1);
    ck_assert_msg(x == 3, "1+2 doesn't equal 3");
}
END_TEST

Suite* whatever_suite (void) {
        Suite *suite;
        TCase *tc1;
        TCase *tc2;
        
        suite = suite_create("Whatever"); 

        tc1 = tcase_create("group_one");
        tcase_add_test(tc1, test_add_one);
        suite_add_tcase(suite, tc1);

        tc2 = tcase_create("group_two");
        tcase_add_test(tc2, test_add_two);
        suite_add_tcase(suite, tc2);

        return suite;
}

int main (int argc, char *argv[]) {
        int number_failed;

        Suite *suite;
        SRunner *sr;

        suite = whatever_suite();
        sr = srunner_create(suite);

        srunner_run_all(sr, CK_VERBOSE);
        number_failed = srunner_ntests_failed(sr);
        srunner_free(sr);
        return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
