#include <linux/sysinfo.h>
#include <stdio.h>
#include <sys/utsname.h>
#include <string.h>
#include <sys/sysinfo.h>
#include <sys/statvfs.h>

void os() {
    FILE *fp = fopen("/etc/os-release", "r"); //fopen - функция открытия файла, *fp - переменная указатель
    char line[256]; //строка 256 символов

    while (fgets(line, sizeof(line), fp)) { //у функции fgets 3 аргумента: 1. массив (line) 2. максимальный размер который можно прочитать за раз (sizeof(line)) 3. откуда читать (fp)
//----------------------------------------------------------------------------------------------------------
//Как это работает в цикле while?
//fgets читает файл построчно. Она берет первую строку, кладет в line, и мы выполняем код внутри цикла.
//На следующей итерации while она затирает старые данные и кладет в line вторую строку.
//Когда файл закончится, fgets вернет NULL (ничего), и цикл while сам остановится.
//----------------------------------------------------------------------------------------------------------
        if (strncmp(line, "PRETTY_NAME=", 12) == 0) { //strncmp (от англ. string number compare) 1. Строка которую мы прочитали 2. Что мы ищем 3. Первые n символов
//----------------------------------------------------------------------------------------------------------
//В файле строка выглядит так: PRETTY_NAME="Arch Linux". Нам не нужно сравнивать её целиком, нам важно узнать:
// начинается ли эта строка с нужного нам маркера? Если первые 12 символов совпали, функция возвращает 0. Именно поэтому мы пишем == 0
//----------------------------------------------------------------------------------------------------------
            printf("Дистрибутив: %s", line + 12); //line + 12 -> мы перешагиваем с PRETTY_NAME= на название дистрибутива
            break;
        }
    }
    fclose(fp); //закрываем поток к файлу
}

void ram() {
    FILE *fp = fopen("/proc/meminfo", "r");
    if (fp == NULL) return;

    char line[256];
    unsigned long long totalkbram = 0;
    unsigned long long freekbram = 0;
    
    while(fgets(line, sizeof(line), fp)) {
        if (strncmp(line, "MemTotal:", 9) == 0) {
            sscanf(line, "%*s %llu", &totalkbram); //"%*s" - выкинь слово до пробела (символ * — это запрет на запись)
            // "%llu" - а вот после прочитай следующее число и запиши его после пробела в &totalkbram
        }
        if (strncmp(line, "MemAvailable:", 13) == 0) {
            sscanf(line, "%*s %llu", &freekbram);
        }
    }
    fclose(fp);

    unsigned long long usedkbram = totalkbram - freekbram;
    unsigned long long total_gb = totalkbram / (1024 * 1024);
    unsigned long long used_gb = usedkbram / (1024 * 1024);

    printf("Оперативная память: %llu GB / %llu GB\n", used_gb, total_gb);
}

void memory() {
    struct statvfs vfs;

    if (statvfs("/", &vfs) == 0) {
        unsigned long long total_bytes = (unsigned long long)vfs.f_frsize * vfs.f_blocks; 
        //vfs.f_frsize — размер одного блока файловой системы в байтах
        //vfs.f_blocks — общее количество таких блоков на диске
        //Приведение типа (unsigned long long) перед vfs.f_frsize нужно обязательно, чтобы при умножении больших чисел у компилятора не сорвало крышу от переполнения
        unsigned long long total_gb = total_bytes / (1024 * 1024 * 1024);

        unsigned long long used_blocks = vfs.f_blocks - vfs.f_bavail; //Находим количество занятых блоков
        unsigned long long used_bytes = used_blocks * vfs.f_frsize; //Переводим занятые блоки в байты, умножая на размер блока
        unsigned long long used_gb = used_bytes / (1024 * 1024 * 1024);

        printf("Диск: %llu GB / %llu GB", used_gb, total_gb);
    }
}

int main() {
    os();
    ram();
    memory();
    return 0;
}