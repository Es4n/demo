#include <unistd.h>
#include <stdio.h>

int main()
{
	char *const argv[] = {NULL};
	//不写绝对路径,直接加载print_success程序,测试上个函数传过来的环境变量
	execvp("print_success",argv);
	perror("test_execvp");
	return 0;
}
