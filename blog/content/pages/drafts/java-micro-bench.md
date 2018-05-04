Title: Things to be aware of before micro benchmarking Java
Date: 2018-03-31 10:20
Category: JAVA Performance

On my current project, there is a been a lot of emphasis on reducing the worst case latency of an eTrading application. In this business domain, few milliseconds can be difference between winning or loosing a trade. It turns out building software systems with predictable latencies is harder, as it doesnt just stop at buildig clean , well tested application code, but also involves understanding and measuring the interactions between, your application software, the OS and the hardware it is running on. One of the critical skills to is to identify and analyse critical code of your application. 
"A good programmer … will be wise to look carefully at the critical code; but only after that code has been identified."
Donald Knuth

To do this effectively it is important to build low-level custom instrumentation into your application, and also use profilers to identify key parts of your application code.

Once we have indentified the critical code path casuing the performace issue, how do we go about writing good benchmark for a small part of your application to effectively measure the performance of your the code , so we can see if any changes we make have made any improvements.

benchmarks on jvm are a black box experiment  

