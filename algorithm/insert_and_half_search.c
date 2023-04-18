#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* 测试插入排序和二分查找
 * 先用随机数生成一个范围的数组集合
 * 用插入排序对生成的随机数进行排序
 * 然后输入一个想要查找的数值,使用二分查找
 * */

int half_serach(int *arr, int start, int end, int key)
{
    for(;;)
    {
        if((end - start) <= 2)  //如果查找的区间内的数小于3个,直接输出结果
        {
            if(key == arr[start])  //如果等于左边,就输出左边
            {
                printf("return %d\n", start);
                return start;
            } else if(key == arr[end])  //如果等于右边,就输出右边
            {
                printf("return %d\n", end);
                return end;
            } else  //输出中间
            {
                printf("return %d\n", ((end - start) >> 1) + start);
                return ((end - start) >> 1) + start;
            }
        } else if(key > arr[((end - start) >> 1) + start])  //如果大于中间值,就搜索右边的区间
        {
            printf("serach in:%d,%d\n", ((end - start) >> 1) + start + 1, end);
            start = ((end - start) >> 1) + start + 1;
        } else  //如果小于等于中间,就搜索左边的区间
        {
            printf("serach in:%d,%d\n", start, ((end - start) >> 1) + start);
            end = ((end - start) >> 1) + start;
        }
    }
}

int main()
{
    int arr[100] = {0};
    int input;
    int arr_num = -1;
    int insert  = 0;

    //播撒随机种子
    srand(time(NULL));

    //循环生成随机数,一边生成一边用插入排序插入数组
    for(int j = 0; j < 100; j++)
    {
        //生成随机数
        input = rand() % 1000;

        //找到插入的位置,insert为插入位置的下标
        for(; insert <= arr_num; insert++)
        {
            if(input <= arr[insert])
                break;
        }

        //数组中的数字数量加一
        arr_num++;

        //插入位置往后的数据往后挪
        for(int i = arr_num; i > insert; i--)
        {
            arr[i] = arr[i - 1];
        }

        //插入数据
        arr[insert] = input;
        insert      = 0;
    }

    //打印数组中的数据,每10个数据为1行
    for(int i = 0; i <= arr_num / 10; i++)
    {
        for(int j = 0; j < 10; j++)
        {
            printf("%d,", arr[i * 10 + j]);
        }
        printf("\n");
    }

    //输入要查找的数据
    printf("search\n");
    scanf("%d", &input);

    //调用二分查找
    int search_re = half_serach(arr, 0, arr_num, input);

    //判断二分查找返回的数据是否与要查找的数据相等
    if(input == arr[search_re])
    {
        printf("%d\n", arr[search_re]);
    } else
    {
        printf("search fail\n");
    }
}
