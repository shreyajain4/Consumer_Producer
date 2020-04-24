#include<iostream>
#include<pthread.h>
#include<unistd.h>
#include<bits/stdc++.h>
using namespace std;

const int p=2;
int c=2;
pthread_mutex_t mutex_head[p];
pthread_mutex_t mutex_tail[p];
pthread_mutex_t mutex_throughput=PTHREAD_MUTEX_INITIALIZER;
int n=4;
int throughput=0;
struct Q
{
	int head=0;
	int tail=0;
	vector<int> sub;
	
};
vector<Q> q;
void* producer(void* z)
{
	intptr_t i=(intptr_t) z;
	//cout<<i<<endl;
	while(true)
	{
	pthread_mutex_lock(&(mutex_tail[i]));
	if(q[i].sub[q[i].tail%n]==0)
	{
		q[i].sub[q[i].tail%n]=rand()%10+1;
		printf("inserted %d \n",q[i].sub[q[i].tail%n]);
		q[i].tail=q[i].tail+1;
		q[i].tail=q[i].tail%n;
		pthread_mutex_unlock(&mutex_tail[i]);
	}
	else
	{
	sleep(2);
	pthread_mutex_unlock(&mutex_tail[i]);
	}
	}
}
void* consumer(void* z)
{
	while(true)
	{
		int i=0;
		while(i<p && pthread_mutex_trylock(&mutex_head[i])!=0)
		i++;
		if(i==p)
		{
		pthread_mutex_lock(&mutex_head[0]);
		i=0;
		
		}
		if(q[i].sub[q[i].head%n]!=0)
		{
			printf("popped %d \n",q[i].sub[q[i].head%n]);
			int t=q[i].sub[q[i].head%n];
			q[i].sub[q[i].head%n]=0;
			pthread_mutex_lock(&mutex_throughput);
			throughput+=t;
			printf("Throughput Data whivch is to be divided by total time of execution %d \n",throughput);
			pthread_mutex_unlock(&mutex_throughput);
			q[i].head=q[i].head+1;
			q[i].head=q[i].head%n;
			pthread_mutex_unlock(&mutex_head[i]);
			sleep(t);
		}
		else
		pthread_mutex_unlock(&mutex_head[i]);
	}
}
int main(int argc, char **argv)
{
	srand (time(NULL));
	vector<Q> temp(p);
	q=temp;
	void* z=NULL;
	pthread_t tid_producers[p];
	
	for(int i=0;i<p;i++)
	{
		vector<int> t(n/p,0);
		q[i].sub=t;
		mutex_head[i]= PTHREAD_MUTEX_INITIALIZER;
		mutex_tail[i]= PTHREAD_MUTEX_INITIALIZER;
		pthread_attr_t attr_p;
		pthread_attr_init(&attr_p);
		pthread_create(&tid_producers[i], &attr_p , producer ,(void*)i);

	}
	pthread_t tid_consumers[c];
	for(int i=0;i<c;i++)
	{
	
		pthread_attr_t attr_c;
		pthread_attr_init(&attr_c);
		pthread_create(&tid_consumers[i], &attr_c , consumer ,(void*)z);
		

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

