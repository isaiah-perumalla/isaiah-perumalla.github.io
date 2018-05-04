Title: RAM is NOT Random Access, it a like block device
Date: 2017-08-19 10:20
Category: VM

RAM capacity on commodity machines is getting larger and larger as costs per go down, while it may be convinient for software developers to think about RAM, as a huge block of memory that returns data from arbitrary addresses with the same latency every request, this model is far from how it work in reality. In this post we cover some basics on why memory access cost vary

* Overview of DRAM, how it works
* Overview of on-chip CPU caches 

### DRAM overview
DRAM bandwith, the amount data that can flow throw the memory channel, has generally increased year after year, however the amount of time it takes for a cpu to request an memory  operation with main memory from start to finish. (latency) has largely remained the same. And the gap between access speed and CPU speeds have widened.

![cpu-vs-dram-perf]({attach}/downloads/CPU-VS-MEM.jpg)

DRAM is not a linear block of memory cell, instead it like a rectangluar grid as shown below.

1. CPU requests control of memory BUS, one the cpu is granted access to the BUS, it transmits the request for data and relinquishes control.  
2. Once the RAM received the request, the physical memory address is decoded to find the row and column for the data cell
3. The required row is determined the row, the sense amplifier must be charged for the row, then there is a period of time for the signal to settle before the column is accessed, and after a period of time known as tCAC, the correct data finally appears on the output pins of the DRAM. (all this take considerable amount of time relative to cpu cycles)
4. RAM requests control of the memory bus, once it has been granted control the data is sent back on the bus to the cpu.


Recharging cost
we noted earlier RAM is arranged as a rectangular grid, each cell has transistor and a capacitor, 
BUS contention cost on multicore  
given the above steps it means that each time the CPU transmits a request for data it has to wait 200 cycles for that data to arrive. If the CPU made 100 requests for data, then that would mean 2000 wasted cycles, and the memory bandwidth is severly under utilization. Once a row is activated, accesing adjacent cells on the row is cheap, since there is plently of bandwidth, DRAM sends blocks blocks of contigious data, (these blocks are general called cache lines) dramatically reducing latency for consecutive data reads. 



From the above we can see that accessing main memory has similar properties to accessing block devices such as disk  
* Memory transfered in contigous blocks (usually 64 bytes)

* Latency, or the time it takes to retrieve a piece of data, varies greatly–depending on whether the read is from the same row as from a different row. 

* Data movement or memory I/O needs to be minimized 

* Latency will increase once band width is utilized over 80%

### Caches 

The graph above shows now there is orders of magnitude difference between CPU speed and access to main memory (DRAM). To hide this huge gap between CPU speed and memory bus speed, modern cpu have a hierarchy of on chip cache (SRAM), these on chip caches are orders of magnitude smaller in capacity to main memory (DRAM) however the access speeds are orders of magnitude faster than DRAM access. 


| Memory Type   | Approx Latency		  | 
| ------------- |:-----------------------:| 
| Register      | 1  cycle				  | 
| L1 cache      | 3  cycles     		  |
| L2 cache 		| 12 cycles     		  | 
| L3 Cache      | 40 cycles     		  |
| Main memory   | ** 300-400 cycles **    |


#### Cache Memory primer
** "Across the industry, today’s chips are largely able to execute code faster than we can feed them with instructions and data.” - Richard Sites (1996) **

On chip SRAM caches are used alleviate the huge cost of accesing main memory, by providing the illusion of fast access to main memory, these caches are designed based on two statistical premises of spatial and temporal locaity.  

* ** Temporal Locality ** is the premise that the likelihood of a memory location that was just been accessed will be accessed again some time shortly. Temporal locality often occurs naturally in software and compiler usually can optimize code and loops to to maximize  register and cache access.

* ** Spatial Locality ** is the the premise that give a memory location has just been accessed, then there is a high likelihood that next memory access with be close the the location that was just accessed. CPU caches exploit this by fetching blocks of data, when a word in memory is accessed. These block are usually refered to as cache lines (typically 64 bytes). Spatial locality however is highly dependent of ** memory access patterns ** of software. hardware has prefetchers which can prefetch cache lines if software memory access patterns are ** predictable ** .


##### Anatomy of a modern Cache
On my current system Fedora runing on intel core i3 processor, we see the current cpu cache details

sys/devices/system/cpu/cpu0/cache/index1/coherency_line_size:64
/sys/devices/system/cpu/cpu0/cache/index1/id:0
/sys/devices/system/cpu/cpu0/cache/index1/level:1
/sys/devices/system/cpu/cpu0/cache/index1/number_of_sets:64
/sys/devices/system/cpu/cpu0/cache/index1/physical_line_partition:1
grep: /sys/devices/system/cpu/cpu0/cache/index1/power: Is a directory
/sys/devices/system/cpu/cpu0/cache/index1/shared_cpu_list:0,2
/sys/devices/system/cpu/cpu0/cache/index1/shared_cpu_map:5
/sys/devices/system/cpu/cpu0/cache/index1/size:32K
/sys/devices/system/cpu/cpu0/cache/index1/type:Instruction
/sys/devices/system/cpu/cpu0/cache/index1/ways_of_associativity:8


* The is a seperate L1 cache for instructions and data, this provides much better performance as code and data have very different access patterns in general

* ** shared_cpu_list: 0, 2 ** each core has its own L1 cache, but each hyper thread on the core share the L1 instructions and data cache.

* sets and ways of associativity , number of buckets/sets in the cache, ways of associativity is just the nubmer of cache lines in each bucket 

Hardware caches are designed based on two statistical premises of temporal and spatial locality
Spatial locality
Temporal locality 

Cache on current system
Cache as performance enhancing device is highly dependent on software (pg 35 handy)

* Cpu can excute code much faster than we can feed them with instructions and data
* 
* smaller faster on chip caches can be use to offset the huge gap in memory and cpu speeds
* Register can be controlled by software but cpu Caches are transparent to software, however the effects of cache heirachy is observable, current software under-exploits hardware caches and huge performace gain about exploiting caches
* basic diagram
* how are caches accesed, tagged- index - offset
* set associative caches
* cache hierarchies will only get deeper

### Observable effects 

** Cache as performance enhancing device is highly dependent on software ** - Jim Handy
As software developers we can memory access tend to be localized in time and space

RAM (Random Access memory) while may be convinient for software developers to think about RAM, as a huge block of memory that returns data from arbitrary addresses with the same latency every request, this is not how it work in reality. Memory access patterns have tremedous impact on performance. 

* Main memory access is roughly 300-400 times slower than accessing a cpu register
* CPU execution is limited by the rate it can get instructions and data from memory
* small SRAM Caches are used to offset the huge latency g

* Sequential/predictable access through memory is 100 times faster than arbitartily accessing memory, not know this can reduce a 3GHZ cpu to effectively a 30Mhz cpu in the worst case .	

while it is certianly possible to write software with no knowledge of the hardware memory subsystem and how the OS kernel manages physical memory,  offsets all the advances made in modern hardware, and prois efficient in either perfomance or energy consumption. The gap between main memory (DRAM) access speed and CPU execution speed is getting larger and larger, smaller heirarchical SRAM caches are used to hide this latency, software developers can no longer ignore this reality, developer model of hardware should take this into account.  



### What is Virtual Memory
Virtual Memory is an address mapping, it is an abstraction which gives the OS kernel and process an illusion that they each have uncontented access to their own private memory address space (which maybe much larger than the physical memory avaliable on the machine).  VM is just a way of mapping memory address from a process to actual physical addresses on machines main memory.

#### Why bother with this indirection 
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





