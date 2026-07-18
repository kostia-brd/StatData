#ifndef STATDATA_H
#define STATDATA_H

typedef struct StatData
{
    long id;
    int count;
    float cost;

    unsigned int primary : 1;
    unsigned int mode : 3;

} StatData;
#endif
