#include <stdio.h>
#include <time.h>
#include <unistd.h>

/* 计算时间差
 * */

int main() {
    time_t start_time, end_time;  //计算时间差的开始时间和结束时间
    double diff_time;

    // 获取起始时间
    time(&start_time);

    // 延迟3秒,计算时间差
    sleep(3);

    // 获取结束时间
    time(&end_time);

    // 计算时间差
    diff_time = difftime(end_time, start_time);

    printf("时间差为 %.2f 秒\n", diff_time);

    return 0;
}

