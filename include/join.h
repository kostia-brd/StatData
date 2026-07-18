#ifndef JOIN_H
#define JOIN_H

#include <stddef.h>

#include "error.h"
#include "statdata.h"

SDResult JoinDump(
    const StatData *left,
    size_t leftCount,

    const StatData *right,
    size_t rightCount,

    StatData **result,
    size_t *resultCount);

#endif