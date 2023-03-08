#include <unistd.h>
#include <stdio.h>

int main()
{
	//给main函数的参数,用不上
	char * const argv[] = {NULL};
	//给加载程序的环境变量
	char * const envp[] = {"/mnt/hgfs/linuxshare/demo/process",NULL};
	//将环境变量给加载程序
	execve("/mnt/hgfs/linuxshare/demo/process/execvp_to_print_success.c/",argv,envp);
	perror("execve");
	return 0;
}
