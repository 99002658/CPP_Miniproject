#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <semaphore.h>
#include <iostream>
#include "karaoke.h"
#include <cstring>
#include <list>
#include <iterator>
#define SHARED		0		/* 0 = thread-, !0 = process-sharing */
#define RUNS		10		/* In total how often they can eat */
#define N		5		/* number of Customerss */
#define LEFT		(i+N-1)%N	/* number of i's left neighbor */
#define RIGHT		(i+1)%N		/* number of i's right neighbor */
#define SINGING	0		/* Customers is SINGING */
#define HUNGRY		1		/* Customers is trying to get forks */
#define EATING		2		/* Customers is EATING */

using namespace std;

void *Customers(void*);				
void take_spoons(int);				
void put_spoons(int);
void test(int);
	
int state[N];			/* array to keep track of everyone's state */
sem_t mutex;			/* mutual exclusion for deadlock inducing regions */
sem_t s[N];			/* one semaphore per Customers */
int runs = 0;			/* how often a Customers has eaten til now */
int eat_count = 0;
int sing_count = 0;
int hungry_count = 0;


int main()
{
	pthread_t threads[N];	
	int num[N];		

	for(int i = 0; i < N; i++)
		num[i] = i;

	sem_init(&mutex, SHARED, 1);
	for(int i = 0; i < N; i++)
		sem_init(&s[i], SHARED, 0);
    
    cout<<"--------------------------------"<<endl;
    cout<<"---Karaoke Restaurant Problem---"<<endl;
    cout<<"--------------------------------"<<endl; 
    cout<<"The problem revolves around a certain number of customers."<<endl ;
    cout<<"The task is to come up with an effective soultion so that all of the customers can eat their food without starving."<<endl ;
    cout<<"The task of  eating is constricted by the number of spoons available. A person has to use 2 spoons in order to eat their food."<<endl;
    cout<<"When no spare spoons are available a customer has to sing, in order to pass the time. "<<endl;
    cout<<"***One thing to keep in mind is that the solution shouldn't cause a deadlock and lead to starvation. "<<endl;
	cout<<"Karaoke Session Started::\n"<<endl;

	for(int i = 0; i < N; i++)
		pthread_create(&threads[i], NULL, Customers, (void*) &num[i]);
	
	for(int i = 0; i < N; i++)
		pthread_join(threads[i], NULL);

    cout<<"\nTotal number of times customers have eaten::"<<eat_count<<endl;
    cout<<"Total number of times customers have sung::"<<sing_count<<endl;

	return 0;
}

void *Customers(void *args)		
{
	while (runs < RUNS)
	{
		sleep(random() % 1 + 2);	
		take_spoons(*(int*)args);
		sleep(random() % 1 + 1);
		put_spoons(*(int*)args);	
	}
	return 0;
}

void take_spoons(int i)
{
	sem_wait(&mutex);	
	state[i] = HUNGRY;	
	cout<<"Customers "<<i<<" is now hungry!"<<endl;
    hungry_count++;
	test(i);			
	sem_post(&mutex);		
	sem_wait(&s[i]);		
}

void put_spoons(int i)
{
	sem_wait(&mutex);		
	state[i] = SINGING;	
	cout<<"Customers "<<i<<" is now SINGING!"<<endl;
    sing_count++;
	runs++;				
	test(LEFT);		
	test(RIGHT);			
	sem_post(&mutex);		
}

void test(int i)
{
	if (state[i] == HUNGRY && state[LEFT] != EATING 
	    	&& state[RIGHT] != EATING) {
		state[i] = EATING;
        cout<<"\nEat Count::"<<eat_count<<endl;
		cout<<"Customers "<<i<<" is now EATING!"<<endl;
        eat_count++;
		sem_post(&s[i]);
	}
}
