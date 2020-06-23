#include <stdio.h>
#include <stdlib.h>
#include <string.h>

__uint8_t passMaxLen = 10;

char *getPassword()
{
    char *pwd = malloc(passMaxLen);
    pwd[0] = 'k';
    pwd[1] = 'e';
    pwd[2] = 'e';
    pwd[3] = 'p';
    pwd[10] = 0;
    return pwd;
}

int checkPassword(char *pass)
{
    char *pwd = malloc(passMaxLen);
    strcpy(pwd, pass);
    return strcmp(getPassword(), pwd) == 0;
}

int main(int argc, char *argv[])
{
    if (argc == 1)
    {
        printf("No password input\n");
    }else if (checkPassword(argv[1]))
    {
        printf("OK\n");
        return 0;
    }
    else
    {
        printf("Wrong\n");
    }
    return 0;
}