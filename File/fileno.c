#include <func.h>

int main(int argc, char *argv[])
{
    // ./fileno file
    ARGS_CHECK(argc, 2);

    FILE *fp = fopen(argv[1], "r+");
    ERROR_CHECK(fp, NULL, "fopen");

    write(fileno(fp), "world", 5);

    fclose(fp);
    return 0;
}