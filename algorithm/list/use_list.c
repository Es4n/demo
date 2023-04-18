#include "list.h"
#include <stdio.h>
#include <stdlib.h>

int main()
{
    MAIN_POINT *list_head = list_init();

    for(int i = 0; i < 10; i++)
    {
        MAIN_DATE *date_main       = (MAIN_DATE *)malloc(sizeof(MAIN_DATE));
        date_main->a               = i;
        date_main->b               = i + 1;
        MAIN_POINT *new_main_point = create_main_point(date_main);
        main_insert(list_head, new_main_point);
    }

    main_output(list_head);

    MAIN_POINT *read_main_list = list_head;
    for(int i = 0;i < 1;i++)
    {
        read_main_list = read_main_list->next;
    }

    printf("----------------\n");

    main_delete(read_main_list);

    main_output(list_head);


}
