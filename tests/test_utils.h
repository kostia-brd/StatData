
#ifndef TEST_UTILS_H
#define TEST_UTILS_H

#include <stddef.h>
#include <stdint.h>

#include "test_cases.h"


/*
 * Запуск одного интеграционного теста.
 *
 * Создает входные файлы,
 * запускает утилиту обработки,
 * загружает результат,
 * сравнивает с эталоном.
 */
int RunTest(
    const TestCase *test);


/*
 * Сравнение двух массивов StatData.
 */
int CompareDump(
    const StatData *expected,
    size_t expectedCount,

    const StatData *actual,
    size_t actualCount);


void PrintRecordDiff(
    const StatData *expected,
    const StatData *actual);


uint64_t GetTimeNs(void);

void IncrementPassed(void);
void IncrementFailed(void);
size_t GetPassedCount(void);
size_t GetFailedCount(void);

#endif