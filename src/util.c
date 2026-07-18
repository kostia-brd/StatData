#include "util.h"

#include <math.h>
#include <stdio.h>

void ModeToBinary(unsigned int mode, char out[4])
{
    out[0] = (mode & 0x4) ? '1' : '0';
    out[1] = (mode & 0x2) ? '1' : '0';
    out[2] = (mode & 0x1) ? '1' : '0';
    out[3] = '\0';
}

void PrintRecord(const StatData *record)
{
    char mode[4];

    ModeToBinary(record->mode, mode);

    printf("|%#016lx |%11d |%12.3e |%8c |%5s\n",
           (unsigned long)record->id,
           record->count,
           record->cost,
           record->primary ? 'y' : 'n',
           mode);
}

void PrintTable(
    const StatData *data,
    size_t count,
    size_t limit)
{
    if (data == NULL || count == 0)
    {
        puts("Empty");
        return;
    }

    if (limit > count)
        limit = count;

    printf("|%16s |%11s |%12s |%8s |%5s\n",
           "id",
           "count",
           "cost",
           "primary",
           "mode");

    printf("-------------------------------------------------------------\n");

    for (size_t i = 0; i < limit; i++)
        PrintRecord(&data[i]);

    printf("-------------------------------------------------------------\n");
}

int EqualRecord(
    const StatData *a,
    const StatData *b)
{
    if (a->id != b->id)
        return 0;

    if (a->count != b->count)
        return 0;

    /* в рамках заявленных тестов можно использовать более жёсткое сравнение
    но так как функция логически ближе к утилитам statdata, 
    то реализуем полноценное мягкое сравнение*/
    float diff = fabsf(a->cost - b->cost);
    if ((diff > 1e-6f) && (diff > (1e-6f * fmaxf(fabsf(a->cost), fabsf(b->cost)))) )
        return 0;

    if (a->primary != b->primary)
        return 0;

    if (a->mode != b->mode)
        return 0;

    return 1;
}