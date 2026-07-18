#include "sort.h"

#include <stdlib.h>

static int CompareCost(const void *lhs, const void *rhs)
{
    const StatData *a = (const StatData *)lhs;
    const StatData *b = (const StatData *)rhs;

    if (a->cost < b->cost)
        return -1;

    if (a->cost > b->cost)
        return 1;

    //Чтобы сортировка была детерминированной при одинаковом cost,
    //дополнительно сравниваем id.
    if (a->id < b->id)
        return -1;

    if (a->id > b->id)
        return 1;

    return 0;
}

void SortDump(StatData *data, size_t count)
{
    if (data == NULL || count < 2)
        return;

    qsort(
        data,
        count,
        sizeof(StatData),
        CompareCost);
}