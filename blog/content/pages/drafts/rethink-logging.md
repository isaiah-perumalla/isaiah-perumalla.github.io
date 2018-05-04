Title: Rethink logging
Date: 2018-04-29 10:20
Category: Logging monitoring

Logging can be an effective way of gathering data from a system and observing its runtime behaviour. In my experience most of application logging is ad-hoc text, which greatly diminishes the potential value of logging. Most systems tend to start off with very little logging or no logging, as the system grows and problems arise developers add additional logging, which leads to megabytes/gigabytes of similar looking text, what make things worse it often not clear what information should be logged, developers often log debug/trace level diagnostic information in development but turned off in production,  which makes it very difficult to extract meaningful information to troubleshoot a problem in production, as it may be missing crucial information.  
  
## Logging is a feature 
There is often emphasis on designing a system for maintanability and testability, I argue that designing a system for observability is even more important as we shall see observable systems are easier to test, debug and often can help diagonase even them most criplling problems. Observable software is software which allows you to answer question about itself by simply observing *facts and events* emitted by the software, however this only happens by *purposefully building this feature* in the software itself. 

"Engineers must be empowered to take the extra time to build for debuggability — we must be secure in the knowledge that
this pays later dividends!" 
&mdash; <cite>[Bryan Cantrill][1]</cite>
[1]:https://files.gotocon.com/uploads/slides/conference_3/86/original/goto-ord-170502172018.pdf

Another property of *Observable/debuggable software* is we should be able to understand the software by observing the fact and events reported by it, understanding by observation can only happend *if* the facts and events have to have semantically rich information with attached context. 
It can be used as a way to better understand system performance and behavior, even during the what can be perceived as “normal” operation of a system.
Events, tracing, exception tracking are all a derivative of logs, and if one has been using any of these tools, one already has some form of “Observability”
Programmer usually do this by 

 if (tracing_enabled)
  printf(“we got here!\n”); 

### what is observability ? 

*"Logging is the process of recording application actions and state to a secondary interface."*  	<cite> Colin Eberhart </cite>
The consumer who sees this interface, the support guy looking at the application log, does not have access to the application's primary interface. This means that what is detailed in this log should be driven by needs of the consumer of this interface; it is often the main source of information when diagnosing a problem. It can be either to diagnose bugs or gather stats or verify the correctness of a running application. 
![write logs for machines to read](/imgs/LOGS-FOR-MACHINES.svg)

## Primary consumer of logs are programs

 The current defacto logging API are centered around formatted strings, in doing so we are conflating the information and the representation. Text based logging using formatted strings are popular, main reason for this the assumption that humans are the first level consumers of the logs, however when analysing logs we usually interested in a subset of information, generally tools like grep are used to extract information of interested, however this approach doesnt scale for larger applications.
 logs should not necessarily have to be closely tied to an outage or a user complaint. It can be used as a way to better understand system performance and behavior. events and facts in a log can lead a developer to the answers, but it can’t make them necessarily find it, they need to be searchable
  critical components of the system proactively but the vast majority of these metrics are never looked at.
 This may be a more contentious point, but I belived logs should be optimized for machine readability first. What I mean by this  is what is recorded in the logs should be sematically rich messages, which is later read by simple tools to extract information which can then be consumed by humans.
 
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



When logs are optimized for consumption by other programs, it has the benefit of many interesting side effects.

![automatic verification](/imgs/duke-checking.gif)
### Automatic runtime and offline verification
All the data gathered via logging is useless if nobody reacts to it, one important consequence of writing logs for machines to read is it makes to possible to write programs to do *Runtime Verification* . 
Runtime verificaiton is using data from observed events from a running system to verify key invariants of a system are not violated. Having sematically rich data recorded by a running systems means we can write monitoring programs which run as a seperate process, that can verify system specifications while the system is running, this can be in production without any additional overhead on the running system.

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
