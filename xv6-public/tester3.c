//allocate and free

#include "types.h"
#include "stat.h"
#include "user.h"
#include "fs.h"

int
main(int argc, char *argv[])
{
    int i=0;
    int *test;
    while (i < 10 )
{
test = (int *)malloc(4096 * 5000);
printf(1, "%d\n", i);
myMemory();
free(test);
printf(1,"\n freed %d",i);
myMemory();
printf(1,"\n");
i++;
}
printf(1,"test variable (ignore) %d",test);
exit();
}