semaphore accessMutex = 1;     // Semaphore to only allow single read or write operation, initialized to 1
semaphore readersMutex = 1;    // Semaphore to ensure mutual exclusion, initialized to 1
semaphore orderMutex = 1;      // Semaphore used for maintaing FIFO signalling of requests, initialized to 1

unsigned int readers = 0;  // Stores the number of readers accessing the resource

// wait()-> Decrements the semaphore value
// signal()-> Increments the semaphore value

void reader()
{
  wait(orderMutex);           // wait for arrival
  wait(readersMutex);         // only allow single access to the code
  
  reader++;                 // New Reader has entered

  if (readers == 1)        
    wait(accessMutex);        // Request for exclusive access to resource is made for the reader.
  
  signal(orderMutex);           // Since we have been served, we can release order of arrival semaphore.
  signal(readersMutex);         // The reader has successfully entered.

  // Critical Section, Reading is performed
  ReadResource();          // Reader can read resources


  wait(readersMutex);         // Mutual exclusion to avoid race between readers
  readers--;               // One reader is leaving, now there is one less reader
  if (readers == 0)        // If there are no more readers to serve
    signal(accessMutex);        // Exclusive Access to resource is released
  signal(readersMutex);         // We have served readers for now.
}

void writer()
{
  wait(orderMutex);           // Wait for writer
  wait(accessMutex);          // Request exclusive access to the resource
  signal(orderMutex);           // Since we have been served, we can release order of arrival semaphore.
  
  //Writer enters into critical section and is doing some action.
  WriteResource();         // Writer have the choice to modify resources.

  signal(accessMutex);          // Release exclusive access to the resource
}