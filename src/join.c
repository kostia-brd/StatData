#include "join.h"

#include <stdlib.h>
#include <string.h>

typedef struct
{
    int used;
    StatData value;
} HashNode;

static size_t NextPowerOf2(size_t n)
{
    size_t p = 1;

    while (p < n)
        p <<= 1;

    return p;
}

static size_t Hash(long key)
{
    size_t x = key;

    x ^= x >> 33;
    x *= 0xff51afd7ed558ccdULL;
    x ^= x >> 33;
    x *= 0xc4ceb9fe1a85ec53ULL;
    x ^= x >> 33;

    return x;
}

static void MergeRecord(StatData *dst, const StatData *src)
{
    dst->count += src->count;
    dst->cost += src->cost;
    dst->primary &= src->primary;

    if (src->mode > dst->mode)
        dst->mode = src->mode;
}

static void Insert(
    HashNode *table,
    size_t mask,
    const StatData *rec)
{
    size_t idx = Hash(rec->id) & mask;

    while (table[idx].used)
    {
        if (table[idx].value.id == rec->id)
        {
            MergeRecord(&table[idx].value, rec);
            return;
        }

        idx = (idx + 1) & mask;
    }

    table[idx].used = 1;
    table[idx].value = *rec;
}

SDResult JoinDump(
    const StatData *left,
    size_t leftCount,
    const StatData *right,
    size_t rightCount,
    StatData **result,
    size_t *resultCount)
{
    if (result == NULL || resultCount == NULL)
        return SD_ERR_ARGUMENT;

    *result = NULL;
    *resultCount = 0;

    if ((left == NULL && leftCount) ||
        (right == NULL && rightCount))
        return SD_ERR_ARGUMENT;

    size_t total = leftCount + rightCount;

    if (total == 0)
        return SD_OK;

    size_t capacity = NextPowerOf2(total * 2);

    HashNode *table = calloc(capacity, sizeof(HashNode));

    if (table == NULL)
        return SD_ERR_MEMORY;

    size_t mask = capacity - 1;

    for (size_t i = 0; i < leftCount; i++)
        Insert(table, mask, &left[i]);

    for (size_t i = 0; i < rightCount; i++)
        Insert(table, mask, &right[i]);

    size_t unique = 0;

    for (size_t i = 0; i < capacity; i++)
        if (table[i].used)
            unique++;

    StatData *out = malloc(unique * sizeof(StatData));

    if (out == NULL)
    {
        free(table);
        return SD_ERR_MEMORY;
    }

    size_t pos = 0;

    for (size_t i = 0; i < capacity; i++)
    {
        if (!table[i].used)
            continue;

        out[pos++] = table[i].value;
    }

    free(table);

    *result = out;
    *resultCount = unique;

    return SD_OK;
}