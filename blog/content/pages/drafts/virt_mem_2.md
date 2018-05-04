In this post is notes on some of the interesting things I learnt about how VM works and implemented on linux for x86_64 hardware. we answer the following questions
* Why memory subsystem has a big impact on overall performance of a running system.
* Overview of virtual memory on x86_64 
* How Linux kernel support virtual memory
* How to measure the effects of memory subsystem on user space application


Part-2 ?
#### Address translation

##### 64 bit addresses
At the simplest level a virtual address is split into distinct parts, one part is used to index into a page frame and the other part (lower bits) are used to address the byte within the frame.
x86_64 is has 64 bit words which can address 2^64 bytes of memory, this is enormous address space, translating every byte in this address space has large overhead, to reduce the translation cost currently x86 limits the usable bits to 48. 
the processor imposes a constraint that every address must have bits 48-63 either all 0 or all 1, this partitions the address space into two valid address ranges called the x64 canonical address range as shown below   


##### Page table structure
X86_64 is 4 level radix-tree like structurne, given address space is 48 bits wide, providing a mapping for every virtual addresses, using a linear table would  take up close to 256GB of main memory just to store the mappings.  


#### Linux implementation

##### kernel and userspace memory seperation


##### mmap shared memory

example alloc program below
##### process context switching
###### system calls
###### TLB flushes
###### ping pong process

