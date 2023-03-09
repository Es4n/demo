#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* 产生随即数
 * rand生成的数是有公式的,例如:(a + b * c % d)
 * a,b,c,d都是常数,因此产生的数有规律且重复
 * srand(time(NULL))就是根据时间改变a,b,c,d,从而产生更加随即的数
 * */

int main()
{
	srand(time(NULL));
	for(int i = 0;i < 30;i++)
	{
		printf("%d\n",rand()%10);
	}

	return 0;
}
