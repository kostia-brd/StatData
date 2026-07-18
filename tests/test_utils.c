
#include "test_utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "statdata.h"
#include "dump.h"
#include "util.h"
#include "error.h"

#define PROCESSING_UTILITY "./statdump"

static size_t passedCount = 0;
static size_t failedCount = 0;

static void MakeFileName(
    char *buffer,
    size_t size,
    const char *testName,
    const char *suffix)
{
    snprintf(buffer,
             size,
             "%s_%s_%s",
             testName,
             suffix,
             ".bin");
}

uint64_t GetTimeNs(void)
{
    struct timespec ts;

    clock_gettime(
        CLOCK_MONOTONIC,
        &ts);

    return (uint64_t)ts.tv_sec * 1000000000ULL +
           (uint64_t)ts.tv_nsec;
}

void IncrementPassed(void)
{
    ++passedCount;
}

void IncrementFailed(void)
{
    ++failedCount;
}

size_t GetPassedCount(void)
{
    return passedCount;
}

size_t GetFailedCount(void)
{
    return failedCount;
}

int RunTest(
    const TestCase *test)
{
    char fileA[256];
    char fileB[256];
    char fileOut[256];

    MakeFileName(
        fileA,
        sizeof(fileA),
        test->name,
        "input_a");

    MakeFileName(
        fileB,
        sizeof(fileB),
        test->name,
        "input_b");

    MakeFileName(
        fileOut,
        sizeof(fileOut),
        test->name,
        "output");

    SDResult result;

    result = StoreDump(
        fileA,
        test->inputA,
        test->inputACount);

    if (result != SD_OK)
    {
        fprintf(stderr,
                "%s: StoreDump(A) failed: %s\n",
                test->name,
                SDResultString(result));

        remove(fileA);

        return 0;
    }

    result = StoreDump(
        fileB,
        test->inputB,
        test->inputBCount);

    if (result != SD_OK)
    {
        fprintf(stderr,
                "%s: StoreDump(B) failed: %s\n",
                test->name,
                SDResultString(result));

        remove(fileA);
        remove(fileB);

        return 0;
    }

    char command[1024];

    snprintf(
        command,
        sizeof(command),
        "%s %s %s %s",
        PROCESSING_UTILITY,
        fileA,
        fileB,
        fileOut);

    int rc = system(command);

    if (rc != 0)
    {
        fprintf(stderr,
                "%s: utility execution failed\n",
                test->name);

        remove(fileA);
        remove(fileB);
        remove(fileOut);

        return 0;
    }

    StatData *actual = NULL;
    size_t actualCount = 0;

    result = LoadDump(
        fileOut,
        &actual,
        &actualCount);

    if (result != SD_OK)
    {
        fprintf(stderr,
                "%s: LoadDump failed: %s\n",
                test->name,
                SDResultString(result));

        remove(fileA);
        remove(fileB);
        remove(fileOut);

        return 0;
    }

    int equal = CompareDump(
        test->expected,
        test->expectedCount,
        actual,
        actualCount);

    if (!equal)
    {
        fprintf(stderr,
                "Test failed: %s\n",
                test->name);
    }

    free(actual);

    remove(fileA);
    remove(fileB);
    remove(fileOut);

    return equal;
}

int CompareDump(
    const StatData *expected,
    size_t expectedCount,
    const StatData *actual,
    size_t actualCount)
{
    if (expectedCount != actualCount)
    {
        fprintf(stderr,
                "Record count mismatch\n"
                "Expected: %zu\n"
                "Actual:   %zu\n",
                expectedCount,
                actualCount);

        return 0;
    }

    for (size_t i = 0; i < expectedCount; i++)
    {
        if (!EqualRecord(
                &expected[i],
                &actual[i]))
        {
            fprintf(stderr,
                    "Record mismatch at index %zu\n",
                    i);

            PrintRecordDiff(
                &expected[i],
                &actual[i]);

            return 0;
        }
    }

    return 1;
}

void PrintRecordDiff(
    const StatData *expected,
    const StatData *actual)
{
    fprintf(stderr,
            "\nExpected:\n");

    PrintRecord(expected);

    fprintf(stderr,
            "\nActual:\n");

    PrintRecord(actual);
}