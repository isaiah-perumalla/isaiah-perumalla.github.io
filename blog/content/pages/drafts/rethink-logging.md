Title: Rethink logging
Date: 2018-04-29 10:20
Category: Logging monitoring

Logging can be an effective way of gathering data from a system and observing its runtime behaviour. In my experience most of application logging is ad-hoc text, which greatly diminishes the potential value of logging. Most systems tend to start off with very little logging or no logging, as the system grows and problems arise developers add additional logging, which leads to megabytes/gigabytes of similar looking text, what make things worse it often not clear what information should be logged, developers often log debug/trace level diagnostic information in development but turned off in production,  which makes it very difficult to extract meaningful information to troubleshoot a problem in production, as it may be missing crucial information.  
  
## Logging is a feature 
There is often emphasis on designing a system for maintanability and testability, I argue that designing a system for observability is even more important as we shall see observable systems are easier to test, debug and often can help diagonase even them most criplling problems. Observable software is software which allows you to answer question about itself by simply observing *facts and events* emitted by the software, however this only happens by *purposefully building this feature* in the software itself. 

"Engineers must be empowered to take the extra time to build for debuggability — we must be secure in the knowledge that
this pays later dividends!" 
&mdash; <cite>[Bryan Cantrill](https://files.gotocon.com/uploads/slides/conference_3/86/original/goto-ord-170502172018.pdf)</cite>

Another property of *Observable/debuggable software* is we should be able to understand the software by observing the fact and events reported by it, understanding by observation can only happend *if* the facts and events have to have semantically rich information with attached context. 
It can be used as a way to better understand system performance and behavior, even during the what can be perceived as “normal” operation of a system.
Events, tracing, exception tracking are all a derivative of logs, and if one has been using any of these tools, one already has some form of “Observability”.
Events, tracing, exception can be derived from logs, to add some form of “Observability", programmer usually do this by 

 ```
if (logger.isInfoEnabled()) {
    logger.info("received price for symbol {} for date: {} with value {}", price.symbol(), price.getDate(), price.value())
                  
   }

```


### Seperate information from representation 
[string base logging img]
 The other main disadvantage of unstructured text based logging and string format based logging API is the calling code is doing two things at once, recording the infomation but also now burdend with knowing how to represent the information. 

```
if (logger.isInfoEnabled()) {
    logger.info("received price for symbol {} for date: {} with value {}", price.symbol(), price.getDate(), price.value())
                  
   }

```

 The logging shown above is very common, but has conflated two concerns, recording information, but the calling code has to know how to representing the information, mixing these two concerns severely affects the flexibility and using string has a serious impact on performance.
 Logging usually used to record the following application/domain events, Errors and Metrics, 

 Formatted Strings are extremely inefficient to represent domain events, they should be represent as structured data with types.
  The current defacto logging API are centered around formatted strings, in doing so we are conflating the information and the representation. Text based logging using formatted strings are popular, main reason for this the assumption that humans are the first level consumers of the logs, however when analysing logs we usually interested in a subset of information, generally tools like grep are used to extract information of interested, however this approach doesnt scale for larger applications.
 logs should not necessarily have to be closely tied to an outage or a user complaint. It can be used as a way to better understand system performance and behavior. events and facts in a log can lead a developer to the answers, but it can’t make them necessarily find it, they need to be searchable
  critical components of the system proactively but the vast majority of these metrics are never looked at.


![write logs for machines to read](/imgs/LOGS-FOR-MACHINES.svg)

## Primary consumer of logs are programs
*"Logging is the process of recording application actions and state to a secondary interface."*  	
&mdash; <cite> Colin Eberhart </cite>
The consumer who sees this interface, the support guy looking at the application log, does not have access to the application's primary interface. This means that what is detailed in this log should be driven by needs of the consumer of this interface; it is often the main source of information when diagnosing a problem. It can be either to diagnose bugs or gather stats or verify the correctness of a running application. 


 This may be a more contentious point, but I belived logs should be optimized for machine readability first. What I mean by this  is what is recorded in the logs should be sematically rich messages, which is later read by simple tools to extract information which can then be consumed by humans.
 
When logs are optimized for consumption by other programs, it has the benefit of many interesting side effects.

![automatic verification](/imgs/duke-checking.gif)
### Automatic runtime and offline verification
All the data gathered via logging is useless if nobody reacts to it, one important consequence of writing logs for machines to read is it makes to possible to write programs to do *Runtime Verification* . 
Runtime verificaiton is using data from observed events from a running system to verify key invariants of a system are not violated. Having sematically rich data recorded by a running systems means we can write monitoring programs which run as a seperate process, that can verify system specifications while the system is running, this can be in production without any additional overhead on the running system.

![auto verify interaction](/imgs/runtime_verify.PNG)

* assertion of temporal logic , monitor program can also verify properties dependent on time, where time constraints can be put on states (one can only be in a state for a certain time period)
Although there is additional work for the developer to writing verification programs, the cost of writing the tool is minimal in comparision to letting bugs creep into production software, i have used this technique recently to improve the quality of a trading system which had very little automated tests. 
Machine readable logs with semantically rich domain events enable *Runtime verification (RV)* in my opinion is a very lightweight method aimed at verifying that a specific execution of a system satisfies or violates a given critical property. Dur-
ing development, runtime monitoring can be used as a debugging monitor or test oracle
for system simulation, prototyping, or tests [9]. This type of monitoring ensures testers
are notified of all specification violations, even if they are not easily noticable by testers
(such as short transient violations). In deployed systems, monitors can be used as a fault
detector to trigger recovery mechanisms such as safety shutdowns	  



verify temproal events 

### Replay debugging


### Efficient logging
"
This gives rise to a paradox: Performance problems are increasingly likely to be seen in production, but they can be understood only in development. To address a performance problem seen in production, the problem must therefore be reproduced in either a development or test environment. In a software system, as in any sufficiently complicated system, disjointed pathologies can manifest the same symptoms: Reproducing symptoms (e.g., high CPU load, heavy I/O traffic, long latency transactions, etc.) does not guarantee reproducing the same underlying problem. To phrase this phenomenon in the vernacular of IT, you might have heard or said something like this:

“Good news: We were able to reproduce it in dev, and we think that we found the problem. Dev has a fix, and we’re scheduling downtime for tonight to put it into prod. Let’s keep our fingers crossed...”

Only to have it followed the next day by something like this:

“Hey, yeah, about that fix...well, the good news is that we are faster this morning; the bad news is that we’re only about 3 percent faster. So it’s back to the war room...”

If this sounds eerily familiar, it’s because you have fallen into the trap endemic to trying to understand production problems by reproducing their symptoms elsewhere: You found a problem that was not the problem.

But Wait, It Gets Worse
The lack of observability endemic to production software is bad enough, but the layering of software abstraction makes the performance problem much more acute. Normally, software abstraction is good news, for it is abstraction that empowers one to develop an application without having to develop the Web server, application server, database server, and operating system that the application rests upon. This power has a darker side, however: When developing at higher layers of abstraction, it is easier to accidentally induce unintended or unnecessary work in the system. This is tautologically true: To be at a higher layer of abstraction, less code must induce more work, meaning it takes less of a misstep to induce more unintended consequences.

Unfortunately, this unnecessary or unintended work tends to multiply as it cascades down the stack of abstraction, so much so that performance problems are typically first understood or characterized at the very lowest layer of the stack in terms of high CPU utilization, acute memory pressure, abnormal I/O activity, excessive network traffic, etc. Despite being a consequence of higher-level software, symptoms at the lowest layer of the stack are likely to be most immediately attributable to activity in the next lowest layer—for example, the operating system or the database.

This presents another paradox: System performance problems are typically introduced at the highest layers of abstraction, but they are often first encountered and attributed at the lowest layers of abstraction. It is because of this paradox that we have adopted the myth that the path to performance lies nearly exclusively with faster hardware: faster CPUs, more networking bandwidth, etc. When this fails, we have taught ourselves to move to the next layer of the stack: to demand faster operating systems, faster databases, and better compilers. Improving these components undoubtedly improves performance, but (to use a perhaps insensitive metaphor) it amounts to hunting vermin: Depending on the relatively small iterative improvements at the lowest layers of the stack amounts to trying to feed a family on the likes of squirrel and skunk. If we can move up the stack—if we can find the underlying performance problems instead of merely addressing their symptoms—we can unlock much more substantial performance gains. This is bigger game to be sure; by focusing on performance problems higher in the stack, we can transition from hunting vermin to hunting cow—big, slow, stupid, tasty cow.

Constraints on a Solution
To hunt cow in its native habitat, the focus of observability infrastructure must make two profound shifts: from development to production, and from programs to systems. These shifts have several important implications. First, the shift from development to production implies that observability infrastructure must have zero disabled probe effect: The mere ability to observe the system must not make the delivered system any slower. This constraint allows only one real solution: Software must be optimized when it ships, and—when one wishes to observe it—the software must be dynamically modified. Further, the shift from programs to systems demands that the entire stack must be able to be dynamically instrumented in this way, from the depths of the operating system, through the system libraries, and into the vaulted heights of higher-level languages and environments. There must be no dependency on compile-time options, having source code, restarting components, etc.; it must be assumed that the first time a body of software is to be observed, that software is already running in production.
"


One important feature of a logging library should be to minimise the overhead of logging business events, in my experience working with electronic trading system, it not unusual to for system to received several 100's of market data updates event per *millisecond* traditional string based logger will cause enormous overhead. Attention needs to paid to the encoding used by the logger, a popular choice tends to be JSON as the argument is it readable by humans and machines, however JSON is a text based encoding without any type information, text based encoding like JSON can be orders of magnitude slower than binary encoding.
Having profiled a system, text based encoding used by a logging library was the biggest consumer of CPU and memory in a business application.
Binary encoding is not only more efficient to encode and decode but are also compact, less memory bandwith is used, which results in lower latency . 
Prefer binary format using share memory (memory mapped files), next post I will walk thorough a simple logging library implementation in Java, using offheap share memory buffers.  
### What should be logged
It depends on the application, one useful way to decide what to log is to think from the consumers perspective, as this dictates the events and states that need to be observed.  

#### Errors
Unexpected errors are logged using a similar API, the stack trace provided by the exception is converted to strings, and recorded, often the same exception occurs repeatedly and is logged over and over again. 
While reading through some of the Aeron source code, I noticed an interesting approach to recording expections. The approach taken there is to log distinct errors only with a count of the number of time the errors has occured, in addition the time of first and last observation is also recorded. This means that when the same error is experienced many times only the count and latest observation timestamp is updated, this approach is not only very efficient but also very much easier to read without loss of any information. I

#### Instrumetation and Metrics
 Logs are often used to gather metrics and instrument internal state of a system. for example often its useful to see 


## Conclusion

#### stats and metrics

it should be a first class citizen in your domain model, this will avoid logging unhandled expections and carrying on with execution.
It is important to think about the consumer of the log messsage. Logging usually can be split up in to the following categories
* support logging
* event logs Trace recording
* Metrics and instrumentation
## logs should be structure
logs are stream of structured data 
### Errors
### Counters
### efficient and minimal overhead
