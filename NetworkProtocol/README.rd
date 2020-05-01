This folder contains the code necessary to run DEEPAC (Dynamic dEtection of Efficient Paths for Adaptive Cloud Computing), an energy-aware routing protocol based on OSPF.
The DEEPACC routing protocol is used to compute the most suitable route between every client and the service provider, by sending a gossiping message from the client to the service provider. 
Since the client does not know the address of the service provider, this message is broadcast as a discovery message to all neighbors following a flooding process. 
In every device in the path the message is captured, processed and updated with information such as the current Round-Trip Time (RTT), the current node’s ID and its energy consumption.
Then, the node forwards the message to all its neighbors. On the other hand, if a device finds itself in the message it assumes that it already sent this message and discards it (to avoid loops). 
At the end of the process, the service provider obtains updated information about all routes between the client and itself. 
This information is later sent to the service manager, to plan the overlay topology. 
