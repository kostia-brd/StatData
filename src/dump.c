#include "dump.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/*
здесь можно обсудить какие-либо сжатия,
но конкретизация long и битовых полей обязательна,
а также желателен паддинг (безопасный для arm).
typedef struct StatData {
long  id;                   -> uint64_t
int   count;                -> uint32_t
float cost;                 -> float
unsigned int primary:1;    | оба битовых поля объединяем в uint8_t
unsigned int mode:3; 	   |
} StatData;
*/

static const size_t rec_size =
    sizeof(int64_t) +
    sizeof(int32_t) +
    sizeof(float) +
    sizeof(uint8_t);

typedef uint64_t CountType;

/*
выделить FillBuffer в отдельную функцию это не только удобно но и полезно,
например если захотим отправлять по сети и т.п.
*/
SDResult FillBuffer(
    const StatData *data,
    size_t count,
    void *buffer,
    size_t size)
{
    if (count && (data == NULL || buffer == NULL))
        return SD_ERR_ARGUMENT;

    if (count > size / rec_size)
        return SD_ERR_ARGUMENT;

    size_t offset = 0;
    uint8_t *ptr = buffer;

    for (size_t i = 0; i < count; i++)
    {
        int64_t id = data[i].id;
        memcpy(ptr + offset, &id, sizeof(id));
        offset += sizeof(id);

        int32_t count = data[i].count;
        memcpy(ptr + offset, &count, sizeof(count));
        offset += sizeof(count);

        memcpy(ptr + offset, &data[i].cost, sizeof(float));
        offset += sizeof(float);

        uint8_t flags = data[i].primary | (data[i].mode << 1);
        memcpy(ptr + offset, &flags, sizeof(flags));
        offset += sizeof(flags);
    }

    return SD_OK;
}

SDResult StoreDump(
    const char *filename,
    const StatData *data,
    size_t count)
{
    if (filename == NULL)
        return SD_ERR_ARGUMENT;

    if (count && data == NULL)
        return SD_ERR_ARGUMENT;

    FILE *fp = fopen(filename, "wb");

    if (fp == NULL)
        return SD_ERR_IO;

    /*в хедер можно было бы добавить не только колличество записей, но и версию, сигнатуру, хеш файла*/
    CountType cn = count;

    if (fwrite(&cn, sizeof(cn), 1, fp) != 1)
    {
        fclose(fp);
        return SD_ERR_IO;
    }

    if (count == 0)
    {
        fclose(fp);
        return SD_OK;
    }
    size_t buffer_size = count * rec_size;
    void *buffer = malloc(buffer_size);
    if (buffer == NULL)
    {
        fclose(fp);
        return SD_ERR_MEMORY;
    }
    SDResult fillBufferRes = FillBuffer(data, count, buffer, buffer_size);
    if (fillBufferRes != SD_OK)
    {
        free(buffer);
        fclose(fp);
        return fillBufferRes;
    }

    size_t written = fwrite(buffer, sizeof(char), buffer_size, fp);

    free(buffer);
    fclose(fp);

    return (written == buffer_size) ? SD_OK : SD_ERR_IO;
}

SDResult ReadBuffer(
    StatData *data,
    size_t count,
    void *buffer,
    size_t size)
{
    if (count && (buffer == NULL || data == NULL))
        return SD_ERR_ARGUMENT;

    if (count > size / rec_size)
        return SD_ERR_ARGUMENT;

    size_t offset = 0;
    const uint8_t *ptr = buffer;

    for (size_t i = 0; i < count; i++)
    {
        int64_t id;
        memcpy(&id, ptr + offset, sizeof(id));
        data[i].id = id;
        offset += sizeof(id);

        int32_t count;
        memcpy(&count, ptr + offset, sizeof(count));
        data[i].count = count;
        offset += sizeof(count);

        memcpy(&data[i].cost, ptr + offset, sizeof(float));
        offset += sizeof(float);

        uint8_t flags;
        memcpy(&flags, ptr + offset, sizeof(flags));
        offset += sizeof(flags);

        data[i].primary = flags & 0x01;
        data[i].mode = (flags >> 1) & 0x07;
    }

    return SD_OK;
}

SDResult LoadDump(
    const char *filename,
    StatData **data,
    size_t *count)
{
    if (filename == NULL || data == NULL || count == NULL)
        return SD_ERR_ARGUMENT;

    *data = NULL;
    *count = 0;

    FILE *fp = fopen(filename, "rb");

    if (fp == NULL)
        return SD_ERR_IO;

    CountType cn;

    if (fread(&cn, sizeof(cn), 1, fp) != 1)
    {
        fclose(fp);
        return SD_ERR_IO;
    }

    size_t buffer_size = cn * rec_size;

    void *buffer = malloc(buffer_size);

    if (buffer == NULL)
    {
        fclose(fp);
        return SD_ERR_MEMORY;
    }

    size_t read = fread(buffer, sizeof(char), buffer_size, fp);

    fclose(fp);

    if (read != buffer_size)
    {
        free(buffer);
        return SD_ERR_IO;
    }

    *data = malloc(sizeof(StatData) * cn);

    if (*data == NULL)
    {
        free(buffer);
        return SD_ERR_MEMORY;
    }

    SDResult result = ReadBuffer(
        *data,
        cn,
        buffer,
        buffer_size);

    free(buffer);

    if (result != SD_OK)
    {
        free(*data);
        *data = NULL;
        return result;
    }
    *count = cn;

    return SD_OK;
}