Rough implementation of a cache covert channel for CPTS 426 - Hardware Security and Reverse Engineering.

To run, you will have to compile the program and then start sending a message, and start the receiver at roughly the same time. There is a 3 second sliding window.

The general principles of this are the receiver tracking a certain memory adress and periodically polling the cache access time. The receiver continually checks this time and calculated a rolling
average. If within the last 10 accesses, the average deviates by a certain margin (30%), then we count it as a "swing". Every second we can see if there was a high or low amount of these swings,
and thus associate them with a certain binary value. 

The receiver allocates a large block of memory, one large enough to fill the entire L1, cache, and then continually iterates over it and flushes the cache, thus increasing noise and causing swings on the receiever.

Unfortunately the cache is smarter than I am, so it becomes receive multiple zeros in a row. As a zero is sent by a high amount of noise sent by receiver, so if you try to send multiple zeros the cache starts to predict
this and starts receiving ones again. To counter this I added an encoding scheme where strings or 0's are represented by a 0, then a string of 1's equal to the amount of 0's, and then a ending 0.

This worked on my system, I was able to sucessfully send the two byte message "hi". Unfortunaley, the system is highly susceptible to noise, and very slow. We only get .5 bps, increasing it makes the noise even more of a factor.
Using other applications such as zoom or even a browser that has content playing will also interfere, causing the system to be unable to transfer data.
