#include "list.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 链表主节点初始化
MAIN_POINT *list_init()
{
    // 创建哨兵节点
    MAIN_POINT *main_head = (MAIN_POINT *)malloc(sizeof(MAIN_POINT));

    main_head->main_date  = NULL;
    main_head->point_head = NULL;
    main_head->prev       = main_head;
    main_head->next       = main_head;

    return main_head;
}

// 创建主节点
MAIN_POINT *create_main_point(MAIN_DATE *main_date)
{
    // 申请堆空间
    MAIN_POINT *new_main_point = (MAIN_POINT *)malloc(sizeof(MAIN_POINT));

    // 设置数据
    new_main_point->main_date = main_date;
    new_main_point->prev      = NULL;
    new_main_point->next      = NULL;

    // 创建次链表的哨兵节点
    new_main_point->point_head             = malloc(sizeof(MINOR_POINT));
    new_main_point->point_head->point_date = NULL;
    new_main_point->point_head->prev       = new_main_point->point_head;
    new_main_point->point_head->next       = new_main_point->point_head;

    return new_main_point;
}

// 插入主节点
int main_insert(MAIN_POINT *main_head, MAIN_POINT *new_main_point)
{
    if(new_main_point == NULL)
        return -1;

    main_head->next->prev = new_main_point;
    new_main_point->next  = main_head->next;
    main_head->next       = new_main_point;
    new_main_point->prev  = main_head;

    return 0;
}

// 插入次节点
int point_insert(MINOR_POINT *insert_local, MINOR_POINT *insert_point)
{
    if(insert_point == NULL)
        return -1;

    insert_local->next->prev = insert_point;
    insert_point->next       = insert_local->next;
    insert_local->next       = insert_point;
    insert_point->prev       = insert_local;

    return 0;
}

// 遍历主节点
void main_output(MAIN_POINT *main_head)
{
    MAIN_POINT *read_main_list = main_head;

    while(read_main_list->next != main_head)
    {
        // 读取下一个节点
        read_main_list = read_main_list->next;

        // 数据输出
        printf("%d,", read_main_list->main_date->a);
        printf("%d\n", read_main_list->main_date->b);
    }
}

//删除主节点
int main_delete(MAIN_POINT *delete_main_point)
{
    if(delete_main_point == NULL)
        return -1;

    delete_main_point->prev->next = delete_main_point->next;
    delete_main_point->next->prev = delete_main_point->prev;

    delete_main_point->next = NULL;
    delete_main_point->prev = NULL;

    free(delete_main_point);

    return 0;
}

//删除次节点
int minor_delete(MINOR_POINT *delete_point);
