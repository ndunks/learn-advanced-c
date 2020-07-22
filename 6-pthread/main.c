#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>

void *busy_thread(void *param)
{
    char *retval = malloc(64);
    int len;
    pthread_t me = pthread_self();
    printf("thread start %x\n", me);

    len = sprintf(retval, "%lu", time(NULL));
    retval[len] = ' ';
    sleep(5);
    len = sprintf(retval + len + 1, "%lu", time(NULL));
    printf("thread exit %x\n", me);
    pthread_exit(retval);
}

int main(int argc, char const *argv[])
{
    char *retval;
    pthread_t me = pthread_self(), thread;
    printf("main start %x\n", me);

    pthread_create(&thread, NULL, busy_thread, NULL);
    pthread_join(thread, &retval);
    printf("RET: %s\n", retval);
    printf("main exit %x\n", me);
}
