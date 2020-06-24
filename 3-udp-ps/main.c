#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/sysinfo.h>

char *atos(struct sockaddr_in *addr)
{
    uint32_t ip = ntohl(addr->sin_addr.s_addr);
    uint16_t port = ntohs(addr->sin_port);
    // maks: xxx.xxx.xxx.xxx:xxxxx
    char *buff = malloc(22);
    sprintf(buff, "%d.%d.%d.%d:%d",
            ip >> 24 & 0xff,
            ip >> 16 & 0xff,
            ip >> 8 & 0xff,
            ip >> 0 & 0xff,
            port);
    return buff;
}

void *sysmon(struct sysinfo *info)
{
    while (1)
    {
        sysinfo(info);
        sleep(1);
    }
    printf("sysmon exit\n");
    pthread_exit(NULL);
}

int main(int argc, char const *argv[])
{

    int size, sock_size = sizeof(struct sockaddr_in);
    ssize_t recv_size;
    struct sockaddr_in server, *client = malloc(sock_size);
    struct sysinfo info;
    pthread_t sysmon_thread;
    char *buf = calloc(512, 1);
    pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    server.sin_port = htons(1234);

    int fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (fd < 0)
    {
        printf("Cannot open fd\n");
        return 1;
    }
    printf("Listening on %s ", atos(&server));
    if (bind(fd, (struct sockaddr *)&server, sock_size) == 0)
    {
        // Starting thread
        if (pthread_create(&sysmon_thread, NULL, (void *)sysmon, (void *)&info) != 0)
        {
            printf("Cannot start sysmon thread\n");
        }
        else
        {
            printf("OK\n");
            while (1)
            {
                size = sock_size;
                memset(client, 0, sock_size);
                recv_size = recvfrom(fd, buf, 512, 0, (struct sockaddr *)client, &size);
                // replace cr with null
                buf[recv_size - 1] = 0;
                pthread_mutex_lock(&mutex1);
                printf("recv %d %d %s: %s\n", size, recv_size, atos(client), buf);
                recv_size = sprintf(buf, "Total: %lu, Free: %lu, Procs: %u\n", info.totalram, info.freeram, info.procs);
                sendto(fd, buf, recv_size, 0, (struct sockaddr *)client, size);
                pthread_mutex_unlock(&mutex1);
            }
        }
    }
    else
    {
        printf("FAIL\n");
    }

    pthread_join(sysmon_thread, NULL);
    close(fd);
    printf("I'm quit\n");
    /* code */
    return 0;
}
