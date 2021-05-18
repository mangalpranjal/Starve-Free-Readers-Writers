# The Problem
The readers-writers problem is a classical one in computer science and is divided into three case. Our interest in this repository is towards Starve Free Readers Writers Problem (Third Case). We have a resource (e.g. database) that can be accessed by readers or writers. Readers do no modify the resouce whereas writers can read as well as modify the resource. When a writer is modifying the resource, no-one else (reader or writer) can access it at the same time. Another writer trying to access the same resource can corrupt it or another reader trying to read it could only read a partially modified value. 

# The Solution

## Reader Process
Reader requests the entry to critical section. The role of reader is to read the resouce and is not allowed to modify the resource. We use Semaphores as a signalling mechanism.

1. Number of readers inside the critical section is incremented. If there is any reader accessing the critical section, writer is locked from accessing the critical 2. section.
2. It then, signals mutex as any other reader is allowed to enter while others are already reading.
3. After performing reading, reader exits the critical section. If no reader is inside, it signals the write semaphore to allow entry of writers in the critical section. 
4. If not allowed, it keeps on waiting.

## Writer Process
The role of writer is simple since we are considering only one writer modifies the resource. If this is not the case, the program may crash.

1. Writer requests for entry in the critical section. It waits until it is granted access.
2. Resources are asked for and allocated.
3. Writer is granted entry to critical section. It can now make changes in the resouce.
4. Writer exits section and resources are released.
