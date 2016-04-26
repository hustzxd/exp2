#include <stdio.h>
#include <pthread.h>
int A;
void subp1();
int main(void)
{
	pthread_t p1;
	int pid;
	A = 0;
	pid = fork();
	if(pid == 0)
	{
		printf("A in son process is %d\n", A);
		A = 100;
		exit(0);
	}
	//wait();
	pthread_create(&p1,NULL,(void *)subp1,NULL);
	pthread_join(p1,NULL);
	printf("A in father process is %d\n", A);
	return 0;
}
void subp1()
{
	printf("A in thread is %d\n", A);
	A = 10;
}