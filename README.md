# Microclouds
This project implements microclouds, a solution for a decentralized ICN (Information Centric Network) focused on reducing energy efficiency in the Cloud environment.

In order to reduce the Cloud’s dependence on such large datacenters, distributed solutions like edge computing are flourishing. In edge computing, virtual machines are hosted in the ISPs infrastructures, which are located near the user. These solutions ensure the ability of the Cloud to keep up with the expected explosion in demand. Moreover, from an energy consumption point of view, datacenters are large constructions which consume large amounts of energy in order to provide a centralized computational power. A reduction in size also reduces the energy they consume. On the other hand, allocating the computation close to the user reduces the physical distance that data has to travel. The smaller the distance traveled by data, the lower the latency perceived by the user. Finally some other approaches, such as P2P Clouds, contemplate drawing out part of this computation into other nodes involved in the communication process, such as private
personal devices. These approaches are able to reduce energy consumption compared to datacenters. Furthermore, they offer a lower latency than other datacenter-centric solutions.
Yet, distributed approaches described in the literature remain inefficient when dealing with latency-critical applications with concurrent users. Most distributed approaches, such as P2P Clouds, rely on having multiple active replicas of the same content. These replicas are computed in multiple nodes which increases the energy consumption, and may cause conflicts between versions (as users work on different versions of the data) and flooding of the network. On the other hand, approaches such as edge computing are still dependent
on edge datacenters, and lack the dynamism of elastic approaches such as P2P Clouds. In this thesis, we show that in these cases where all the clients are localized in a bounded geographical region, the use of a distributed Cloud approach is preferable. In this line, we designed GRaNADA, a software architecture for Cloud computing, which makes use of all available nodes involved in the communication process. In our architecture, communication devices such as ISPs’ routers and switches; and domestic routers (also known as “boxes”) can be used to host virtual environments. GRaNADA creates an overlay network across these nodes, which we call a microcloud. Every microcloud is responsible for an instance of a service or application. Inside a microcloud, management and hosting of computation are run on different devices, to reduce computational weight on single nodes. However, only one manager and one copy of data are active at the same time.

We show that GRaNADA is more energy-efficient than popular distributed solutions, where data are cached in the ISPs’ datacenters. GRaNADA is designed to benefit Internet providers by reducing the traffic along Wide Area Networks (WANs). This way, ISPs can shut down unused resources, which leads towards a reduction of energy consumption and broadband utilization. Furthermore, a reduction in the WAN traffic reduces ISPs’ transit costs.

On the other hand, GRaNADA also benefits Cloud providers. Using GRaNADA, they reduce the load in their datacenters and can also shut down unused resources to save energy or use them to accommodate more clients. Finally, by hosting the computation close to the users, latency is reduced, which improves the Quality-of-Service to the user. While the public network and the datacenter can be used by different users accessing different services, downgrading the configuration of both network and datacenters saves a significant proportion of energy. This downgrade is possible due to the reduction in traffic through specific networks, such as the Wide Area Network. Our experimental results show that through the use of GRaNADA, we save up to 75% of energy consumed by the ISPs, compared with edge computing. Furthermore, GRaNADA is able to use up to 10 times less energy than the worst case scenario of datacenter-centralized approaches. Our experiments were run on a simulation of the French research core network, RENATER. Values are explained by the reduction of the number of hops between users, which decreases the number of network nodes involved in the communication, and the reduction in the involved devices in the datacenter.

Our approach also provides a better experience for the user in terms of latency and adaptability while providing a robust Cloud platform. We evaluated the suitability of GRaNADA in the context of smart cities. We used a scenario which considered a neighborhoodservices based platform. Through the definition of this scenario we propose a solution which makes use of our architecture for mobile Cloud computing, where mobile devices offload computation in the Cloud.

Due to their network-based design, microclouds make better use of the network resources than other distributed approaches, reducing the amount of unnecessary data traveling through external networks. We evaluated the RTT, overhead time and robustness of our approach in a platform for neighborhood services. We compared our solution to a mainstream IaaS Cloud provider. We show that in this scenario the use of GRaNADA provides better performance in terms of latency and network awareness than the centralized approach. We also determined the need for an inefficiency heuristic threshold to manage the high dynamism of mobile Clouds. Thanks to this heuristic, the system experiences less migrations, which reduces the use of the computing resources.

Finally, we acknowledge that free riding in a main concern for the users. Indeed, the perception that, in the system, a significant set of users makes use of the services offered by the system without contributing its share may affect the adoption of a distributed technology. We developed a new solution to address this issue. We extended the double auction approach by involving the users as both buyers and sellers of computation. On top of this, we gave the service provider the ability of interfering with the result of the auction, by joining a group of users to ensure fairness in the system. By using a creditbased system, we enhance the perception of the Mobile Cloud as a local infrastructure. On the other hand, it allows users to go beyond the purely digital domain by exchanging this credit by real-life services. It also enhances the adaptability of services to neighborhoods, since larger amounts of credit in a neighborhood increase the number of services and, thus, the credit distribution (for example, a store may accept certain discounts on products as an exchange for credits, which can be returned into the community during the auction process). We show that, compared to other double auction solutions, our system provides a fair distribution of money while still allowing competition between services.

# Code
This project includes all the code used in the demonstration. These demonstrations where made on NS3, using the ECOFEN module for obtaining the energy consumption of the nodes. It includes the following folders:
- Architecture: Which includes all the code needed to run a decentralized ICN PaaS.
- Protocol: Which includes the code of an OSPF-based protocol for obtaining the greenest route between every client and the information.
- Simulation: which includes the support code to simulate the network (based on RENATER).

# More Information
https://tel.archives-ouvertes.fr/tel-01498190/document
