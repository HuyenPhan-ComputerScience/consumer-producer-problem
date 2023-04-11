#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>

void *producer(void *params);
void *consumer(void *params);
#define MAX_ITEM 5
pthread_mutex_t mutex;

int buffer[MAX_ITEM];
int insertpoint = 0;
int removepoint = 0;
sem_t semEmpty;
sem_t semFull;

int main(int argc, const char *argv[])
{
    // .homework8 sleepTime numberProducer numberConsumer
    if (argc < 4 || argc > 4)
    {
        printf("user need to enter 3 arguments");
        return 1;
    }

    int sleeptime = atoi(argv[1]);
    int numberProducer = atoi(argv[2]);
    int numberConsumer = atoi(argv[3]);
    if (sleeptime == 0 || numberProducer == 0 || numberConsumer == 0)
    {
        printf("user need to enter interger as in input");
        return 1;
    }

    pthread_t producerWorkers[numberProducer];
    pthread_t consumerWorkers[numberConsumer];
    pthread_mutex_init(&mutex, NULL);
    sem_init(&semEmpty, 0, 5);
    sem_init(&semFull, 0, 0);

    for (int i = 0; i < numberProducer; i++)
    {
        pthread_create(&producerWorkers[i], 0, producer, NULL);
    }

    for (int i = 0; i < numberConsumer; i++)
    {
        pthread_create(&consumerWorkers[i], 0, consumer, NULL);
    }
    sleep(sleeptime);
    printf("%d\n", insertpoint);
    sem_destroy(&semEmpty);
    sem_destroy(&semFull);
    pthread_mutex_destroy(&mutex);

    return 0;
}
void *producer(void *params)
{

    // loop infinity
    // 1. sleep
    // 2. generate random number
    // 3. insert item
    // print inserted item

    while (1)
    {
        sleep(random() % 5);
        int number = rand() % 20;
        sem_wait(&semEmpty);
        pthread_mutex_lock(&mutex);
        buffer[insertpoint] = number;
        insertpoint++;
        printf("produce this item:%d\n", number);

        // insertpoint = insertpoint % 5;
        pthread_mutex_unlock(&mutex);
        sem_post(&semFull);
    }
}
void *consumer(void *params)
{
    while (1)
    {

        // loop infi...
        // 1. sleep
        // 3. remove the item
        // 4. if successful -> print the removed item
        sleep(random() % 5);
        sem_wait(&semFull);
        pthread_mutex_lock(&mutex);
        int y = buffer[insertpoint - 1];
        insertpoint--;
        pthread_mutex_unlock(&mutex);
        sem_post(&semEmpty);
        printf("item consume is %d\n", y);
    }
}
