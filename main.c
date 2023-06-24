#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

int cnt;
void* threadFunct(void* arg)
{
    int i, a;
    printf("Tread: %lu\n",pthread_self());
    cnt = cnt + 1;
    a = 0;
    for(i = 0; i< 100000000; i++)
    {
        a++;
    }
    pthread_exit(NULL);

}

int main()
{
    int i;
    pthread_t ptid[2000];

    printf("Before threads!\n");
    cnt = 0;
    for(i = 0; i< 2000; i++)
    {
        //threadFunct(NULL);
        pthread_create(&ptid[i], NULL, &threadFunct, NULL);
    }
    for(i = 0; i< 2000; i++)
    {
        pthread_join(ptid[i], NULL);
    }
    printf("Cnt: %d\n", cnt);
    printf("Goodbye\n");
    return 0;
}
