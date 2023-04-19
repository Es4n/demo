#include <stdio.h>

/* 该程序实现了十六位二进制数转换十进制压缩BCD码
 *
 * 流程:
 * 1.十六位二进制数放在联合体的最低位
 * 2.联合体逐位右移
 * 3.检查每一个BCD码是否即将溢出,如果是,就要加3,这样在下一次右移后将会将一个BCD码拆分成两个BCD码表示两位数
 * 4.重复上述步骤,直到将十六位二进制数全部移动到BCD码中
 * */

typedef union {
    long int change;
    unsigned short int b_date; // 要转换的二进制数据
    struct {
        unsigned short int nop;       // 留空占位
        unsigned char d_date_1_and_2; // 十进制的低两位
        unsigned char d_date_3_and_4; // 十进制的中两位
        unsigned char d_date_5;       // 十进制的最高位
    };
} change_union;

/* 联合体内存布局如下:
 * -------------------------------------------------------
 * | d_date_5 | d_date_3_and_4 | d_date_1_and_2 | b_date |
 * -------------------------------------------------------
 * \                                                     /
 *   ----------------------change----------------------   
 * */

int main()
{
    change_union test;

    // 赋值要转换的二进制码
    test.b_date = 53241;
    printf("%lx\n", test.change);

    // 逐位右移,并检查每一位BCD码是否即将溢出
    for(int i = 0; i < sizeof(test.b_date) * 8; i++)
    {
        //右移
        test.change = test.change << 1;

        //逐位检查BCD码是否即将溢出
        if(i < sizeof(test.b_date) * 8 - 1)
        {
            if((test.d_date_1_and_2 & 0x0f) >= 0x05)
            {
                test.d_date_1_and_2 += 3;  //低位即将溢出,直接加3
            }
            if((test.d_date_1_and_2 & 0xf0) >= 0x50)
            {
                test.d_date_1_and_2 += 0x30;  //高位即将溢出,高为加3
            }
            if((test.d_date_3_and_4 & 0x0f) >= 0x05)
            {
                test.d_date_3_and_4 += 3;  //低位即将溢出,直接加3
            }
            if((test.d_date_3_and_4 & 0xf0) >= 0x50)
            {
                test.d_date_3_and_4 += 0x30;  //高位即将溢出,高为加3
            }
            if(test.d_date_5 >= 5)
            {
                test.d_date_5 += 3;
            }
        }
        printf("%lx\n", test.change);
    }

    // 输出结果
    printf("%x", test.d_date_5);
    printf("%x", test.d_date_3_and_4);
    printf("%x\n", test.d_date_1_and_2);

    return 0;
}
