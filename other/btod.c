#include <stdio.h>

/* 八位的二进制转十进制BCD码
 * 原理详见btod_long.c
 * */

typedef union {
    int change;
    unsigned char b_date;
    struct {
        unsigned char nop;
        unsigned char d_date_l_and_m;
        unsigned char d_date_h;
    };
} change_union;

int main()
{
    change_union test;

    test.b_date = 112;
    printf("%x\n",test.change);

    for(int i = 0; i < sizeof(unsigned char) * 8; i++)
    {
        test.change = test.change << 1;

        if(i < sizeof(unsigned char) * 8 - 1)
        {
            if((test.d_date_l_and_m & 0x0f) >= 0x05)
            {
                test.d_date_l_and_m += 3;
            }
            if((test.d_date_l_and_m & 0xf0) >= 0x50)
            {
                test.d_date_l_and_m += 0x30;
            }
            if(test.d_date_h >= 5)
            {
                test.d_date_h += 3;
            }
        }
        printf("%x\n",test.change);
    }

    printf("%x", test.d_date_h);
    printf("%x\n", test.d_date_l_and_m);

    return 0;
}
