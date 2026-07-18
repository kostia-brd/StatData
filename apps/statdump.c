#include <stdio.h>
#include <stdlib.h>

#include "dump.h"
#include "join.h"
#include "sort.h"
#include "util.h"

int main(int argc, char *argv[])
{
    if (argc != 4)
    {
        fprintf(stderr,
                "Usage: %s <input1> <input2> <output>\n",
                argv[0]);
        return EXIT_FAILURE;
    }

    StatData *left = NULL;
    StatData *right = NULL;
    StatData *result = NULL;

    size_t leftCount = 0;
    size_t rightCount = 0;
    size_t resultCount = 0;

    SDResult rc;

    rc = LoadDump(argv[1], &left, &leftCount);
    if (rc != SD_OK)
    {
        fprintf(stderr,
                "LoadDump('%s') failed: %s\n",
                argv[1],
                SDResultString(rc));
        goto cleanup;
    }

    rc = LoadDump(argv[2], &right, &rightCount);
    if (rc != SD_OK)
    {
        fprintf(stderr,
                "LoadDump('%s') failed: %s\n",
                argv[2],
                SDResultString(rc));
        goto cleanup;
    }

    rc = JoinDump(
        left,
        leftCount,
        right,
        rightCount,
        &result,
        &resultCount);

    if (rc != SD_OK)
    {
        fprintf(stderr,
                "JoinDump() failed: %s\n",
                SDResultString(rc));
        goto cleanup;
    }

    SortDump(result, resultCount);

    PrintTable(result, resultCount, 10);

    rc = StoreDump(
        argv[3],
        result,
        resultCount);

    if (rc != SD_OK)
    {
        fprintf(stderr,
                "StoreDump('%s') failed: %s\n",
                argv[3],
                SDResultString(rc));
        goto cleanup;
    }

cleanup:

    free(left);
    free(right);
    free(result);

    return rc == SD_OK
               ? EXIT_SUCCESS
               : EXIT_FAILURE;
}