#ifndef UTIL_H
#define UTIL_H

#include <stddef.h>

#include "statdata.h"

void PrintRecord(
    const StatData *record);

void PrintTable(
    const StatData *data,
    size_t count,
    size_t limit);

void ModeToBinary(
    unsigned int mode,
    char out[4]);

int EqualRecord(
    const StatData *a,
    const StatData *b);

#endif