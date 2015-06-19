UDP client server application for ns-3
======================================
Client
++++++
Client is configured to genarate packets based on Poisson Distribution. 

Server
++++++
The server is programed to insert the received packet to its packet buffer. 
Then according to random distribution, server will piack a packet from the packet bufffer and serve it. 
Simply servers will genarate a response message and reply the packet.

Note that a separate header is added to measure the RTT. 
The header file is inherited from "ns-3.21/src/internet/model/"
The header name is :  comm-header.h and its associated comm-header.cc

Both those files are also added in to this repositary. 
Please make sure to move those files as appropriate directory and update the "wscript" (i.e., in application and internet derectories) files accordingly.

In addition, the server software is configured to advertise its performance statistics to the asssociated gateway router.
Therefore, the ESLR router module is added to the server. In case some one uses the server without ESLR module, 
please disable all modules related to the ESLR. Otherwise add the ESLR module to ns-3 as explained in 
https://github.com/janakawest/ESLR


