#include <stdlib.h>

#include "test_cases.h"
#include "sort.h"

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))

static const StatData case_1_in_a[] =
    {
        {.id = 90889,
         .count = 13,
         .cost = 3.567,
         .primary = 0,
         .mode = 3},

        {.id = 90089,
         .count = 1,
         .cost = 88.90,
         .primary = 1,
         .mode = 0}};

static const StatData case_1_in_b[] =
    {
        {.id = 90089,
         .count = 13,
         .cost = 0.011,
         .primary = 0,
         .mode = 2},

        {.id = 90189,
         .count = 1000,
         .cost = 1.00003,
         .primary = 1,
         .mode = 2}};

static const StatData case_1_out[] =
    {
        {.id = 90189,
         .count = 1000,
         .cost = 1.00003,
         .primary = 1,
         .mode = 2},

        {.id = 90889,
         .count = 13,
         .cost = 3.567,
         .primary = 0,
         .mode = 3},

        {.id = 90089,
         .count = 14,
         .cost = 88.911,
         .primary = 0,
         .mode = 2}};

static const StatData single_record[] =
    {
        {.id = 1,
         .count = 10,
         .cost = 5.5,
         .primary = 1,
         .mode = 1}};

static const StatData duplicate_id_input[] =
    {
        {.id = 100,
         .count = 1,
         .cost = 1.0,
         .primary = 1,
         .mode = 1},

        {.id = 100,
         .count = 2,
         .cost = 2.0,
         .primary = 0,
         .mode = 3}};

static const StatData duplicate_id_output[] =
    {
        {.id = 100,
         .count = 3,
         .cost = 3.0,
         .primary = 0,
         .mode = 3}};

TestCase test_cases[] =
    {
        {.name = "general_test",

         .inputA = case_1_in_a,
         .inputACount = ARRAY_SIZE(case_1_in_a),

         .inputB = case_1_in_b,
         .inputBCount = ARRAY_SIZE(case_1_in_b),

         .expected = case_1_out,
         .expectedCount = ARRAY_SIZE(case_1_out)},

        {.name = "empty_arrays",

         .inputA = NULL,
         .inputACount = 0,

         .inputB = NULL,
         .inputBCount = 0,

         .expected = NULL,
         .expectedCount = 0},

        {.name = "single_record",

         .inputA = single_record,
         .inputACount = 1,
         
         .inputB = NULL,
         .inputBCount = 0,

         .expected = single_record,
         .expectedCount = 1},

        {.name = "duplicate_id",

         .inputA = single_record,
         .inputACount = 0,

         .inputB = duplicate_id_input,
         .inputBCount = ARRAY_SIZE(duplicate_id_input),

         .expected = duplicate_id_output,
         .expectedCount = ARRAY_SIZE(duplicate_id_output)},

        {.name = "stress_test",

         .inputA = NULL,
         .inputACount = 0,

         .inputB = NULL,
         .inputBCount = 0,

         .expected = NULL,
         .expectedCount = 0}};

size_t test_cases_count = ARRAY_SIZE(test_cases);

int NextRandom()
{
    static int state = 123456789;
    state ^= state << 13;
    state ^= state >> 17;
    state ^= state << 5;
    return state;
}

int PrepareStressTest(
    TestCase *test)
{
    const size_t count = 50000;

    StatData *inputA =
        malloc(sizeof(StatData) * count);

    StatData *inputB =
        malloc(sizeof(StatData) * count);

    StatData *output =
        malloc(sizeof(StatData) * count * 2);

    if (!inputA || !inputB)
    {
        free(inputA);
        free(inputB);
        free(output);

        return 0;
    }

    for (size_t i = 0; i < count; i++)
    {
        inputA[i].id = i;
        inputA[i].count = NextRandom() % 1000;
        inputA[i].cost = (float)NextRandom() / 1000000.0f;
        inputA[i].primary = NextRandom() & 1;
        inputA[i].mode = NextRandom() & 7;

        inputB[i].id = (i + 1) << 35;
        inputB[i].count = NextRandom() % 1000;
        inputB[i].cost = (float)NextRandom() / 1000000.0f;
        inputB[i].primary = NextRandom() & 1;
        inputB[i].mode = NextRandom() & 7;

        output[2 * i] = inputA[i];
        output[2 * i + 1] = inputB[i];
    }

    test->inputA = inputA;
    test->inputACount = count;

    test->inputB = inputB;
    test->inputBCount = count;

    SortDump(output, count * 2);
    test->expected = output;
    test->expectedCount = count * 2;

    return 1;
}

void CleanupStressTest(
    TestCase *test)
{
    free((void *)test->inputA);
    free((void *)test->inputB);
    free((void *)test->expected);

    test->inputA = NULL;
    test->inputB = NULL;
    test->expected = NULL;
}