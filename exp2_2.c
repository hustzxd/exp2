#include <stdio.h>
#include <pthread.h>
#include <linux/sem.h>

#define MUTEX 0
#define FULL 1
#define EMPTY 2

int semid;
int a[10];
void P(int semid,int index)
{
	struct sembuf sem;
	sem.sem_num = index;
	sem.sem_op = -1;
	sem.sem_flg = 0;
	semop(semid, &sem ,1);
	return;
}

void V(int semid,int index)
{
	struct sembuf sem;
	sem.sem_num = index;
	sem.sem_op = 1;
	sem.sem_flg = 0;
	semop(semid, &sem ,1);
	return;
}

void *pthread_c();
void *pthread_p();

int main()
{
	int i;
	for(i=0;i<10;i++)
	{
		a[i] = i;
	}
	int ret;
	pthread_t pid_c;
	pthread_t pid_p;
	union semun semopts;
	semid = semget(120,3,IPC_CREAT|0666);//two sem
	if(semid == -1)
	{
		printf("create sem error!\n");
		exit(1);
	}
	semopts.val = 1;
	ret = semctl(semid,MUTEX,SETVAL,semopts);
	if(ret == -1)
	{
		printf("semctl error!\n");
		exit(1);
	}
	semopts.val = 1;
	ret = semctl(semid,EMPTY,SETVAL,semopts);
	if(ret == -1)
	{
		printf("semctl error!\n");
		exit(1);
	}
	semopts.val = 0;
	ret = semctl(semid,FULL,SETVAL,semopts);
	if(ret == -1)
	{
		printf("semctl error!\n");
		exit(1);
	}

	ret = pthread_create(&pid_p,NULL,pthread_p,NULL);
	if(ret != 0)
	{
		printf("create thread error!\n");
		exit(1);
	}
	ret = pthread_create(&pid_c,NULL,pthread_c,NULL);
	if(ret != 0)
	{
		printf("create thread error!\n");
		exit(1);
	}

	ret = pthread_join(pid_c,NULL);
	if(ret != 0)
	{
		printf("pthread_join error!\n");
		exit(1);
	}
	ret = pthread_join(pid_p,NULL);
	if(ret != 0)
	{
		printf("pthread_join error!\n");
		exit(1);
	}
	//delete sems
	// ret = semctl(semid,EMPTY,IPC_RMID,0);
	// if(ret == -1)
	// {
	// 	printf("semctl error!\n");
	// }

	// ret = semctl(semid,FULL,IPC_RMID,0);
	// if(ret == -1)
	// {
	// 	printf("semctl error!\n");
	// }
	
	ret = semctl(semid,MUTEX,IPC_RMID,0);
	if(ret == -1)
	{
		printf("semctl error!\n");
	}
	
	printf("Back to main!\n");
	return 0;
}

void *pthread_c()
{//compute
	int i = 0;
	int j = 0;
	for(;i<10;i++)
	{
		P(semid,EMPTY);
		P(semid,MUTEX);
		printf("in c\n");
		for(j=0;j<10;j++)
		{
			a[j]=a[j]+1;
		}
		sleep(1);//computing time
		V(semid,MUTEX);
		V(semid,FULL);
	}
	
	return NULL;
}

void *pthread_p()
{//print
	int i =0;
	int j = 0;
	for(;i<10;i++)
	{
		P(semid,FULL);
		P(semid,MUTEX);
		printf("in p: ");
		for(j=0;j<10;j++)
		{
			printf("%d ", a[j]);
		}
		printf("\n");
		V(semid,MUTEX);
		V(semid,EMPTY);
	}
	return NULL;
}


