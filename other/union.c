#include <stdio.h>

/* 联合体的用法--局部操作数据
 * 定义一个联合体,大小为2字节(short int)
 * 联合体中有一个结构体,包含两个char成员(必须是结构体,如果直接定义两个char,两个char的内存布局会重叠)
 * 因此该联合体的内存布局为:
 *
 * |----|
 * |char|\
 * |----|  short int
 * |char|/
 * |----|
 *
 * 因此该联合体成了一个灵活的数据类型:
 * 既然可以整体操作,也可以局部操作字节,就像寄存器可以分级使用一样
 * */

typedef union test{
    short int test;
    struct {
        char test_low;
        char test_high;
    };
}test_union;

int main()
{
    test_union test;

    //整体操作联合体
    test.test = 1;
    printf("test.test = 1;\n");
    printf("test_low %d,test_high %d\n",test.test_low,test.test_high);

    test.test = 0;
    printf("\n");

    //局部操作联合体
    test.test_low = -1;
    printf("test.test_low = -1;\n");
    printf("test %d\n",test.test);

    return 0;
}
