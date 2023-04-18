#include <stdarg.h>  //可变参数的宏在这个头文件里面
#include <stdio.h>

/* 编写可变参数函数
 * 可变参数用栈来实现,用宏来使用
 *
 * 该例子是一个可以打印可变数量的字符串
 * */

void fun(int a, ...)  //虽然参数可变,但固定必须要有一个参数
{
    va_list arg;  //栈顶指针
    va_start(arg, a);  //使栈顶指针指向第一个参数(固定参数)的下一个参数

    char *str;
    while((str = va_arg(arg, char *)) != NULL)  //用va_arg来返回当前的值,并使栈顶指针指向下一个参数
    //va_arg的第一个参数指针是栈顶指针,第二个参数是数据类型,因为是宏定义直接替换,所以可以用char*作为参数
    {
        printf("%s\n",str);
    }

    va_end(arg);  //规定可变参数函数返回前必须调用va_end
}

int main(int argc,char *argv[])
{
    char *str1 = "12345";
    char *str2 = "12agds345";
    char *str3 = "oiuo12agds345";

    //调用可变参数
    fun(1,str1,str2,str3);

    return 0;
}
