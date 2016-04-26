#include <stdio.h>
#include <pthread.h>
static int ticket_num = 10;
static int agent_num = 4;
pthread_mutex_t mutex;
void *sell_ticket(void *agent);
int main()
{
	int ret;
	pthread_t npid[agent_num];
	int i = 0;
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
	while(1);
	printf("Back to main! sell ticket over!!!\n");
	return 0;
}

void *sell_ticket(void *agent)
{
	while(1)
	{
		pthread_mutex_lock(&mutex);
		if(ticket_num == 0)
			break;
		ticket_num--;
		printf("agent %d sell a ticket! And %d tickets left!\n", (int)agent,ticket_num);
		pthread_mutex_unlock(&mutex);
		sleep(1);
	}
	printf("Agent %d sell all tickets! See you tomorrow!\n",(int)agent);
	return NULL;
}