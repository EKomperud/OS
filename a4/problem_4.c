#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <assert.h>
#include <time.h>
#include <unistd.h>

typedef struct a_thread
{
  int tID, enterCount;
} a_thread;

void lock(a_thread tID);
void unlock(a_thread tID);
void *thread(void *tID);

static volatile int* number;
static volatile int* entering;

static long numThreads;
static long totalTime;
//static volatile clock_t timer;
static volatile int running;
static volatile int in_cs;

int main (int argc, const char *argv[])
{
  if (argc != 3)
    return 1;

  numThreads = strtol(argv[1],NULL,0);
  totalTime = strtol(argv[2],NULL,0);
  //totalTime *= numThreads;
  if (numThreads < 1 || numThreads > 99 || totalTime < 1)
    return 1;
  
  number = calloc(numThreads, sizeof(int));
  entering = calloc(numThreads, sizeof(int));
  pthread_t t[numThreads];
  a_thread td[numThreads];
  in_cs = 0;
  //timer = 0;
  running = 1;

  int i;
  for (i = 0; i < numThreads; i++)
  {
    a_thread thr = { i, 0 };
    td[i] = thr;
    if (pthread_create(&t[i],NULL,thread,&td[i]))
    {
      return 1;
    }
  }

  if(sleep(totalTime))
  {
    return 1;
  }
  running = 0;

  for (i = 0; i < numThreads; i++)
  {
    pthread_join(t[i], NULL);
    printf("Thread %d joined. Entered critical section %d times\n", i, td[i].enterCount);
  }

  return 0;
}

void *thread(void *tID)
{
  a_thread *data = (a_thread *)tID;
  while (running)
  {
    lock(*data);
    data->enterCount++;
    
    assert (in_cs == 0);
    in_cs++;
    assert (in_cs == 1);
    in_cs++;
    assert (in_cs == 2);
    in_cs++;
    assert (in_cs == 3);
    in_cs=0;

    //timer = clock() / CLOCKS_PER_SEC;
    unlock(*data);
  }
  pthread_exit(NULL);
  return NULL;
}

void lock(a_thread tID)
{
  entering[tID.tID] = 1;
  int max = 0;
  int i;
  for (i = 0; i < numThreads; i++)
  {
    max = max < number[i] ? number[i] : max;
  }
  number[tID.tID] = 1 + max;
  entering[tID.tID] = 0;

  int j;
  for (j = 0; j < numThreads; j++)
  {
    while (entering[j]) {;}
    while ((number[j] != 0) && (number[tID.tID] > number[j] || (number[tID.tID] == number[j] && tID.tID > j))) {;}
  }
}

void unlock(a_thread tID)
{
  number[tID.tID] = 0;
}
