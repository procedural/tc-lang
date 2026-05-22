#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define TC_ALLOC(type, count) ((type *)calloc((count), sizeof(type)))
#define TC_LENOF(x) (sizeof(x) / sizeof((x)[0]))

void foo(void);
void main(void);


void foo(void) {
    int32_t x = 10;
    int32_t *ptr = (&x);
}

void main(void) {
    foo();
}
