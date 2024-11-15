#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "../my_vm.h"
#include "../my_vm.c"

int main() {
    
    printf("Size of int: %zu bytes\n", sizeof(int));
    printf("Size of char: %zu bytes\n", sizeof(char));
    printf("Size of unsigned pte_t: %zu bytes\n", sizeof(pte_t));
    printf("%d\n", (1<<10));
    set_physical_mem();
    translate(NULL, (void *)4292169728);
    return 0;
}