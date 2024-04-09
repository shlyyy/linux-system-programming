#ifndef __FUNC_H__
#define __FUNC_H__
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define ARGS_CHECK(argc, n)                                                \
    {                                                                      \
        if (argc != n)                                                     \
        {                                                                  \
            fprintf(stderr, "%s line: %d ERROR: expected %d arguments.\n", \
                    __FILE__, __LINE__, n);                                \
            exit(1);                                                       \
        }                                                                  \
    }

#define ERROR_CHECK(retval, val, msg)       \
    {                                       \
        if (retval == val)                  \
        {                                   \
            printf("%s ", __FILE__);        \
            printf("line: %d\n", __LINE__); \
            perror(msg);                    \
            exit(1);                        \
        }                                   \
    }

#define SIZE(a) (sizeof(a) / sizeof(a[0]))

#endif // end __FUNC_H__