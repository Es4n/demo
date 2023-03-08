#include <unistd.h>
#include <stdio.h>

int main()
{
	//给main函数的参数,用不上
	char * const argv[] = {NULL};
	//给加载程序的环境变量
	char * const envp[] = {"PATH=/mnt/hgfs/linuxshare/demo/process/exec",NULL};
	//将环境变量给加载程序
	execve("/mnt/hgfs/linuxshare/demo/process/exec/execvp_to_print_success",argv,envp);
	perror("execve");
	return 0;
}
