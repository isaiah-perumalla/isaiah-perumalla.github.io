## Mark and Sweep 

most fundamental gc algorithm and used a foundation for more sophisticated algorithms

tricolor marking

initially all objects are white (dead objects)

invariant of the algorithm: at en of each iteration of marking loop there are no references from black to white objects.

mark bits stored in objects header

time complexity of mark is O(L) , L = number of live objects

sweep phase O(H) , H = heap size, in practice marking dominates collection time due to cache misses , layout objects or same size in a particular area consistent memory access patterns

once an object is garbage it cannot be accessed by mutator thread, sweep can be done in parallel or by the allocator thread

### disadvantages:

fragmentation

operation is expensive need some extra head room in the heap. if allocation rate is too high, gc will be called often and will trash. generally for large heaps atleast 50% space is needed or more is needed [hertz and berger] 2005 say x5 of required heap size if

reference counting
mark and sweep
copy collectors
mark sweep compact
precise , conservative collectors
collectors which use copying need to be precise so they can fix the references of live objects once they are moved

### GC safe points
Global safe point all threads are at a safe point eg stop the world pause

## Moving collectors 
### advantages
need space-usage:2x live set, monolithic, time linear to live set
mark-compact -> incremental, need at space usage:2x live set, time: linear to live set
mark-sweep-compact -> space: linear to live, time: linear to entire heap

### disadvantages

## heap partitioning 
### generational collector
gen hypotesis assumption not often you will have pointers from old-gen to young-gen
ideally
we want young-gen not promoted too soon, but we dont want to wait too long otherwise live-set in young-gen will be ~ young-gen size

old-gen to young-gen pointers
remembered set mark old-gen objects what point to new-gen
remembered set is part of 'root' when examining young-gen live set
every store to old-gen to young-gen write barrier, need to write to rememered set (overhead here)

incremental ie (non monolithic)
serious race conditon
while marking live object, application-threads are running (mutator)
to avoid this
track reference mutations during marking (card table)
revist all mutated references and track new mutations
if the mutations is small do a stop the world.

work grows with mutation rate! may never complete and run out of memory

Azul
Azul had single pass concurrent marker, guarenteed to complete!
concurrent compaction new generation (not copy collector) and old generation
non-stop the world fall back algorithm

incremental compaction
indentify region, which be compacted within a given time
stop the the world and compact the indentified heap
time: possible quadratic to heap size
G1 collector uses this technique

CMS and GC1 have fall back mechanisms
