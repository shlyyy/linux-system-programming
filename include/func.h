//
// Created by shlyyy on 2024/4/9.
//

#ifndef GETCWD_FUNC_H
#define GETCWD_FUNC_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define ARGS_CHECK(argc, n) { \
    if (argc != n) { \
        fprintf(stderr, "ERROR: expected %d arguments.\n", n); \
        exit(1); \
    } \
}


#define ERROR_CHECK(retval, val, msg) { \
    if (retval == val) { \
        perror(msg); \
        exit(1); \
    } \
}

#define SIZE(a) (sizeof(a)/sizeof(a[0]))

#endif //GETCWD_FUNC_H
