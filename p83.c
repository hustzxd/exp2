#include <sys/types.h>
#include <sys/wait.h>
int main()
{
	pid_t  pid;
	int status;
	pid = fork();
	if(pid == 0)
	{
		printf("p2\n");
		exit(1);
	}
	else
	{
		printf("p1\n");
		sleep();
	}
	wait(&status);
	printf("p3\n");
}