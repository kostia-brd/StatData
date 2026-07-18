#ifndef TEST_CASES_H
#define TEST_CASES_H

#include <stddef.h>

#include "statdata.h"
#include "error.h"

typedef struct
{
    const char *name;

    const StatData *inputA;
    size_t inputACount;

    const StatData *inputB;
    size_t inputBCount;

    const StatData *expected;
    size_t expectedCount;

} TestCase;

extern TestCase test_cases[];

extern size_t test_cases_count;

int PrepareStressTest(TestCase *test);
void CleanupStressTest(TestCase *test);

#endif