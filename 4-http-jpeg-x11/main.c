#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/sysinfo.h>
#include <netdb.h>
#include <arpa/inet.h>

#include "decode_jpeg.h"

#define OK 1
#define FAIL 0
#define MAX_URL 512
#define BUFFER_BLOCK_SIZE 10240
#define CHUNKED -2

typedef struct http_header_line
{
    char *key;
    char *value;
} http_header_line_t;

typedef struct http_header_status
{
    char *version;
    int code;
    char *message;
} http_header_status_t;

typedef struct response
{
    http_header_status_t status;
    http_header_line_t **headers;
    size_t received;
    ssize_t body_size;
    char *type;
    char *body;
} response_t;

typedef struct sockaddr_in addrin;
uint16_t total_alloc = 0;
void print_addr(addrin *addr)
{
    uint32_t ip = ntohl(addr->sin_addr.s_addr);
    uint16_t port = ntohs(addr->sin_port);
    printf("%d.%d.%d.%d:%d",
           ip >> 24 & 0xff,
           ip >> 16 & 0xff,
           ip >> 8 & 0xff,
           ip >> 0 & 0xff,
           port);
}

int log_malloc(void **dst, size_t size, const char *log)
{
    printf("Alloc %s (%lu bytes) ", log, size);
    *dst = calloc(size, 1);
    total_alloc += size;
    if (*dst == NULL)
    {
        printf("FAIL\n");
        return FAIL;
    }
    else
    {
        printf("OK %p\n", *dst);
        return OK;
    }
}

int parse_headers(char *const buf, response_t *const result)
{

    char *start_header, *pointer;
    int index, header_lines = 0;
    http_header_line_t *line_ptr;
    result->body_size = -1;
    // HTTP status code
    result->status.version = strtok(buf, " ");
    result->status.code = atoi(strtok(NULL, " "));
    result->status.message = strtok(NULL, "\r\n");
    printf("HTTP_VERSION: %s\nHTTP_CODE: %d\nHTTP_MSG: %s\n",
           result->status.version,
           result->status.code,
           result->status.message);

    assert(result->status.code > 0);

    start_header = strchr(result->status.message, 0) + 2;
    pointer = start_header;
    while (*pointer)
    {
        //printf("%02X %c\n", *pointer, *pointer);
        if (*(pointer++) == '\r' && *(pointer++) == '\n')
        {
            header_lines++;
            // Nulled 2 bytes CR LF
            memset(pointer - 2, 0, 2);
            if (*pointer == '\r' && *(pointer + 1) == '\n')
            {
                // Nulled 2 bytes CR LF
                memset(pointer, 0, 2);
                // point to the start of body
                result->body = pointer + 2;
                break;
            }
        }
    }
    printf("Header lines: %d\n", header_lines);
    // +1 for null pointer
    if (!log_malloc((void **)&result->headers, sizeof(http_header_line_t *) * (header_lines + 1), "headers ptrs"))
    {
        return FAIL;
    }

    index = 0;
    pointer = start_header;
    while (*pointer && index < header_lines)
    {
        log_malloc((void **)&line_ptr, sizeof(http_header_line_t), "header-line");
        line_ptr->key = strtok(pointer, ":");
        // the char ':' is nulled by strtok
        line_ptr->value = strchr(pointer, 0) + 1;
        //trim space
        while (*line_ptr->value == ' ')
        {
            line_ptr->value++;
        }
        if (strcmp(line_ptr->key, "Content-Length") == 0)
        {
            result->body_size = atol(line_ptr->value);
        }
        else if (strcmp(line_ptr->key, "Transfer-Encoding") == 0 &&
                 strcmp(line_ptr->value, "chunked") == 0)
        {
            result->body_size = CHUNKED;
        }
        else if (strcmp(line_ptr->key, "Content-Type") == 0)
        {
            result->type = line_ptr->value;
        }
        //printf("C %s = %s\n", line_ptr->key, line_ptr->value);
        pointer = strchr(line_ptr->value, 0) + 2;
        result->headers[index++] = line_ptr;
    }
    result->headers[header_lines] = NULL;
    // index = 0;
    // while ((line_ptr = result->headers[index]) != NULL)
    // {
    //     printf("H> %p %s = %s\n", line_ptr, line_ptr->key, line_ptr->value);
    //     index++;
    // }
    return OK;
}
void parse_chunked_body(response_t *result)
{
    printf("Parsing chungked body\n");
    size_t offset = 0, chunk_size;
    result->body_size = 0;
    char *len_str;
    int len_str_len = 0;
    while (*(result->body + offset))
    {
        len_str = strtok(result->body + offset, "\r\n");
        len_str_len = strlen(len_str);
        chunk_size = strtoul(len_str, NULL, 16);
        // printf("%lu %lu %d %lu\n", offset, result->body_size, len_str_len, chunk_size);
        if (len_str_len == 0 || chunk_size == 0)
        {
            memset(result->body + result->body_size, 0, len_str_len);
            break;
        }
        offset += len_str_len + 2;
        memcpy(result->body + result->body_size, result->body + offset, chunk_size);
        result->body_size += chunk_size;
        offset += chunk_size;
        // printf("X: %s\n", result->body + offset);
    }
}
response_t *http_download(const char *input_url)
{
    /* These variable will be on the stack and cleared after exit the function */
    size_t usize, buf_size;
    ssize_t size, tmp;
    /* just pointer, no alloc for this */
    const char *path;
    /* Dynamicaly allocated variable */
    char *buf, *url, *protocol, *host;
    response_t *result;
    addrin *server;
    struct hostent *hostinfo;

    if (strncmp(input_url, "http://", 7) != 0)
    {
        printf("Unsupported protocol\n");
        return NULL;
    }
    if (strlen(input_url) >= MAX_URL)
    {
        printf("URL too long\n");
        return NULL;
    }

    if (!log_malloc((void **)&server, sizeof(addrin), "sockaddr"))
        return NULL;

    if (!log_malloc((void **)&result, sizeof(response_t), "result"))
        return NULL;

    if (!log_malloc((void **)&url, MAX_URL, "url"))
        return NULL;

    strcpy(url, input_url);

    protocol = strtok(url, "://");
    printf("PROTOCOL: %s\n", protocol);

    host = strtok(NULL, "/");
    printf("HOST: %s\n", host);
    // Just a slice of input url, start after host
    path = input_url + strlen(protocol) + 3 + strlen(host);
    // OR
    // path = strpbrk(input_url + strlen(protocol) + 3, "/");

    printf("PATH: %s\n", path);

    printf("Resolving ip.. ");
    hostinfo = gethostbyname(host);
    if (hostinfo == NULL)
    {
        printf("Canot resolve hostname\n");
        return NULL;
    }
    else
    {
        printf("OK\n");
    }

    // get first address
    server->sin_addr.s_addr = *(in_addr_t *)hostinfo->h_addr_list[0];
    server->sin_port = htons(80);
    server->sin_family = AF_INET;
    printf("Connecting to: ");
    print_addr(server);

    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0)
    {
        printf(" Cannot open fd\n");
        return NULL;
    }
    if (connect(fd, (struct sockaddr *)server, sizeof(addrin)) == 0)
    {
        printf(" OK\n");
    }
    else
    {
        printf(" Failed\n");
        close(fd);
        return NULL;
    }

    // allocating buffers and can grow dynamically
    buf_size = BUFFER_BLOCK_SIZE;
    if (!log_malloc((void **)&buf, buf_size, "buffers"))
        return NULL;

    size = sprintf(buf, "GET %s HTTP/1.1\r\nHost: %s\r\nConnection: close\r\n\r\n", path, host);
    printf("Sending HTTP request %lu bytes\n------\n%s", size, buf);
    usize = send(fd, buf, size, 0);
    if (size != usize)
    {
        printf("Expect write %lu but only %lu\n", size, usize);
        return NULL;
    }
    else
    {
        printf(">> SENT\n");
    }
    size = -1;
    // First, accept all response, store it on buffer
    while (size)
    {
        // last 1 byte is for null
        tmp = buf_size - (result->received + 1);
        size = recv(fd, buf + result->received, tmp, 0);
        buf[result->received + size] = 0;
        result->received += size;
        if (buf_size - result->received < BUFFER_BLOCK_SIZE)
        {
            buf_size += BUFFER_BLOCK_SIZE;
            buf = realloc(buf, buf_size);
            printf("Realloc buffers (%lu bytes)\n", buf_size);
        }
    }

    if (!parse_headers(buf, result))
    {
        return NULL;
    }
    if (result->body_size == CHUNKED)
    {
        parse_chunked_body(result);
    }
    printf("Done %lu bytes\n", result->received);
    //printf("---\n%s\n---\n", result->body);

    // Tear down
    shutdown(fd, SHUT_RDWR);
    close(fd);
    free(server);
    free(url);
    return result;
}

int main(int argc, char const *argv[])
{
    response_t *result;
    if (argc <= 1)
    {
        printf("No url.\n");
        return 1;
    }
    result = http_download(argv[1]);
    if (result == NULL)
    {
        return 1;
    }
    if (strcmp(result->type, "image/jpeg") != 0)
    {
        printf("Not an image, got %s\n", result->type);
        return 1;
    }
    if (decode_jpeg(result->body, result->body_size) != 0)
    {
        return 1;
    }
    // FILE *f = fopen("image.jpg", "w");

    // fwrite(result->body, 1, result->body_size, f);
    // fclose(f);
    /* code */
    return 0;
}
