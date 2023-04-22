#include <stdio.h>
#include <time.h>

/* 获取当前时间
 * */

int main()
{
    time_t current_time;  // 存放时间戳
    struct tm *time_info; // 时间戳转换结构体
    char time_string[40]; // 输出时间字符串

    time(&current_time);                  // 获取时间戳
    time_info = localtime(&current_time); // 时间戳放入时间戳转换结构体

    // 获得输出字符串
    strftime(time_string, sizeof(time_string), "%Y-%m-%d %H:%M:%S", time_info);
    printf("当前时间：%s\n", time_string);

    return 0;
}
