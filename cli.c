#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define TC_ALLOC(type, count) ((type *)calloc((count), sizeof(type)))
#define TC_LENOF(x) (sizeof(x) / sizeof((x)[0]))
#define TC_FAT_LENOF(x) ((x).len)

typedef struct { char * *ptr; size_t len; } tc_fat_ptr;

#include "stdlib/io.h"
int32_t main(int argc, char **argv);


int32_t main(int argc, char **argv) {
    tc_fat_ptr args = { .ptr = argv, .len = (size_t)argc };
    printi(args.len);
    print("arguments:");
    int32_t i = 0;
    while ((i < args.len)) {
        print(args.ptr[i]);
        (i += 1);
    }
    
    return 0;
}
