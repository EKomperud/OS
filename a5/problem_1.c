#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <assert.h>
#include <unistd.h>

/// Problem 1

/// Method headers
void *cat_starter(void *data);
void *cat_thread(void *data);
void cat_enter(void);
void cat_exit(void);
void *dog_starter(void *data);
void *dog_thread(void *data);
void dog_enter(void);
void dog_exit(void);
void *bird_starter(void *data);
void *bird_thread(void *data);
void bird_enter(void);
void bird_exit(void);
void play(void);
int mutex_lock(void);
int mutex_unlock(void);

/// Globals
static pthread_cond_t animalConditions[3];
static pthread_mutex_t keeper;
static long numAnimals[3];
static volatile long playPen[3];
static volatile long enterCount[3];
static int running;
static pthread_t *catThreads;
static pthread_t *dogThreads;
static pthread_t *birdThreads;
//static pthread_t *animalThreads[3];

int main (int argc, const char *argv[])
{
    /// Init globals
    running = 1;

    /// Init mutex
    if (pthread_mutex_init(&keeper, NULL))
    {
        fprintf(stderr, "Awww sonuva jumpin' johnny on a haystack, our mutex didn't init. Go ahead and try again. Let's see if it don't work this time 'round.\n");
        return 1;
    }

    /// Error check argument count
    if (argc != 4)
    {
        fprintf(stderr, "Now listen here boy, I'm gonna need exactly three arguments outta you or we're gonna have some trouble. You hear?\n");
        return 1;
    }

    /// Get arguments
    int s;
    for (s = 0; s < 3; s++)
    {
        char *error;
        long n = strtol(argv[s+1],&error,0);
        if (n < 0 || n > 99 || (n == 0 && error == argv[s+1]))
        {
            fprintf(stderr, "Now we's a hard-workin' folk 'round here, and that's just why we's only lettin' in numerical arguments between 0-99 (inclusive). We got a reputation to uphold, you see?\n");
            return 1;
        }
        numAnimals[s] = n;
    }

    /// Allocate memory for threads and init condition variables
    catThreads = calloc(numAnimals[0],sizeof(pthread_t));
    dogThreads = calloc(numAnimals[1],sizeof(pthread_t));
    birdThreads = calloc(numAnimals[2],sizeof(pthread_t));
    if (!catThreads || !dogThreads || !birdThreads)
    {
        fprintf(stderr, "Listen up boy, there comes a time in every man's life when his memory allocation fails like your's did just now. And when that happens, you just gotta say 'Sonuvabitch' and try again.\n");
        return 1;
    }
    for (s = 0; s < 3; s++)
    {
        if (pthread_cond_init(&animalConditions[s],NULL))
        {
            fprintf(stderr, "Down here, we live and die by the condition variables boy. And well, if your condition variable don't init like just now, you just gotta keep tryin' 'til it does.\n");
            return 1;
        }
    }

    /// Start all threads
    pthread_t starters[3];
    if (pthread_create(&starters[0], NULL, cat_starter, NULL) || pthread_create(&starters[1], NULL, dog_starter, NULL) || pthread_create(&starters[2], NULL, bird_starter, NULL))
    {
        fprintf(stderr, "Darlin, one of your threads got alla tangled up when you tried to start it!\n");
        return 1;
    }

    /// Sleep for 10 seconds
    if(sleep(10))
    {
        fprintf(stderr, "Boy, I told you not to wake me up 'less it was sumthin' important!\n");
        return 1;
    }
    running = 0;

    /// Join all threads
    for (s = 0; s < 3; s++)
    {
        if (pthread_join(starters[s], NULL))
        {
            return 1;
        }
    }

    fprintf(stdout, "cats played %ld times. dogs played %ld times. birds played %ld times.\n", enterCount[0], enterCount[1], enterCount[2]);
}

void *cat_starter(void *data)
{
    int a;
    for (a = 0; a < numAnimals[0]; a++)
    {
        if (pthread_create(&catThreads[a], NULL, cat_thread, NULL))
        {
            fprintf(stderr, "Darlin, another one of your cat threads got alla tangled up when you tried to start it!\n");
            //a--;
            return NULL;
        }
    }
    for (a = 0; a < numAnimals[0]; a++)
    {
        if (pthread_join(catThreads[a], NULL))
        {
            fprintf(stderr, "Darn, one of dem threads cat went and got loose. See if you can't round it up again.\n");
            return NULL;
        }
    }
    return NULL;
}

void *cat_thread(void *data)
{
    while (running)
    {
        cat_enter();
        cat_exit();
    }
    return NULL;
}

void cat_enter(void)
{
    mutex_lock();

    while (playPen[1] || playPen[2])
    {
        pthread_cond_wait(&animalConditions[0], &keeper);
    }

    playPen[0]++;
    play();
    enterCount[0]++;

    mutex_unlock();
}

void cat_exit(void)
{
    mutex_lock();

    long catCount = enterCount[0];
    long dogBirdCount = (enterCount[1] + enterCount[2]) / 2;
    playPen[0]--;
    if (!playPen[0])
    {
        if (catCount < dogBirdCount)
        {
            pthread_cond_broadcast(&animalConditions[0]);
        }
        else
        {
            pthread_cond_broadcast(&animalConditions[1]);
            pthread_cond_broadcast(&animalConditions[2]);
        }
    }

    mutex_unlock();
}

void *dog_starter(void *data)
{
    int a;
    for (a = 0; a < numAnimals[1]; a++)
    {
        if (pthread_create(&dogThreads[a], NULL, dog_thread, NULL))
        {
            fprintf(stderr, "Darlin, another one of your dog threads got alla tangled up when you tried to start it!\n");
            //a--;
            return NULL;
        }
    }
    for (a = 0; a < numAnimals[1]; a++)
    {
        if (pthread_join(dogThreads[a], NULL))
        {
            fprintf(stderr, "Darn, one of dem dog threads went and got loose. See if you can't round it up again.\n");
            return NULL;
        }
    }
    return NULL;
}

void *dog_thread(void *data)
{
    while (running)
    {
        dog_enter();
        dog_exit();
    }
    return NULL;
}

void dog_enter(void)
{
    mutex_lock();

    while (playPen[0])
    {
        pthread_cond_wait(&animalConditions[1], &keeper);
    }

    playPen[1]++;
    play();
    enterCount[1]++;

    mutex_unlock();
}

void dog_exit(void)
{
    mutex_lock();

    playPen[1]--;
    if (!playPen[1] && !playPen[2])
    {
        pthread_cond_broadcast(&animalConditions[0]);
    }

    mutex_unlock();
}

void *bird_starter(void *data)
{
    int a;
    for (a = 0; a < numAnimals[2]; a++)
    {
        if (pthread_create(&birdThreads[a], NULL, bird_thread, NULL))
        {
            fprintf(stderr, "Darlin, another one of your bird threads got alla tangled up when you tried to start it!\n");
            //a--;
            return NULL;
        }
    }
    for (a = 0; a < numAnimals[2]; a++)
    {
        if (pthread_join(birdThreads[a], NULL))
        {
            fprintf(stderr, "Darn, one of dem bird threads went and got loose. See if you can't round it up again.\n");
            return NULL;
        }
    }
    return NULL;
}

void *bird_thread(void *data)
{
    while (running)
    {
        bird_enter();
        bird_exit();
    }
    return NULL;
}

void bird_enter(void)
{
    mutex_lock();

    while (playPen[0])
    {  
        pthread_cond_wait(&animalConditions[2], &keeper);
    }

    playPen[2]++;
    play();
    enterCount[2]++;

    mutex_unlock();
}

void bird_exit(void)
{
    mutex_lock();

    playPen[2]--;
    if (!playPen[1] && !playPen[2])
    {
        pthread_cond_broadcast(&animalConditions[0]);
    }

    mutex_unlock();
}

void play(void)
{
    int i;
    for (i=0; i<10; i++)
    {
        /// 0 = cats, 1 = dogs, 2 = birds
        assert(playPen[0] >= 0 && playPen[0] <= numAnimals[0]);
        assert(playPen[1] >= 0 && playPen[1] <= numAnimals[1]);
        assert(playPen[2] >= 0 && playPen[2] <= numAnimals[2]);
        assert(playPen[0] == 0 || playPen[1] == 0);
        assert(playPen[0] == 0 || playPen[2] == 0);
    }
}

int mutex_lock(void)
{
    if (pthread_mutex_lock(&keeper))
    {
        fprintf(stderr, "Come here boy, looks like some varmint been chewin' on our mutex and now it won't lock proper!\n");
        return 1;
    }
    return 0;
}

int mutex_unlock(void)
{
    if (pthread_mutex_unlock(&keeper))
    {
        fprintf(stderr, "Gahdarnit! This here mutex is stuck locked! How in the hell are we supposed to get outta here now?!\n");
        return 1;
    }
    return 0;
}

