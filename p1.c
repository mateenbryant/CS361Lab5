/*-------------------------------------------------------------------------------
Demo: IPC using Shared Memory
Written By:
     1- Dr. Mohamed Aboutabl

     The P1 Process:    p1.c

-------------------------------------------------------------------------------*/

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include "wrappers.h"

#include "shmSegment.h"

int main(int argc, char *argv[])
{
     int shmid;
     key_t shmkey;
     int shmflg;
     shmData *p;

     sem_t *sem; 
     sem = sem_open("/my_semaphore", O_CREAT | O_EXCL, S_IRUSR | S_IWUSR, 1);

     shmkey = ftok("shmSegment.h", 5);
     shmflg = IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR; // So, P1 must run before P2

     shmid = shmget(shmkey, SHMEM_SIZE, shmflg);
     if (shmid == -1)
     {
          printf("\nP1 Failed to get shared memory id=%d\n", shmid);
          perror("Reason: ");
          exit(-1);
     }

     p = (shmData *)shmat(shmid, NULL, 0); // Attach for R/W
     if (p == (shmData *)-1)
     {
          printf("\nP1 Failed to attach shared memory id=%d\n", shmid);
          perror("Reason: ");
          exit(-1);
     }


     /* Initialize data in the shared memory  */
     p->counter = 0;
     p->p1Done = 0;
     p->p1Started = 1;
     unsigned long MANY = 100000000;

     printf("P1 started. MANY = %10lu\n", MANY);
     printf("Waiting for P2 to start, too.\n");

     /* Wait for Other Process to start   */
     while (p->p2Started != 1)
          ;

     printf("P1: line 66");
     sem_wait(sem);
     printf("P1 now will increment the counter\n");
     for (unsigned i = 1; i <= MANY; i++)
          p->counter++;

     p->p1Done = 1;

     sem_post(sem);
     printf("P1 is done. Waiting for P2 to finish, too.\n");
     while (p->p2Done != 1)
          ;

     unsigned long expected = MANY << 1; // 2*MANY
     printf("P1 reports final counter value = %10u  Expecting: %10lu", p->counter, expected);

     if (p->counter == expected)
          printf("    CORRECT\n");
     else
          printf("    NOT CORRECT\n");

     shmdt(p);
     shmctl(shmid, IPC_RMID, NULL);

     return 0;
}
