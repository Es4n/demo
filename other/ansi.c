#include <stdio.h>
#include <unistd.h>

/* 本例子示范了一个使用ANSI的例子
 * 先输出字符串"wait"
 * 3秒后删除"wait",并重新在本行输出"yes"
 * */

int main()
{
    //输出字符串“wait”
    printf("wait\n");

    //等待3秒
    for(int i = 0;i < 3;i++)
    {
        sleep(1);
    }

    //输出光标跳转到本行的第0列
    printf("\033[0G");
    //删除光标到本行末尾的内容
    printf("\033[K");
    //将缓冲区的内容输出
    fflush(stdout);
    //在本行重新打印yes
    printf("yes\n");
}
