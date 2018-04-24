#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <assert.h>

/// Animal thread struct
typedef struct animal_data
{
    char species;
    int playCount;
} animal_data;

/// Method headers
void *thread_starter(void *data);
void *cat_thread(void *data)
void cat_enter();
void cat_exit();
void *dog_thread(void *data);
void dog_enter();
void dog_exit();
void *bird_thread(void *data);
void bird_enter();
void bird_exit();
void play(void);
void mutex_lock();
void mutex_unlock();

/// Globals
pthread_cond_t[3] animalConditions;
pthread_mutex_t keeper;
long[3] numAnimals;
long[3] playPen;
long[3] enterCount;
int running;
animal_data *cats;
animal_data *dogs;
animal_data *birds;
animal_data *animals[3] = {cats, dogs, birds};
pthread_t *catThreads;
pthread_t *dogThreads;
pthread_t *birdThreads;
pthread_t *animalThreads[3] = {catThreads, dogThreads, birdThreads};

int main (int argc, const char *argv[])
{
    running = 0;
    if (pthread_mutex_init(&keeper, NULL))
    {
        "Awwws sonuva jumpin' johnny on a haystack, our mutex didn't init. Go ahead and try again. We'll see if it don't work this time 'round neither.\n");
        return 1;
    }

    if (argc != 4)
    {
        fprintf(stderr, "Now listen here boy, I'm gonna need exactly three arguments outta you or we're gonna have some trouble. You hear?\n");
        return 1;
    }

    int s;
    for (s = 0; s < 3; s++)
    {
        char *error;
        long n = strtol(argv[s+1],error,0);
        if (n < 0 || n > 99 || (n == 0 && error == argv[s+1]))
        {
            fprintf(stderr, "Now we's a hard-workin' folk 'round here, and that's just why we's only lettin' in numerical arguments between 0-99 (inclusive). We got a reputation to uphold, you see?\n");
            return 1;
        }
        numAnimals[s] = n;
    }

    for (s = 0; s < 3; s++)
    {
        animals[s] = calloc(numAnimals[s],sizeof(animal_data));
        animalThreads[s] = calloc(numAnimals[s],sizeof(pthread_t));
        if (!animals[s] || !animalThreads[s])
        {
            fprintf(stderr, "Listen up boy, there comes a time in every man's life when his memory allocation failed like your's did just now. And when that happens, you just gotta say 'Sonuvabitch' and try again.\n");
            return 1;
        }
        int a;
        for (a = 0; a < numAnimals[s]; a++)
        {
            animal_data animal = {s, 0};
            animals[s][a] = animal;
            pthread_t animalThread;
            animalThreads[s][a] = animalThread;
        }
        if (pthread_cond_init(&animalConditions[s],NULL))
        {
            fprintf(stderr, "Down here, we live and die by the condition variables boy. And well, if your condition variable don't init like just now, you just gotta keep tryin' 'til it does.\n");
            return 1;
        }
    }

    for (s = 0; s < 3; s++)
    {
        pthread_t starter;
        int species = s;
        if (pthread_create(&starter, NULL, thread_starter, &species))
        {
            fprintf(stderr, "Darlin, one of your darn threads got alla tangled up when you tried to start it!\n");
            return 1;
        }
    }

    if(sleep(10))
    {
        fprintf(stderr, "Boy, I told you not to wake me up less it was sumthin' important!\n");
        return 1;
    }
    running = 0;

    for (s = 0; s < 3; s++)
    {
        /// LEFT OFF HERE
    }
}

void *thread_starter(void *data)
{
    int *species = (int *)species;
    int a;
    for (a = 0; a < numAnimals[*species]; a++)
    {
        void *(*startRoutine) (void *);
        if (*species == 0)
            startRoutine = cat_thread;
        else if (*species == 1)
            startRoutine = dog_thread;
        else if (*species == 2)
            startRoutine = bird_thread;

        if (pthread_create(&animalThreads[*species], NULL, startRoutine, &animals[*species][a]))
        {
            fprintf(stderr, "Darlin, another one of your darn threads got alla tangled up when you tried to start it!\n");
            return 1;
        }
    }
}

void *cat_thread(void *data)
{
    animal_data *animal = (animal_data *)data;
    while (running)
    {
        cat_enter();
        cat
    }
}

void cat_enter();
{
    mutex_lock();

    mutex_unlock();
}

void cat_exit()
{
    mutex_lock();

    mutex_unlock();
}

void *dog_thread(void *data)
{
    
}

void dog_enter()
{
    mutex_lock();

    mutex_unlock();
}

void dog_exit()
{
    mutex_lock();

    mutex_unlock();
}

void *bird_thread(void *data)
{

}

void bird_enter()
{
    mutex_lock();

    mutex_unlock();
}

void bird_exit()
{
    mutex_lock();

    mutex_unlock();
}

void play(void)
{
    for (int i=0; i<10; i++)
    {
        /// 0 = cats, 1 = dogs, 2 = birds
        assert(playPen[0] >= 0 && playPen[0] <= numAnimals[0]);
        assert(playPen[1] >= 0 && playPen[1] <= numAnimals[1]);
        assert(playPen[2] >= 0 && playPen[2] <= numAnimals[2]);
        assert(playPen[0] == 0 || playPen[1] == 0);
        assert(playPen[0] == 0 || playPen[2] == 0);
    }
}

void mutex_lock()
{
    if (pthread_mutex_lock(&keeper))
    {
        fprintf(stderr, "Come here boy, looks like some varmint been chewin' on our mutex and now it won't lock proper!\n");
        return 1;
    }
}

void mutex_unlock()
{
    if (pthread_mutex_unlock(&keeper))
    {
        fprintf(stderr, "Gahdarnit! This here mutex is stuck locked! How in the hell are we supposed to get outta here now?!\n");
        return 1;
    }
}

