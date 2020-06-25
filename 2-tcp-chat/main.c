#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <time.h>
#include <pthread.h>

typedef struct client
{
    uint8_t id;
    char *name;
    int fd;
    struct sockaddr_in socket;
    pthread_t thread;
} client_t;

uint32_t listenAddr = INADDR_ANY;
uint16_t port = 1234U;
uint32_t total_client = 0;
uint8_t client_count = 0;
const size_t max_client = 255, max_msg = 64;
client_t *clients[255] = {0};

char *atos(uint32_t ip, uint16_t port)
{
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

void handleClient(client_t *client)
{
    char *buf = malloc(max_msg), *reply = malloc(max_msg);
    size_t len;
    client_count++;
    printf("\x1b[33mHandle [%d] %s\x1b[0m\n",
           client->id,
           atos(
               ntohl(client->socket.sin_addr.s_addr),
               ntohs(client->socket.sin_port)));
    write(client->fd, "Your Name: ", 11);
    while (1)
    {
        memset(buf, 0, 64);
        len = read(client->fd, buf, max_msg);
        if (len == 0)
            break;
        printf("messg [%d] %lu: ", client->id, len);
        if (len < 0)
        {
            printf("read fail\n");
        }
        else
        {
            if (!client->name)
            {
                // just 10 bytes :-)
                client->name = calloc(10, 1);
                // remove cr
                strncpy(client->name, buf, len - 1);
                for (int i = 0; i <= max_client; i++)
                {
                    if (clients[i])
                    {
                        len = sprintf(reply, "%d: %s\n", clients[i]->id, clients[i]->name);
                        write(client->fd, reply, len);
                    }
                }
                len = sprintf(reply, "** Online %d **\n", client_count);
                write(client->fd, reply, len);
            }
            else
            {
                //Broad cast
                for (int i = 0; i <= max_client; i++)
                {
                    if (clients[i] && i != client->id)
                    {
                        len = sprintf(reply, "%s: %s", client->name, buf);
                        write(clients[i]->fd, reply, len);
                    }
                }
            }
        }
    }
    close(client->fd);
    printf("\x1b[32mClose [%d] %s\x1b[0m\n",
           client->id,
           atos(
               ntohl(client->socket.sin_addr.s_addr),
               ntohs(client->socket.sin_port)));
    clients[client->id] = NULL;
    free(client->name);
    free(client);
    client_count--;
}

void handleConnection(int *fd)
{
    client_t *client;
    unsigned int sockSize = sizeof(struct sockaddr_in);
    unsigned int recvAddrLen;

    while (1)
    {
        client = calloc(sizeof(client_t), 1);
        recvAddrLen = sockSize;
        client->fd = accept(*fd, (struct sockaddr *)&client->socket, &recvAddrLen);
        if (client->fd < 0)
        {
            printf("%lu: Fail accepting new connection\n", time(NULL));
            close(*fd);
            free(client);
        }
        else
        {
            client->id = total_client++;
            clients[client->id] = client;
            pthread_create(&client->thread, NULL, (void *)&handleClient, (void *)client);
            printf("Accepted client %lu, count %d\n",
                   client->id, client_count);
        }
    }
}
int main(int argc, char const *argv[])
{
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in serveraddr;

    if (fd == -1)
    {
        fprintf(stderr, "Fail creating socket\n");
        return 1;
    }
    memset(&serveraddr, 0, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = htonl(listenAddr);
    serveraddr.sin_port = htons(port);
    printf("Listening on %s ", atos(listenAddr, port));

    if (bind(fd, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) == 0)
    {
        if (listen(fd, 5) == 0)
        {
            printf("OK\n");
            handleConnection(&fd);
        }
        else
        {
            printf("Failed to listen\n");
        }
    }
    else
    {
        printf("Fail bind\n");
    }

    /* code */
    close(fd);
    return 0;
}
