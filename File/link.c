#include <func.h>
int main(int argc, char *argv[])
{
    ARGS_CHECK(argc, 3);
    
    int ret = link(argv[1], argv[2]);
    ERROR_CHECK(ret, -1, "link");
    return 0;
}