#include <iostream>
#include<bits/stdc++.h>
#include <semaphore.h>
#include <pthread.h>
#include <unistd.h>


using namespace std;
int counter=0;
sem_t counterFull,countSem,s,emptyBuffer, fullBuffer;
int currentCounter,n,m;

vector<int >  buffer;
int nextIn,nextOut;

void *collectorFunction(void* arg){

while(1){


    sleep(rand()%20);
    int full;
    sem_getvalue(&fullBuffer,&full);
    if(full==0){
        cout<<"Collector Thread: Nothing in the Buffer"<<endl;
    }
    else{
    sem_wait(&fullBuffer);
    sem_wait(&s);
    cout<<"Collector Thread: reading from buffer at position "<< nextOut<<endl;
    nextOut=(nextOut+1)%m;
    sem_post(&s);
    sem_post(&emptyBuffer);
    }
  }

}



void *monitorFunction(void* var){

while(1){

    sleep(rand()%20);
    int emptyy;
    sem_getvalue(&emptyBuffer,&emptyy);
    if(emptyy==0){
    cout<<"Monitor Thread: Buffer full!!"<<endl;
    }
    else{

    sem_wait(&counterFull);
    cout<<"Monitor Thread: waiting to read"<<endl;
    sem_wait(&countSem);
    cout<<"Monitor Thread: reading a count of value of "<<counter<<endl;
    currentCounter=counter;
    counter=0;
    sem_post(&countSem);
    sem_wait(&emptyBuffer);
    sem_wait(&s);
    cout<<"Monitor Thread: writing to a buffer at position "<< nextIn<<endl;
    buffer[nextIn]=currentCounter;
    nextIn=(nextIn+1)%m;
    sem_post(&s);
    sem_post(&fullBuffer);

    }

  }

}

void *increamentCounter(void *k){

int* i=(int*)malloc(sizeof(int));

i=(int *)k;

cout<<"Counter Thread "<<i[0]<<": recieved message"<<endl;
  while(1){

    sleep(rand()%10);
    cout<<"Counter Thread "<<i[0]<<": waiting to write"<<endl;
    sem_wait(&countSem);
    counter++;
    cout<<"Counter Thread "<<i[0]<<": now adding to the counter,counter value= "<<counter<<"\n";
    sem_post(&countSem);
   sem_post(&counterFull);
   }

}




int main()
{

cout<<"Enter number of mcounter threads: "<<"\n";
cin>>n;
cout<<"Enter number of elemnts of buffer: "<<"\n";
cin>>m;
buffer.assign(m,0);

sem_init(&countSem,1,1);
sem_init(&counterFull,1,0);
sem_init(&s,1,1);
sem_init(&emptyBuffer,1,m);
sem_init(&fullBuffer,1,0);

nextIn=0,nextOut=0;

pthread_t mcounter[n];

for(int i=1;i<=n;i++){

int* k=(int*)malloc(sizeof(int));
*k=i;
pthread_create(&mcounter[i],NULL,increamentCounter,(void*)k);
}

pthread_t mmonitor,mcollector;

pthread_create(&mmonitor,NULL,monitorFunction,NULL);
pthread_create(&mcollector,NULL,collectorFunction,NULL);


for(int i=1;i<=n;i++){

pthread_join(mcounter[i],NULL);
}

pthread_join(mmonitor,NULL);
pthread_join(mcollector,NULL);


return 0;

}
