#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "../my_vm.h"
#include "../my_vm.c"

int main() {
    
    //printf("Size of int: %zu bytes\n", sizeof(int));
    //printf("Size of char: %zu bytes\n", sizeof(char));
    //printf("Size of unsigned pte_t: %zu bytes\n", sizeof(pte_t));
    //printf("%d\n", (1<<10));
    //set_physical_mem();
    //translate(NULL, (void *)4292169728);
    n_malloc(1);
    void *t = n_malloc(4097);
    pte_t *test = translate(directory, t);
    printf("t: %u\n", (unsigned int)t);
    printf("test: %u\n", (unsigned int)test);
    return 0;
}