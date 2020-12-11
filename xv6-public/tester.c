#include "types.h"
#include "stat.h"
#include "user.h"
#include "fs.h"

int
main(int argc, char *argv[])
{
int *test = (int *)malloc(4096 * 8000);
printf(1, "%d\n", test);
myMemory();
exit();
}