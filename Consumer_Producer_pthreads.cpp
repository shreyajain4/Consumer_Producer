#include<iostream>
#include<pthread.h>
#include<unistd.h>
#include<bits/stdc++.h>
using namespace std;
vector<int> q;
int head=0;
int tail=0;
int p=2;
int c=2;
pthread_mutex_t mutex_head= PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_tail= PTHREAD_MUTEX_INITIALIZER;
int n=4;


void* producer(void* z)
{
	while(true)
	{
	pthread_mutex_lock(&mutex_tail);
	if(q[tail%n]==0)
	{
		q[tail]=rand()%10+1;
		printf("inserted %d \n",q[tail]);
		tail=tail+1;
		tail=tail%n;
		pthread_mutex_unlock(&mutex_tail);
	}
	else
	{
	sleep(2);
	pthread_mutex_unlock(&mutex_tail);
	}
	}
}
void* consumer(void* z)
{
	while(true)
	{
		pthread_mutex_lock(&mutex_head);
		if(q[head%n]!=0)
		{
			printf("popped %d \n",q[head]);
			q[head]=0;
			int t=q[head];
			head=head+1;
			head=head%n;
			pthread_mutex_unlock(&mutex_head);
			sleep(t);
		}
		else
		pthread_mutex_unlock(&mutex_head);
	}
}
int main(int argc, char **argv)
{
	srand (time(NULL));
	vector<int> temp(n,0);
	q=temp;
	void* z=NULL;
	pthread_t tid_producers[p];
	for(int i=0;i<p;i++)
	{
		pthread_attr_t attr_p;
		pthread_attr_init(&attr_p);
		pthread_create(&tid_producers[i], &attr_p , producer ,z);

	}
	pthread_t tid_consumers[c];
	for(int i=0;i<c;i++)
	{
	
		pthread_attr_t attr_c;
		pthread_attr_init(&attr_c);
		pthread_create(&tid_consumers[i], &attr_c , consumer ,z);
		

	}
	for(int i=0;i<p;i++)
	{
		pthread_join(tid_producers[i],NULL);
	}
	for(int i=0;i<c;i++)
	{
		pthread_join(tid_consumers[i],NULL);
	}

	
}

