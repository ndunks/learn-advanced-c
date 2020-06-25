#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/sysinfo.h>
#include <arpa/inet.h>


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
struct thread_param
{
    int *fd;
    struct sockaddr_in *server;
};

int main(int argc, char const *argv[])
{

    int size, broadcastEnable = 1, sock_size = sizeof(struct sockaddr_in);
    ssize_t recv_size;
    char *buf = calloc(512, 1);
    struct sockaddr_in sockbc, *client = malloc(sock_size);
    struct sysinfo info;

    sockbc.sin_family = AF_INET;
    sockbc.sin_addr.s_addr = INADDR_BROADCAST;
    sockbc.sin_port = htons(1234);

    int fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (fd < 0)
    {
        printf("Cannot open fd\n");
        return 1;
    }

    if (setsockopt(fd, SOL_SOCKET, SO_BROADCAST, &broadcastEnable, sizeof(broadcastEnable)) == 0)
    {
        while (1)
        {
            sleep(2);
            sysinfo(&info);
            size = sprintf(buf, "Total: %lu, Free: %lu, Procs: %u\n", info.totalram, info.freeram, info.procs);
            sendto(fd, buf, size, 0, (struct sockaddr *)&sockbc, sock_size);
        }
    }
    else
    {
        printf("Cannot enable broadcast fd\n");
    }

    close(fd);
    printf("I'm quit\n");
    /* code */
    return 0;
}
