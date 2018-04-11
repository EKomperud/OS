#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <assert.h>
#include <time.h>
#include <unistd.h>

typedef struct a_thread
{
  int enterCount;
} a_thread;

typedef struct spin_lock_t
{
  volatile int counter, turn;
} spin_lock_t;

static inline int atomic_xadd (volatile int *ptr);
void spin_lock(struct spin_lock_t *s);
void spin_unlock(struct spin_lock_t *s);
void *thread(void *tID);

static long numThreads;
static long totalTime;
static volatile int running;
static volatile int in_cs;
static spin_lock_t lock = { 0 };

int main (int argc, const char *argv[])
{
  if (argc != 3)
    return 1;

  numThreads = strtol(argv[1],NULL,0);
  totalTime = strtol(argv[2],NULL,0);
  if (numThreads < 1 || numThreads > 99 || totalTime < 1)
    return 1;
  
  pthread_t t[numThreads];
  a_thread td[numThreads];
  in_cs = 0;
  running = 1;
  

  int i;
  for (i = 0; i < numThreads; i++)
  {
    a_thread thr = { 0 };
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
    spin_lock(&lock);
    data->enterCount++;
    
    assert (in_cs == 0);
    in_cs++;
    assert (in_cs == 1);
    in_cs++;
    assert (in_cs == 2);
    in_cs++;
    assert (in_cs == 3);
    in_cs=0;

    spin_unlock(&lock);
  }
  pthread_exit(NULL);
  return NULL;
}

static inline int atomic_xadd (volatile int *ptr)
{
  register int val __asm__("eax") = 1;
  asm volatile ("lock xaddl %0,%1"
		: "+a" (val)
		: "m" (*ptr)
		: "memory");
  return val;
}

void spin_lock (struct spin_lock_t *s)
{
  int me;
  me = atomic_xadd(&(s->counter));
  while (me != s->turn)
    sched_yield();
}

void spin_unlock (struct spin_lock_t *s)
{
  ++s->turn;
}
