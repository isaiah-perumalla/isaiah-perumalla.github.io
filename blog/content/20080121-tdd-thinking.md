
link: https://isaiahperumalla.wordpress.com/2008/01/21/tdd-thinking/
author: isaiahperumalla
description: 
post_id: 6
date: 2008/01/21 14:21:40
created_gmt: 2008/01/21 14:21:40
comment_status: open
post_name: tdd-thinking
status: private
post_type: post


Test Driven Development (TDD) is gaining more and more acceptance in mainstream development, it is easy to be misled and think about TDD in terms of only testing your code (the word "Test" in TDD adds to the misunderstanding). When thinking about TDD from a test centric view, it seem odd and feels rather unnatural to write unit tests before implementing the units. Thinking in a test centric manner what tends to happen is we ending up implementing our objects and only then we write some tests to verify the behavior of the objects we just implemented. Taking this approach we loose one of the key benefits of TDD, which is the guidance and feedback it gives on our low level design. In Test Driven Development, tests are used to communicate the intent of our design and motivate our design, to leverage this it is essential to write the tests first.