# StatDump

Тестовое задание по обработке бинарных данных.

## Возможности

- сериализация массива StatData;
- десериализация;
- объединение массивов;
- сортировка;
- тестирование.

## Документация

- [README.md](README.md) — описание проекта, способы сборки и запуска, структура.
- [DESIGN.md](DESIGN.md) — описание принятых решений, выбранных алгоритмов, тестового покрытия и возможных направлений оптимизации.

## Сборка

```bash
mkdir build
cd build
cmake ..
cmake --build .
```

## Запуск

```
./statdump <input1> <input2> <output>
```

## Тесты

```
./test_runner
```

## Тестовая платформа

- WSL Ubuntu 20.04.6 LTS 
- GCC 9.4.0

## Структура

```
├── apps
│   ├── statdump.c
│   └── test_statdump.c
├── include
│   ├── dump.h
│   ├── error.h
│   ├── join.h
│   ├── sort.h
│   ├── statdata.h
│   └── util.h
├── src
│   ├── dump.c
│   ├── error.c
│   ├── join.c
│   ├── sort.c
│   └── util.c
└── tests
    ├── test_cases.c
    ├── test_cases.h
    ├── test_utils.c
    └── test_utils.h
```