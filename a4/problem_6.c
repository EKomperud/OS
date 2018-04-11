#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>
#include <math.h>

void *thread(void *tID);

static long numThreads;
static long totalTime;
static volatile int running;
pthread_mutex_t lock;

static volatile double hits;
static volatile double tries;

int main (int argc, const char *argv[])
{
  if (argc != 3)
    return 1;

  numThreads = strtol(argv[1],NULL,0);
  totalTime = strtol(argv[2],NULL,0);
  if (numThreads < 1 || numThreads > 99 || totalTime < 1)
    return 1;

  if (pthread_mutex_init(&lock, NULL) != 0)
    return 1;

  pthread_t t[numThreads];
  running = 1;

  int i;
  for (i = 0; i < numThreads; i++)
  {
    int gen = i;
    if (pthread_create(&t[i],NULL,thread,&gen))
    {
      return 1;
    }
  }

  if(sleep(totalTime))
    return 1;

  running = 0;

  for (i = 0; i < numThreads; i++)
    if (pthread_join(t[i], NULL))
      return 1;

  double four = 4.0;
  double pi = (four * hits) / tries;
  fprintf(stdout,"You may have %.10f pieces of my pi\n",pi);

  return 0;
}

void *thread(void *data)
{
  int *gen = (int *)data;
  srand(time(NULL) + *gen);
  while (running)
  {
    double two = 2.0;
    double four = 4.0;
    double x = (((double)rand()/(double)RAND_MAX)*four) - two;
    double y = (((double)rand()/(double)RAND_MAX)*four) - two;
    //printf("[%.5f , %.5f]\n",x,y);
    if (sqrt(pow(x,two) + pow(y,two)) <= two)
    {
        pthread_mutex_lock(&lock);
        ++hits;
        pthread_mutex_unlock(&lock);
    }
    pthread_mutex_lock(&lock);
    ++tries;
    pthread_mutex_unlock(&lock);
  }
  return NULL;
}
