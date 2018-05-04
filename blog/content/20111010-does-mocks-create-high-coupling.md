title: Do mocks create high coupling ?
link: https://isaiahperumalla.wordpress.com/2011/10/10/does-mocks-create-high-coupling/
author: isaiahperumalla
description: 
post_id: 71
date: 2011-10-10 12:09:53
created_gmt: 2011-10-10 12:09:53
comment_status: open
post_name: does-mocks-create-high-coupling
post_type: post


In a recent discussion with a colleague, he bought up the age old myth that mock objects are bad because they cause brittle tests and pointed me to this [post](http://www.ngauthier.com/2010/12/everything-that-is-wrong-with-mocking.html). My view is if using mock objects are causing brittle tests it is a hint that there is a issue with the design of objects collaborating, which needs to be addressed. Mock object are a great tool that aid in the design of OO systems if you subscribe the "mystical view" of object oriented programming as described by these books [RDD ](http://www.amazon.com/Object-Design-Roles-Responsibilities-Collaborations/dp/0201379430/ref=sr_1_1?ie=UTF8&qid=1318245577&sr=8-1)and [GOOS ](http://www.growing-object-oriented-software.com/). [Ralph Johnson](http://www.cincomsmalltalk.com/userblogs/ralph/blogView?showComments=true&entry=3364027251) notes "The mystical view is that an OO system is one that is built out of objects that communicate by sending messages to each other, and computation is the messages flying from object to object." [read more](https://isaiahperumalla.wordpress.com/2011/10/10/does-mocks-create-high-coupling/)

