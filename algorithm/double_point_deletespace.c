#include <stdio.h>
#include <string.h>

/* 双指针应用:删除字符串的指定字符
 * 定义两个指针,一个负责搜索非制定字符,一个负责接受字符
 * 两个指针的跨度就是字符串中制定字符的数量
 * */

int main(int argc, char *argv[])
{
    printf("%s\n", argv[1]);
    char str[100] = {0};

    strcpy(str, argv[1]);

    int p = 0, q = 0;

    //每次转移一个字符,转移后两个指针同时向前移动
    for(;; q++, p++)
    {
        //指针p向前搜索非_的字符
        while(str[p] == '_')
            p++;

        //传输字符
        str[q] = str[p];

        //如果已经到了结尾就结束
        if(str[p] == '\0')
            break;
    }

    printf("%s\n", str);
}
