// Author- Kavish Lodha-kavishlodha123 
#include<stdio.h>
#include<pthread.h> 
#include<semaphore.h>
#include<math.h>
sem_t Queue_mtx;

pthread_mutex_t Reader_mtx , Writer_mtx;//Mutexs
pthread_t Readers[10], Writers[10];//Threads of readers and writers
int LabelArray[10];         //Labels Array
int Resource = 1;          //Resource
unsigned int ReadersCount = 0;  //Count of readers
//Reader Function
void *Reader_Func(void *Reader_idx)
{
    sem_wait(&Queue_mtx);

    pthread_mutex_lock(&Reader_mtx);
    if (ReadersCount == 0)
        pthread_mutex_lock(&Writer_mtx);
    ReadersCount++;

    sem_post(&Queue_mtx);
    pthread_mutex_unlock(&Reader_mtx);

    printf("Reader %d reads Resource as %d\n",*((int *)Reader_idx),Resource);

    pthread_mutex_lock(&Reader_mtx);
    ReadersCount--;
    if( ReadersCount == 0 )
        pthread_mutex_unlock(&Writer_mtx);
    pthread_mutex_unlock(&Reader_mtx);

    return Reader_idx;
}
//Writer Function
void *Writer_Func(void* Writer_idx)
{
    sem_wait(&Queue_mtx);
    pthread_mutex_lock(&Writer_mtx);
    sem_post(&Queue_mtx);

    Resource  = pow(2,*((int *)Writer_idx));
    printf("Writer %d modifies Resource as %d\n",*((int *)Writer_idx),Resource);

    pthread_mutex_unlock(&Writer_mtx);

    return Writer_idx;
}
//Initialize LabelArray Function
void init()
{
    for(int i = 0; i < 10; i++) 
    {
        LabelArray[i]=i;
        LabelArray[i]++;
    }
}
//Create Thread Function
void CreateThreads()
{
    for(int i = 0; i < 10; i++) 
    {
        pthread_create(&Readers[i], NULL, Reader_Func, (void *)&LabelArray[i]);
        pthread_create(&Writers[i], NULL, Writer_Func, (void *)&LabelArray[i]);
    }
}
//Join Thread Function
void JoinThreads()
{
    for(int i = 0; i < 10; i++) 
    {
        pthread_join(Readers[i], NULL);
        pthread_join(Writers[i], NULL);
    }
}
int main()
{
    
    //Initialization of LabelArray
    init();

    //Initialization of Reader and Writer Mutex
    pthread_mutex_init(&Reader_mtx, NULL);
    pthread_mutex_init(&Writer_mtx , NULL);

    //Initialization of semaphore
    sem_init(&Queue_mtx,0,1);

    //Create thread function called
    CreateThreads();
    
    //Join thread function called
    JoinThreads();
    

    //Destroying threads and semaphores
    pthread_mutex_destroy(&Writer_mtx);
    pthread_mutex_destroy(&Reader_mtx);
    
    sem_destroy(&Queue_mtx);

    return 0;
}