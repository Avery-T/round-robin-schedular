#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
   char buff[1024];
   int cnt, i;
   struct timeval tv;

   tv.tv_usec = 0;
   tv.tv_sec = 1;

   cnt = atol(argv[1]);

   sprintf(buff, "%%%dd\n", 8*cnt);
   for(i = 0; i < cnt; i++) {
      printf(buff, cnt);
      fflush(stdout);
      sleep(1);
   }
   return 0;
}
