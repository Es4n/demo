#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main()
{
    int pid = fork();

    if(pid < 0)
    {
        perror("fork");
        return 0;
    } else if(pid == 0)
    {
        int arg = 1;

        while(arg != -1)
        {
            sleep(1);
        }
    }else
    {
        int arg = 1;

        while(arg != -1)
        {
            sleep(1);
        }

        wait(NULL);
    }
}
