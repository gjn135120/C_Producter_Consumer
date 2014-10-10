#include <queue>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <errno.h>
#define ERR_EXIT(m) \
    do { \
        perror(m);\
        exit(EXIT_FAILURE);\
    }while(0)
using namespace std;

//这是一份伪C++代码

pthread_mutex_t mutex; //互斥锁
pthread_cond_t full; //缓冲区有产品
pthread_cond_t empty; //有空闲空间
queue<int> Q;

void *producer(void *arg)
{
    while(1)
    {
        //上锁
        //放入产品
        //通知消费者
        //解锁
        pthread_mutex_lock(&mutex);
        while(Q.size() >= 5)
            pthread_cond_wait(&empty, &mutex);
        int num = rand()%100;
        Q.push(num);
        cout << "push " << num << endl;
        pthread_cond_signal(&full);
        pthread_mutex_unlock(&mutex);

        sleep(1);
    }
}

void *consumer(void *arg)
{
    while(1)
    {
        pthread_mutex_lock(&mutex);
        while(Q.empty())
            pthread_cond_wait(&full, &mutex);
        cout << "get " << Q.front() << endl;
        Q.pop();
        pthread_cond_signal(&empty);
        pthread_mutex_unlock(&mutex);

        sleep(2);
    }

}

int main(int argc, const char *argv[])
{
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&full, NULL);
    pthread_cond_init(&empty, NULL);

    srand(123345);

    pthread_t producers[3];
    pthread_t consumers[5];
    int i;
    for(i = 0; i < 3 ; ++i)
        pthread_create(&producers[i], NULL, producer, NULL);
    for(i = 0; i < 5; ++i)
        pthread_create(&consumers[i], NULL, consumer, NULL);


    for(i = 0; i < 3 ; ++i)
        pthread_join(producers[i], NULL);
    for(i = 0; i < 5; ++i)
        pthread_join(consumers[i], NULL);
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&full);
    pthread_cond_destroy(&empty);




    return 0;
}







