#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* 本demo展现了#的使用以及sizeof的特性
 *
 * 1.在宏定义函数Printf中,用#x创建了一个字符串
 * 2.除了数组和字符串常量外,sizeof计算的都是变量的大小


 * */

//用#x可以创建字符串
#define Printf(x) printf("the "#x" sizeof is %ld\n",sizeof(x))

int main()
{
    int size_int;
    char size_char;
    long int size_long_int;
    char size_str[50];
    int *size_int_malloc = (int *)malloc(40);
    char *size_char_malloc = (char *)malloc(10);

    strcpy(size_char_malloc,"123456789");
    printf("%s\n",size_char_malloc);

    Printf(size_int);
    Printf(&size_int);  //&size_int也会被算作指针
    Printf(size_char);
    Printf(size_long_int);
    Printf(size_str);
    Printf(size_int_malloc);
    Printf(size_char_malloc);
    Printf("123456789");

    return 0;
}
