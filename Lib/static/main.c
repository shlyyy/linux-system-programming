#include "mymath.h"

int main()
{
    extern int myerrno;
    //   printf("1+1=%d\n", add(1,1));

    printf("10/0=%d, errno=%d\n", div(10, 0), myerrno);

    return 0;
}