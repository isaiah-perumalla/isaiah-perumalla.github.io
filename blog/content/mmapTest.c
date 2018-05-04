#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <errno.h>

long long GB = 1024 * 1024 * 1024;

static void* try_mmap(size_t bytes) {
    void* mem = (void*)mmap(NULL,bytes, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    if(mem != MAP_FAILED ) {
        printf("allocated using mmap %zu GBs, starting at %p \n", bytes/GB, mem);
        }
    else {
        printf("failed to allocate with mmap errno = %i", errno);
    }
    return mem;
}

int main(int argc, char **argv) {
    if(argc > 1) {
        int sz;
        sz = atoi(argv[1]);
        if(try_mmap(sz*GB) > 0) {
            getchar();
        }
    }
    else {
        printf("usage allocTest <size in GB> ");
    }
    return 0;
    
}
