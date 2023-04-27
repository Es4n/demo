#include <stdio.h>

/* 该例程演示用sprintf将不同类型的数据转换为字符串并且进行拼接
 * */

int main()
{
    int a = 1;
    float b = 3.11111;
    char *c = "abcdefg";

    char str[100] = {0};

    // 将不同类型的数据转换成字符串并拼接
    sprintf(str,"%d,%f,%s\n",a,b,c);

    printf("%s\n",str);

    return 0;
}
