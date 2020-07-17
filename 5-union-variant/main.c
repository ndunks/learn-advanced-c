#include <stdio.h>
#define SHOW(expression, fmt) printf(#expression " = " fmt "\n", (expression))
#define LINE printf("-------------\n")
#define HEXDUMP(var)                                     \
    do                                                   \
    {                                                    \
        printf(#var " (%lu) :", sizeof(var));            \
        for (int i = 0; i < sizeof(var); i++)            \
            printf(" %02x", ((unsigned char *)&var)[i]); \
        printf("\n");                                    \
    } while (0)

union number8 {
    char int8;
    unsigned char int8_u;
} number8;

union number16 {
    char int8;
    unsigned char int8_u;
    short int16;
    unsigned short int16_u;
} number16;

union number32 {
    char int8;
    unsigned char int8_u;
    short int16;
    unsigned short int16_u;
    int int32;
    unsigned int int32_u;
} number32;

union number64 {
    char int8;
    unsigned char int8_u;
    short int16;
    unsigned short int16_u;
    int int32;
    unsigned int int32_u;
    long int64;
    unsigned long int64_u;
} number64;

int main(int argc, char const *argv[])
{

    printf("char %lu, short %lu, int %lu, long %lu, \n",
           sizeof(char), sizeof(short), sizeof(int), sizeof(long));
    LINE;
    SHOW(sizeof(number8), "%lu");
    SHOW(sizeof(number16), "%lu");
    SHOW(sizeof(number32), "%lu");
    SHOW(sizeof(number64), "%lu");
    LINE;
    SHOW(number8.int8 = -1, "%hd");
    SHOW(number8.int8_u = -1, "%hd");
    SHOW(number8, "%hd");
    SHOW(number8.int8, "%hd");
    SHOW(number8.int8_u, "%hd");
    LINE;
    SHOW(number64.int8 = 256, "%d");
    SHOW(number64.int8, "%d");
    SHOW(number64.int16, "%d");
    LINE;
    SHOW(number64.int8 = 0xfdfe, "%d");
    SHOW(number64.int8, "%d");
    SHOW(number64.int16, "%d");
    SHOW(number64.int32, "%d");
    HEXDUMP(number64);
    LINE;
    SHOW(number64.int16 = 0xffff, "%d");
    SHOW(number64.int8, "%d");
    SHOW(number64.int16, "%d");
    SHOW(number64.int32, "%d");
    SHOW(number64.int64, "%lu");
    HEXDUMP(number64);
    LINE;
    char *ptr_char = "\xff\x00\xff\xff\x00\xff\x00";
    union number64 *ptr_number64;
    ptr_number64 = (void *)ptr_char;
    SHOW(*ptr_number64, "%1$lu %1$x");
    SHOW(ptr_number64->int8_u, "%u");
    SHOW(ptr_number64->int16_u, "%u");
    SHOW(ptr_number64->int32_u, "%u");
    SHOW(ptr_number64->int64_u, "%lu");
    HEXDUMP(*ptr_number64);
}
