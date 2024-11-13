#include "my_vm.h"
#include <math.h>
#include <string.h>

int phys_init = 0;

/*
Function responsible for allocating and setting your physical memory 
*/
int pow2(int num){
    return 1 << num;
}

void set_physical_mem() {

    //Allocate physical memory using mmap or malloc; this is the total size of
    //your memory you are simulating

    
    //HINT: Also calculate the number of physical and virtual pages and allocate
    //virtual and physical bitmaps and initialize them
    phys_init = 1;
    phys_mem = (void *)malloc(MEMSIZE);
    //*pte_mem = (pte_t *)malloc()
    offset_bits = (int) log2(PGSIZE);
    num_virt = 32 - offset_bits;
    inner_page_bits = offset_bits - log2(sizeof(pte_t));
    outer_page_bits = 32 - offset_bits - inner_page_bits;
    //double addr_size = pow(2, 32);
    //addr_size /= PGSIZE;
    //addr_size = log2(addr_size);
    //num_virt = (int) addr_size;
    if(pow2(inner_page_bits) % 8 == 0){
        phys_bitmap = (char *)malloc(((MEMSIZE / PGSIZE) / pow2(inner_page_bits)) / 8);
        memset(phys_bitmap, 0, ((MEMSIZE / PGSIZE) / pow2(inner_page_bits)) / 8);
        phys_bitmap_size = ((MEMSIZE / PGSIZE) / pow2(inner_page_bits)) / 8;
    }
    else {
        phys_bitmap = (char *)malloc(((MEMSIZE / PGSIZE) / pow2(inner_page_bits)) / 8 + 1);
        memset(phys_bitmap, 0, ((MEMSIZE / PGSIZE) / pow2(inner_page_bits)) / 8 + 1);
        phys_bitmap_size = ((MEMSIZE / PGSIZE) / pow2(inner_page_bits)) / 8 + 1;
    }
    
    if(pow2(outer_page_bits) % 8 == 0){
        virt_bitmap = (char *)malloc(pow2(outer_page_bits) / 8);
        memset(virt_bitmap, 0, pow2(outer_page_bits) / 8);
        virt_bitmap_size = pow2(outer_page_bits) / 8;
    }
    else {
        virt_bitmap = (char *)malloc(pow2(outer_page_bits) / 8 + 1);
        memset(virt_bitmap, 0, pow2(outer_page_bits) / 8 + 1);
        virt_bitmap_size = pow2(outer_page_bits) / 8 + 1;
    }
    printf("Offset: %d, Inner: %d, Outer: %d\n", offset_bits, inner_page_bits, outer_page_bits);
}


/*
 * Part 2: Add a virtual to physical page translation to the TLB.
 * Feel free to extend the function arguments or return type.
 * 
 * Note: Make sure this is thread safe by locking around critical 
 *       data structures touched when interacting with the TLB
 */
int
TLB_add(void *va, void *pa)
{

    /*Part 2 HINT: Add a virtual to physical page translation to the TLB */

    return -1;
}


/*
 * Part 2: Check TLB for a valid translation.
 * Returns the physical page address.
 * Feel free to extend this function and change the return type.
 * 
 * Note: Make sure this is thread safe by locking around critical 
 *       data structures touched when interacting with the TLB
 */
pte_t *
TLB_check(void *va) {

    /* Part 2: TLB lookup code here */



   /*This function should return a pte_t pointer*/

   return NULL;
}


/*
 * Part 2: Print TLB miss rate.
 * Feel free to extend the function arguments or return type.
 */
void
print_TLB_missrate()
{
    double miss_rate = 0;	

    /*Part 2 Code here to calculate and print the TLB miss rate*/




    fprintf(stderr, "TLB miss rate %lf \n", miss_rate);
}

static unsigned int get_top_bits(unsigned int value,  int num_bits){
	if(num_bits <= 0 || num_bits > 32){
		printf("Invalid num_bits\n");
		return 0;
	}
	return value >> (32 - num_bits);
}

static unsigned int get_bottom_bits(unsigned int value){
    unsigned int tot = (unsigned int) (1 << inner_page_bits) - 1;
    return value & tot;
}

/*
The function takes a virtual address and page directories starting address and
performs translation to return the physical address
*/
pte_t *translate(pde_t *pgdir, void *va) {
    /* Part 1 HINT: Get the Page directory index (1st level) Then get the
    * 2nd-level-page table index using the virtual address.  Using the page
    * directory index and page table index get the physical address.
    *
    * Part 2 HINT: Check the TLB before performing the translation. If
    * translation exists, then you can return physical address from the TLB.
    */
   unsigned int outer_pg_addr = get_top_bits((unsigned int) va, outer_page_bits);
   unsigned int inner_pg_addr = get_bottom_bits(get_top_bits((unsigned int) va, outer_page_bits + inner_page_bits));
   printf("Outer: %d, Inner: %d\n", outer_pg_addr, inner_pg_addr);
   pte_t *inner_pg = &pgdir[outer_pg_addr];
   return &inner_pg[inner_pg_addr];
    // As of now translate will correctly get the right values of the address.
    // Still need to test if the correct value is returned.


    //If translation not successful, then return NULL
    return NULL; 
}

static void set_bit_at_index(char *bitmap, int index, int size) {
    if(index >= size * 8 || index < 0)
    {
    	printf("Invalid set index\n");
    	return;
    }
    int i = index / 8;
    int j = (index % 8);
    bitmap[i] = bitmap[i] | 1 << j;
    return;
}

static int get_bit_at_index(char *bitmap, int index, int size) {
    if(index >= size * 8 || index < 0)
    {
    	printf("Invalid get index\n");
    	return -1;
    }
    int i = index / 8;
    int j = (index % 8);
    if((bitmap[i] & 1 << j) > 0) return 1;
    else return 0;
}

/*
The function takes a page directory address, virtual address, physical address
as an argument, and sets a page table entry. This function will walk the page
directory to see if there is an existing mapping for a virtual address. If the
virtual address is not present, then a new entry will be added
*/
int map_page(pde_t *pgdir, void *va, void *pa) {
    /*HINT: Similar to translate(), find the page directory (1st level)
    and page table (2nd-level) indices. If no mapping exists, set the
    virtual to physical mapping */
    unsigned int outer_pg_addr = get_top_bits((unsigned int) va, outer_page_bits);
    unsigned int inner_pg_addr = get_bottom_bits(get_top_bits((unsigned int) va, outer_page_bits + inner_page_bits));
    if(get_bit_at_index(virt_bitmap, outer_pg_addr, virt_bitmap_size) == 1){ // If inner page exists
        pte_t *inner_pg = &pgdir[outer_pg_addr];
        if(inner_pg[inner_pg_addr] == 0){ // Unmapped
            inner_pg[inner_pg_addr] = (long unsigned int) pa;
            printf("Mapping successful\n");
            return 1;
        }
        else{
            printf("Virtual Address already mapped\n");
            return 0;
        }
        /*
        if(get_bit_at_index(phys_bitmap, inner_pg[inner_pg_addr], phys_bitmap_size) == 0){ // If physcal space is free

        }
        */

    }
    printf("Page does not exist\n");
    return -1;
}


/*Function that gets the next available page
*/
void *get_next_avail(int num_pages) {
 
    //Use virtual address bitmap to find the next free page
    //num_pages should be == virt_bitmap_size
    //What should 1 or 0 mean in bitmap? (0 = page full/does not exist)
    for(int i = 0; i < num_pages; i++){
        if(get_bit_at_index(virt_bitmap, i, num_pages) == 1){
            void *page = (void *) directory[i];
            return page;
        }
    }
    printf("No page available\n");
    return NULL;
}

/*
struct page* create_page(pte_t *pte){ // Might need to change how bitmap works to be able to 
    struct page* new_page = (struct page*)malloc(sizeof(struct page));
	new_page->entry = pte;
    char *bmap;
    if(inner_page_bits == 8){
        bmap = (char *)malloc(inner_page_bits / 8);
        memset(bmap, 0, inner_page_bits / 8);
    }
    else {
        bmap = (char *)malloc(inner_page_bits / 8 + 1);
        memset(bmap, 0, inner_page_bits / 8 + 1);
    }
	new_page->bitmap = bmap;
	return new_page;
}
*/

/* Function responsible for allocating pages and used by the benchmark
 *
 * Note: Make sure this is thread safe by locking around critical 
 *       data structures you plan on updating. Two threads calling
 *       this function at the same time should NOT get the same
 *       or overlapping allocations
*/
void *n_malloc(unsigned int num_bytes) {

    /* 
     * HINT: If the physical memory is not yet initialized, then allocate and initialize.
     */

   /* 
    * HINT: If the page directory is not initialized, then initialize the
    * page directory. Next, using get_next_avail(), check if there are free pages. If
    * free pages are available, set the bitmaps and map a new page. Note, you will 
    * have to mark which physical pages are used. 
    */

   /*
   int num_pages = 0;
   unsigned int num_bytes_temp = num_bytes;
   while(num_bytes_temp > PGSIZE){
    num_pages++;
    num_bytes_temp -= PGSIZE;
   }
   num_pages++;
   */
   if(!phys_init) {
        set_physical_mem();
        directory = (pde_t *) malloc(pow2(outer_page_bits) * sizeof(pde_t *));
   }
   while(num_bytes > PGSIZE){ //bytes left to allocate
    num_bytes -= PGSIZE;
    
   }

    return NULL;
}

/* Responsible for releasing one or more memory pages using virtual address (va)
*/
void n_free(void *va, int size) {

    /* Part 1: Free the page table entries starting from this virtual address
     * (va). Also mark the pages free in the bitmap. Perform free only if the 
     * memory from "va" to va+size is valid.
     *
     * Part 2: Also, remove the translation from the TLB
     */
    
}


/* The function copies data pointed by "val" to physical
 * memory pages using virtual address (va)
 * The function returns 0 if the put is successfull and -1 otherwise.
*/
int put_data(void *va, void *val, int size) {

    /* HINT: Using the virtual address and translate(), find the physical page. Copy
     * the contents of "val" to a physical page. NOTE: The "size" value can be larger 
     * than one page. Therefore, you may have to find multiple pages using translate()
     * function.
     */


    /*return -1 if put_data failed and 0 if put is successfull*/

    return -1;
}


/*Given a virtual address, this function copies the contents of the page to val*/
void get_data(void *va, void *val, int size) {

    /* HINT: put the values pointed to by "va" inside the physical memory at given
    * "val" address. Assume you can access "val" directly by derefencing them.
    */

}



/*
This function receives two matrices mat1 and mat2 as an argument with size
argument representing the number of rows and columns. After performing matrix
multiplication, copy the result to answer.
*/
void mat_mult(void *mat1, void *mat2, int size, void *answer) {

    /* Hint: You will index as [i * size + j] where  "i, j" are the indices of the
     * matrix accessed. Similar to the code in test.c, you will use get_data() to
     * load each element and perform multiplication. Take a look at test.c! In addition to 
     * getting the values from two matrices, you will perform multiplication and 
     * store the result to the "answer array"
     */
    int x, y, val_size = sizeof(int);
    int i, j, k;
    for (i = 0; i < size; i++) {
        for(j = 0; j < size; j++) {
            unsigned int a, b, c = 0;
            for (k = 0; k < size; k++) {
                int address_a = (unsigned int)mat1 + ((i * size * sizeof(int))) + (k * sizeof(int));
                int address_b = (unsigned int)mat2 + ((k * size * sizeof(int))) + (j * sizeof(int));
                get_data( (void *)address_a, &a, sizeof(int));
                get_data( (void *)address_b, &b, sizeof(int));
                // printf("Values at the index: %d, %d, %d, %d, %d\n", 
                //     a, b, size, (i * size + k), (k * size + j));
                c += (a * b);
            }
            int address_c = (unsigned int)answer + ((i * size * sizeof(int))) + (j * sizeof(int));
            // printf("This is the c: %d, address: %x!\n", c, address_c);
            put_data((void *)address_c, (void *)&c, sizeof(int));
        }
    }
}



