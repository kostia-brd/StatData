#ifndef DUMP_H
#define DUMP_H

#include <stddef.h>

#include "error.h"
#include "statdata.h"

/*
 * Сохраняет массив в бинарный файл.
 */

SDResult StoreDump(
    const char *filename,
    const StatData *data,
    size_t count);

/* Загружает массив из бинарного файла.
 *
 * Память выделяется внутри функции.
 * Пользователь обязан вызвать free().
 */

SDResult LoadDump(
    const char *filename,
    StatData **data,
    size_t *count);
#endif