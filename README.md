# Tether

## WIP

### Eng:
A minimalist and fast system information fetching tool for Linux written in pure C. It has zero heavy dependencies and gathers system metrics directly via core kernel APIs and parsing files in '/proc'.

## Monitored metrics
* Operating system / Linux distribution name.
* Used and total RAM.
* Used and total storage space on the root partition ('/').
* Username
* CPU name

## Installation & Usage

To compile the project, you will need a standard C compiler (gcc or clang).

### Compile manually:
```bash
gcc main.c -o tether
```

### Rus:
Минималистичный и быстрый инструмент для вывода базовой информации о Linux системе, написанный на чистом C. Программа не имеет тяжелых зависимостей и собирает данные напрямую через системные вызовы ядра и парсинг файлов в '/proc'.

## Что показывает
* Название ОС / Linux дистрибутива.
* Размер занятой и всей оперативной памяти.
* Занятое и общее пространство на корневом разделе диска ('/').
* Имя пользователя.
* Название процессора.

## Сборка и запуск

Для компиляции вам понадобится любой стандартный компилятор C (gcc or clang).

### Компиляция вручную:
```bash
gcc main.c -o tether
```