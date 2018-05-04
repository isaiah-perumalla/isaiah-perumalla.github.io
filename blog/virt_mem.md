Title: Notes on virtual memory
Date: 2017-08-19 10:20
Category: VM

### Intro
TL;DR In this post is notes on some of the interesting things i learnt about how VM works and implemented on linux for x86_64 hardware. VM (virtual memory) subsystem has a big impact on overall performance of a running system. Having a good understanding of this VM helps system developers make informed decision when designing perf sensitive applications and also help perf engineers diagonse errors and issue related to application memory.

### What is Virtual Memory
Virtual Memory is an address mapping, it is an abstraction which gives the OS kernel and process an illusion that they each have uncontented access to their own private memory address space (which maybe much larger than the physical memory avaliable on the machine).  VM is just a way of mapping memory address from a process to actual physical addresses on machines main memory.

#### why bother with this indirection 
giving the kernel and processes it own virtual address space allows the same view of the system to each running process which allow the following, this makes it convinient and safe to run mulitple programs as the same time sharing machines resources.
* Each process can have differet memory mapping which makes it possible for multiple processes to run simultaneously without interfering with the memory contents of the others. 
* isolates memory access between processes, eg one process cannot access another process memory unless explicitly shared.
* memory can be moved or allocated only when required (lazy allocation).
* allows a program to map a portion of its address space to external device memory (eg PCI devices)
* allows memory to extend beyond main memory to disks
* allows each process to ** illusion ** it has ** uncontended ** access to all the memory on system

Most OS use virtual memory and most hardware have built in MMU (memory management unit) with build in support for virtual memory

 
### Linux on x86_64 Virtual memory support in nutshell


* Address mapping is done by hardware with support from the OS kernel

* MMU on X86 processor supports paging, this is just a set of funtionalites which allow virtual memory to be supported. (protected mode) When this is enabled in the processor every load/store instruction is translated, each time  a cpu needs to access memory it uses page tables (set up by the OS) to transtaled each address before accessing the physical memory (DRAM). 

* It is also possible to disable paging, which means there is no translation and actual address in the code is used to access physical memory, this is usually only disabled on system init and enabled by the kernel once all the kernel data structures to support virtual memory is in place.

2. The OS loads a Page table, directly into **physical memory**,  x86 has special register, CR3 which hold the physical address of page frame, which holds the page table mappings, that translate from virtual address to physical address.
3. Each translation could take 3-4 memory lookups, to avoid this overhead, x86 has a TLB (translation look aside buffer, a fast on chip cache) which is a hardware cache which holds mappings of virtual to physical addresses along with permissions
4. linux supports on demand paging which means mappings to physical memory is only created when the memory is accessed, not when the virtual memory is allocated, eg When a userland process allocates virtual memory, initially no page mapping from virtual to physical is set in the page table.
5. The structure of the table is dictated by the hardware, on x86_64 it is a radix-tree like structure with 4 levels.
6. When the CPU encouters a load/store instruction, it checks the TLB if a mapping exists for the virtual address, if it does cpu uses the physical address to load from memory.
7. If there is no mapping in the TLB, CPU ** walks ** the page table (the page table is a radix trie like structure) to find a mapping for the virtual address entry. 
8. If a page corresponding to the virtual address cannot be found in Page Table,s a CPU exception is raised, CPU switches level (only kernel can run at this level)  runs Page fault handler as defined by the kernel.  
#### Address mapping
##### Physical memory representation
* The smallest unit of memory on which the MMU and the OS kernel operates on is a ** Page ** , page size varies but is usually 4K blocks. (x86_64 support multiple page sizes). A page is an abstract concept, but A page frame refers to physical memory block, aligned at page size (commonly 4k). 
* physical memory is divided into ** page ** sized chunks, in linux every physical memory Page frame is represented by a [struct page](http://elixir.free-electrons.com/linux/v4.13.10/source/include/linux/mm_types.h#L40) the kernel uses this structure to keep track of all the pages in the system, because the kernel needs to know whether a page is free (that is, if the page is not allocated). If a page is not free, the kernel needs to know who owns the page. Possible owners include user-space processes, dynamically allocated kernel data, static kernel code, the page cache (in-memory data cache used to fulfill all disk I/O) via filesystems etc
###### Zones
* physical frames are grouped into ** zones ** by the kernel, on x86_64 there pages are partitioned into two zones, ZONE_DMA and ZONE_NORMAL, for devices that require DMA (direct memroy access) pages from ZONE_DMA are used. A zone is represented by struct zone , which holds information on how much memory is used/available in the zone
* watermarks high/low
* kmalloc analog to userspace malloc (physically contiguous memory)
##### Virtual memory representation
As mentioned earlier each process has it own virtual address space, the process has the illusion it has access to all the memory, although virtual memory is byte addressable the address space is partitioned in to intervals called VMAs ( **virtual memory areas**), each memory area has a set of permissions and operation allowed on that area. The process stack, heap and memory mapped files all have their own memory areas. for example if a process trys to write to a an address in a ready only VMA then the kernel kills the process raises a seg fault.

* struct mm_struct hold all the information related to process address space, it contains
1. pointer to page table directory (root of the page table trie)
2. list of VMA's

* struct vm_area_struct represents a continous interval in virtual memory, unique to each process (thread in a process would share this however)
1. start and end virtual address [vm_start, vm_end)
2. permissions
3. name backing file (if any ), and offset in backing file 
 
##### Process memory layout
The process address space consists of contiguous virtual memory addressable by the process,
cat /proc/pid/maps

##### File backed mmap
when mmap syscall is used by the kernel will create a new VMA (most of the time) in the process address space. note address interval is adjacent to an existing address interval, and if they share the same per-
missions, the two intervals are merged into one. If this is not possible, a new VMA is cre-
ated. In any case, do_mmap() is the function used to add an address interval to a process’s
address space—whether that means expanding an existing memory area or creating a new
one.
Java mmapped file example

#### X86, Linux observations
Linux has some great obervabilty and tracing tool that are part of the mainline kernel. 
we will use perf and bcc tool (frontend for ebpf)


##### Process Address space != Memory

* contigious virtual address does not mean contigious physical memory, the OS could map adjacent virtual pages to arbitatry physical pages, which may or may not be adjacent

* Virtual memory is an abstract resource, "out of virtual memory" error does not mean insuffcient physical memory. 

* Similarly process can subscribe to more the memory physical memory available, the kernel, can store physical pages on a swap device (if needed) and move Pages to main memory when required by the process
example allocation on 32bit machine 
use pmap to show virtual memory and rss physical memory mapped 
##### On demand paging (Lazy Allocation)
Like most modern os, linux does on demand physical page allocation. What this means is event though a process may allocate large amount of virtual memory (could be much larger than physical memory), physical pages are only allocated once the process modifies a page corresponding to a physical page. 

#### on demand paging  
pagewalk.c

#### practical steps wrap up
tools, vmstat, pidstat, funcctools perf





