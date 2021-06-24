#include <stdio.h>
#include <stdlib.h>
#include <math.h>
int main()
{

  unsigned char buf[] = {128, 156, 100, 138, 118, 28, 128, 178, 131, 130, 127, 124, 117, 116, 115, 115,
                         116, 117, 119, 123, 125, 128, 135, 141, 145, 147, 145, 142, 141, 138, 137, 134,
                         127, 124, 122, 119, 155, 113, 114, 117};

  int i;
  for (i = 0; i < sizeof(buf); i++)
  {
    printf("%3u %3d %3u\n", buf[i], ((signed char) buf[i] ^ 128), abs(buf[i] - 128));
  }
  return 0;
}
