#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <errno.h>
#include <string.h>
#define KB 1024
#define MB (1024 * KB)
#define HUGE_PAGE_SIZE (2 * MB)
#define PAGE_SIZE 4096
#define MMAP(sz) (char*)mmap(NULL,sz, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0)
#define MMAP_HUGE(sz) (char *)mmap(NULL,sz, PROT_READ | PROT_WRITE,MAP_PRIVATE | MAP_ANONYMOUS | MAP_HUGETLB,-1, 0)

void do_walk(char* mem, int pages) {

    for (size_t p = 0; p < pages; p++ ) 
    {
            size_t byte_pos = p*PAGE_SIZE;
            *(mem + byte_pos) = 1;
    }
}



int main(int argc, char **argv) {
    if(argc < 2) 
    {
        printf("usage memaccess <size in MBs>  \n");
        return 0;
    }
    
    if(argc > 1) 
    {
    	size_t sz_mb = atoi(argv[1]);
    	size_t max_bytes = sz_mb * MB;
        size_t page_size = (argc == 3 && strcmp("--hugepages", argv[2]) == 0) ? HUGE_PAGE_SIZE : PAGE_SIZE;
        size_t pages = max_bytes/page_size;
        printf("press any key to allocate %li bytes, [%lik-Pages= %li]\n", max_bytes, page_size/KB, pages);
        getchar();
        char* mem =  page_size == 2048 ? MMAP_HUGE(max_bytes) : MMAP(max_bytes);
    	if(mem != MAP_FAILED ) {
            printf("allocated using mmap %zu MBs, %li %lik-Pages, starting at address %p \n", sz_mb, page_size/KB, pages, mem);
        }
        else {
            printf("failed to allocate with mmap errno = %i \n", errno);
            return -1;
        }
        printf("press any key to touch all %li %liK-Pages \n", pages, page_size/KB);
        getchar();
    	
    	do_walk(mem, pages);
        printf("touched all %li %liK-Pages, press any key to release pages \n", pages, page_size/KB);
        getchar();
        munmap(mem, max_bytes);
        printf("munmmap all %li %liK-Pages, press any key to exit \n", pages, page_size/KB);
        // getchar();

    }
    
    return 0;
}
