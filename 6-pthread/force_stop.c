#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>
#include <malloc.h>

char buf[255] = {0};

void *busy_thread(void *param)
{
    char *retval = malloc(64);
    int len;
    pthread_t me = pthread_self();

    printf("thread start %lx\n", me);
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);

    scanf("%255s", buf);

    len = sprintf(retval, "%lu", time(NULL));
    retval[len] = ' ';
    
    len = sprintf(retval + len + 1, "%lu", time(NULL));
    printf("thread exit %lx\n", me);
    pthread_exit(retval);
}

int main(int argc, char const *argv[])
{
    char *retval = NULL;
    int ret;
    pthread_t me = pthread_self(), thread;
    printf("main start %lx\n", me);

    pthread_create(&thread, NULL, busy_thread, NULL);
    sleep(5);
    ret = pthread_cancel(thread);
    pthread_join(thread, (void *)&retval);
    printf("canceled %d\n", ret);
    printf("RET: %p\n", retval);
    printf("main exit %lx\n", me);
    printf("Thread read: %s\n", buf);
}
