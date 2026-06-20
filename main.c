// Standart C headers
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// POSIX C headers
#include <sys/utsname.h>
#include <sys/sysinfo.h>
#include <sys/statvfs.h>
#include <unistd.h>

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
            printf("Distro: %s", line + 12); //line + 12 -> мы перешагиваем с PRETTY_NAME= на название дистрибутива
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

    printf("RAM: %llu GB / %llu GB\n", used_gb, total_gb);
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

        printf("SSD: %llu GB / %llu GB\n", used_gb, total_gb);
    }
}

void user() {
    char *username = getlogin(); // getlogin() опрашивает систему и возвращает указатель на строку (char *) с именем пользователя, который управляет текущим терминалом
    if (username != NULL) {
        printf("User: %s\n", username);
    } else {
        printf("User: ???\n");
    }
}

void cpu() {
    FILE *fp = fopen("/proc/cpuinfo", "r");
    if (fp == NULL) return;

    char line[256];
    while (fgets(line, sizeof(line), fp)) {
        // Ищем строку начинающуюся с "model name"
        if (strncmp(line, "model name", 10) == 0) {
            char *cpu_name = strchr(line, ':'); // 1. Где искать 2. Искомый символ
            // strchr() — найти первое вхождение определенного символа в строке
            // *cpu_name указывает именно на этот символ в строке line
            if (cpu_name != NULL) { // Если функция strchr обыщет всю строку и не найдет двоеточие, она вернет пустоту — NULL
                cpu_name += 2; // выводим название без двоеточия и пробела
                printf("CPU: %s", cpu_name); 
                break;
            }
        }
    }
    fclose(fp);
}

void gpu() {
    // work in progress
}

void shell() {
    char *shell_path = getenv("SHELL"); // Функция getenv() вернет полный путь, например: "/bin/bash" или "/usr/bin/zsh"

    if (shell_path != NULL) {
        char *shell_name = strrchr(shell_path, '/');
        // Чтобы не выводить длинный путь "/usr/bin/zsh" или "/bin/bash",
        // мы можем найти самый последний слеш '/' в строке с помощью функции strrchr()

        if (shell_name != NULL) {
            printf("Shell: %s\n", shell_name + 1);
        } else {
            printf("Shell: %s\n", shell_path);
        }
    }
}

void hostname() {
    struct utsname host_info;

    if (uname(&host_info) == 0) {
        printf("Host: %s\n", host_info.nodename);
        // Когда ты вызываешь функцию uname(&host_info),
        // ядро Linux берет имя твоего ПК (которое ты можешь настроить в файле /etc/hostname)
        // и копирует эти буквы в массив host_info.nodename.
    }
}

int main() {
    os();
    ram();
    memory();
    user();
    cpu();
    // gpu();
    shell(); // works wrong
    hostname();
    return 0;
}