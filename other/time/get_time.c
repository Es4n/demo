#include <stdio.h>
#include <time.h>

/* 获取当前时间
 * */

int main() {
    time_t current_time;
    struct tm * time_info;
    char time_string[40];

    time(&current_time);
    time_info = localtime(&current_time);

    strftime(time_string, sizeof(time_string), "%Y-%m-%d %H:%M:%S", time_info);
    printf("当前时间：%s\n", time_string);

    return 0;
}

