#include <func.h>

int main(int argc, char *argv[])
{
    // ./text_bin file
    ARGS_CHECK(argc, 2);

    FILE *fp = fopen(argv[1], "r+");
    ERROR_CHECK(fp, NULL, "fopen");

    // char buf[] = "1000000";
    // fwrite(buf, 1, strlen(buf), fp);

    int i = 1000000;
    fwrite(&i, sizeof(int), 1, fp);

    fclose(fp);
    return 0;
}
