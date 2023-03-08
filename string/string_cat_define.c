#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>

/* 作用
 * 用宏函数实现链接字符串
 * */
#define PATH1 "Find " 
#define PATH2 " plase"
#define CAT(a,b,x) \
	{strcat((x),b); \
	 memmove((x) + strlen(a),(x),strlen(x) + 1); \
	 memmove((x),(a),strlen(a));}

int main()
{
	//这是可变参数，可以创建想要的
	char str[30] = "me";

	//连接PATH1和PATH2及可变参数
	CAT(PATH1,PATH2,str);

	printf("%s\n",str);

	return 0;
}
