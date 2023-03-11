/*-------------------------------------------------------------------------------
IPC using Shared Memory
Written By:
     1- Dr. Mohamed Aboutabl
-------------------------------------------------------------------------------*/

#ifndef SHMEM_SEGMENT
#define SHMEM_SEGMENT

typedef struct
{
    unsigned counter;
    int p1Started;
    int p2Started;
    int p1Done;
    int p2Done;
} shmData;

#define SHMEM_SIZE sizeof(shmData)

#define LARGEINT 50000000

#endif
