#include <stdio.h>
#include <pthread.h>
#include <linux/sem.h>

static int ticket_num = 100;
static int agent_num = 2;
int semid;
int a = 0;

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

void *sell_ticket(void *agent);
int main()
{
	int ret;
	pthread_t npid[agent_num];
	int i;
	union semun semopts;
	semid  = semget(100,1,IPC_CREAT|0666);
	if(semid == -1)
	{
		printf("create sem error!\n");
		exit(1);
	}
	semopts.val = 1;
	ret = semctl(semid,0,SETVAL,semopts);
	if(ret == -1)
	{
		printf("semctl error!\n");
		exit(1);
	}
	for(i = 0; i < agent_num; i++)
	{
		ret = -1;
		ret = pthread_create(&npid[i], NULL, sell_ticket, (int *)i);
		if(ret == 0)
		{
			//success
			printf("Create a thread %d successful!\n", i);
		}
		else
		{
			//error
			printf("Create thread error!\n");
		}
	}
	for(i = 0;i < agent_num; i++)
	{
		pthread_join(npid[i], NULL);
	}
	ret = semctl(semid,0,IPC_RMID,0);
	if(ret != 0)
	{
		printf("RMID Error!\n");
	}
	printf("Back to main! sell ticket over!!!\n");
	return 0;
}

void *sell_ticket(void *agent)
{
	while(1)
	{
		P(semid,0);
		if(ticket_num == 0)
		{
			V(semid,0);
			break;
		}
		ticket_num--;
		printf("agent %d sell a ticket! And %d tickets left!\n", (int)agent,ticket_num);
		V(semid,0);
		//sleep(1);
	}
	printf("Agent %d sell all tickets! See you tomorrow!\n",(int)agent);
	return NULL;
}