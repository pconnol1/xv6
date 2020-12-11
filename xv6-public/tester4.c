//checking with child and parent
//log in child and parent
#include "types.h"
#include "stat.h"
#include "user.h"
#include "fs.h"

int
main(int argc, char *argv[])
{
int *test = (int *)malloc(4096 * 8000);
printf(1, "%d\n", test);
printf(1,"displaying memory before fork()");
myMemory();
if (fork()==0)
{
    int *test = (int *)malloc(4096 * 8  000);
    printf(1,"displaying memory allocation in child");
    myMemory();
    printf(1,"Ignore:%d",test);
    exit();
}
else{
    wait();
    printf(1,"\n parent memory after child process \n");
    myMemory();

}
printf(1,"Ignore:%d",test);
exit();
}