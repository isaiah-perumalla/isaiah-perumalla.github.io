Title: how Sampling profilers work
Date: 2019-02-18 10:20
Category: performance java monitoring
# Notes on JVM application Profiling 
Profilers in are a great tool in a developers toolbox, which is often underused, profiling not only helps us uncover bottlenecks in our software but also helps us gain an indepth understand the execution of application code. 
Profilers also helps uncover bugs early as it could uncover execution of code which maybe should never be executed in a certian context. 


## How execution profilers work (1000 ft view)
In this section we go through simplified view of how profiler work in general. 
### User-space profilers
A program can be setup to profile its own execution using a linux sys call [setitimer(2)](https://linux.die.net/man/2/setitimer) this facilitates a user-space program to be interrupted and notified with the *SIGPROF* posix signal, at specified interval. A signal handler is be set up to execute each time the process receives this signal from kernel. 
1) get the instruction pointer that was executing on cpu
2) make a copy of programs stack trace
3) use the instruction pointer and unwind the stack to map the address to function names using a symbol table

### Challenges of JIT compiled languages
Could we not use the same approach outlined above to profile application running on JVM ?
As we saw earlier JVM is an adaptive runtime, at any given point any when a sample is collected of the follwing 
1) the interpreter could be executing the java bytecode, a sample collected at this point the instruction pointer simply would tell us which part of interpreter code the JVM is executing, there is no way to know which **Java method** it was executing
2) JVM could be doing some house keeping either doing GC or JIT compiling some bytecode to machine code or even deoptimizing some machine code, while a sample here would give us no info on what is happening with applicaton code.
3) Suppose a we have a sample when the JIT compiled application code was running, we could have instruction pointer, but there is no way to unwind the stack and map it to java method,since the machine code is generated at runtime we dont have a symbol table, so issue here is how do **Mapping JIT Compiled methods** to java symbols [[1]](#ref-1). 
The other issue we could have two samples with same intruction pointer but they could **potentially* be part of two completely different methods, the JIT could have regenerated machine code between samples and reused the same memory address.

## JVM tooling interface 


