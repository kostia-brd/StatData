#include <stdio.h>
#include <stdlib.h>

#include "test_cases.h"
#include "test_utils.h"

static void RunAllTests(void);

int main(void)
{
    uint64_t start = GetTimeNs();

    RunAllTests();

    uint64_t end = GetTimeNs();

    CleanupStressTest(
        &test_cases[test_cases_count - 1]);

    printf("\n");
    printf("========================================\n");
    printf("Test summary\n");
    printf("========================================\n");

    printf("Total tests: %zu\n",
           test_cases_count);

    printf("Passed: %zu\n",
           GetPassedCount());

    printf("Failed: %zu\n",
           GetFailedCount());

    printf("Total time: %.3f ms\n\n",
           (double)(end - start) / 1000000.0);

    if (GetFailedCount() != 0)
    {
        fprintf(stderr,
                "Some tests failed\n");

        return EXIT_FAILURE;
    }

    printf("All tests passed\n");

    return EXIT_SUCCESS;
}

static void RunAllTests(void)
{
    size_t stress_number = test_cases_count - 1;
    int stress_prepared = PrepareStressTest(
        &test_cases[stress_number]);

    for (size_t i = 0; i < test_cases_count; i++)
    {
        uint64_t start = GetTimeNs();

        int result = RunTest(&test_cases[i]);

        uint64_t end = GetTimeNs();

        if (i == stress_number && !stress_prepared)
            result = 0;

        if (result)
        {
            printf("[PASS] %-32s %.3f ms\n\n",
                   test_cases[i].name,
                   (double)(end - start) / 1000000.0);

            IncrementPassed();
        }
        else
        {
            fprintf(stderr,
                    "[FAIL] %-32s %.3f ms\n",
                    test_cases[i].name,
                    (double)(end - start) / 1000000.0);

            IncrementFailed();
        }
    }
}