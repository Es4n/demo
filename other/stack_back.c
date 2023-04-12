#include <stdio.h>

/* 通过对栈的理解,从函数内推算出函数外的变量的地址
 * 访问并改变该变量
 * 具体:
 * 在main里面定义一个变量,在fun函数里面推算出该变量在栈内的地址
 * */

void fun(int a)
{
    //保存main rbp ,rsp - 8

    //自此推算出mian函数的a变量的地址,就是main的初始下移16字节,call下移8字节,保存main rbp8字节,就是要上移动4 * 8字节
    printf("%d\n",*((char *)(&a) + 8 + 8 + 8 + 8));
    *((char *)(&a) + 8 + 8 + 8 + 8) = 99;
}

int main()
{
    //起始栈顶rsp下移0x10字节

    //a的起始地址main rbp
    int a = 100;

    //call指令,保存8字节的下一条指令,rsp - 8
    fun(1);

    printf("%d\n",a);

    return 0;
}
