#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>

int data=1;

sem_t accessMutex;     // Semaphore to only allow single read or write operation, initialized to 1
sem_t readersMutex;    // Semaphore to ensure mutual exclusion, initialized to 1
sem_t orderMutex;      // Semaphore used for maintaing FIFO signalling of requests, initialized to 1

unsigned int readers = 0;  // Stores the number of readers accessing the resource

// wait()-> Decrements the semaphore value
// signal()-> Increments the semaphore value

void ReadResource(void *rid)
{
    printf("Reader %d: read the data as %d\n", *((int *)rid), data);
}

void WriteResource(void *wid)
{
    data = data * 2;
    printf("Writer %d updated the data to %d\n", (*((int *)wid)), data);
    
}

void *reader(void *rid)
{
  sem_wait(&orderMutex);           // wait for arrival
  sem_wait(&readersMutex);         // only allow single access to the code
  
  readers++;                 // New Reader has entered

  if (readers == 1)        
    sem_wait(&accessMutex);        // Request for exclusive access to resource is made for the reader.
  
  sem_post(&orderMutex);           // Since we have been served, we can release order of arrival semaphore.
  sem_post(&readersMutex);         // The reader has successfully entered.

  // Critical Section, Reading is performed
  ReadResource(rid);          // Readers can read resource


  sem_wait(&readersMutex);         // Mutual exclusion to avoid race between readers
  readers--;               // One reader is leaving, now there is one less reader
  if (readers == 0)        // If there are no more readers to serve
    sem_post(&accessMutex);        // Exclusive Access to resource is released
  sem_post(&readersMutex);         // We have served readers for now.
}

void *writer(void *wid)
{
  sem_wait(&orderMutex);           // Wait for writer
  sem_wait(&accessMutex);          // Request exclusive access to the resource
  sem_post(&orderMutex);           // Since we have been served, we can release order of arrival semaphore.
  
  //Writer enters into critical section and is doing some action.
  WriteResource(wid);         // Writer has the choice to modify resource.

  sem_post(&accessMutex);          // Release exclusive access to the resource
}

int main()
{
    int reader_cnt,writer_cnt, MAX_ID;
    printf("Number of Readers: \n");
    scanf("%d", &reader_cnt);

    printf("Number of Writers: \n");
    scanf("%d", &writer_cnt);

    MAX_ID = reader_cnt >= writer_cnt ? reader_cnt : writer_cnt;

    int id[MAX_ID];
    for (int i = 0; i < MAX_ID; i++)
        id[i]=i+1;

    pthread_t r_thread[reader_cnt], w_thread[writer_cnt];

    //Initializing Semaphores
    sem_init(&accessMutex, 0, 1);
    sem_init(&readersMutex, 0, 1);
    sem_init(&orderMutex, 0, 1);

    for (int i = 0; i < reader_cnt; i++)
    {
        pthread_create(&r_thread[i], NULL, reader, (void *)&id[i]);
    }
    for (int i = 0; i < writer_cnt; i++)
    {
        pthread_create(&w_thread[i], NULL, writer, (void *)&id[i]);
    }

    for (int i = 0; i < reader_cnt; i++)
    {
        pthread_join(r_thread[i], NULL);
    }
    for (int i = 0; i < writer_cnt; i++)
    {
        pthread_join(w_thread[i], NULL);
    }

    //Destroying Semaphores
    sem_destroy(&orderMutex);
    sem_destroy(&accessMutex);
    sem_destroy(&readersMutex);

    return 0;
}