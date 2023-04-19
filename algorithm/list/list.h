#ifndef HEAD_H
#define HEAD_H

#define MAIN_POINT main_point
#define MINOR_POINT minor_point
#define MAIN_DATE main_date
#define MINOR_DATE minor_date

/* 该链表是双向循环链表,有哨兵结构
 * 链表结构如下:
 * -----      -----      -----      -----
 * |   | <--> |   | <--> |   | <--> |   |
 * -----      -----      -----      -----
 *   |          |          |          |
 * -----      -----      -----      -----
 * |   |      |   |      |   |      |   |
 * -----      -----      -----      -----
 *   |          |          |          |
 * -----      -----      -----      -----
 * |   |      |   |      |   |      |   |
 * -----      -----      -----      -----
 * */

// 主节点数据结构体
typedef struct {
    int a;
    int b;
} MAIN_DATE;

// 次节点数据结构体
typedef struct {
    int a;
    int b;
} MINOR_DATE;

// 次节点结构体
typedef struct MINOR_POINT {
    MINOR_DATE *point_date; // 数据域指针
    struct MINOR_POINT *prev;
    struct MINOR_POINT *next;
} MINOR_POINT;

// 链表主节点结构体
typedef struct MAIN_POINT {
    MAIN_DATE *main_date; // 数据域指针
    MINOR_POINT *point_head;    // 次节点指针
    struct MAIN_POINT *prev;
    struct MAIN_POINT *next;
} MAIN_POINT;

// 初始化链表
main_point *list_init();
// 创建主节点
main_point *create_main_point(MAIN_DATE *main_date);
// 插入节点
int main_insert(MAIN_POINT *main_head, MAIN_POINT *new_main_point);
int minor_insert(MINOR_POINT *insert_local, MINOR_POINT *insert_point);
// 查找节点(可以封装这两个函数,查找所有类型的节点)
MAIN_POINT *main_search(MINOR_DATE *date);
MINOR_POINT *minor_search(MAIN_POINT *search_main_point, MINOR_DATE *point_date);
// 删除节点
int main_delete(MAIN_POINT *delete_main_point);
int minor_delete(MINOR_POINT *delete_point);
// 遍历节点(可以将这两个函数封装,遍历所有节点)
void main_output(MAIN_POINT *main_head);
void minor_output(MINOR_POINT *point_head);

#endif
