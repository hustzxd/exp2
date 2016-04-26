#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
main()
{
	if(fork() == 0)
	{
		printf("a\n");
		execlp("./file1",0);
		printf("b\n");
	}
	sleep(1);
	printf("c\n");
}