#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/csma-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"
#include "ns3/ecofen-module.h"
#include "ns3/application.h"
#include "ns3/olsr-routing-protocol.h"

#include <unistd.h>
#include <time.h>

using namespace ns3;

static NetDeviceContainer **netdevicestable;

NodeContainer nodes;
int max ;
ns3::Ipv4InterfaceContainer * interfacestable [40][40] = {};

LinearNetdeviceEnergyHelper linearNetdeviceEnergy;
int rows;
int columns;
int pathLenght;
std::set<Ptr<ns3::Node> > used_nodes2;
std::set<Ptr<ns3::Node> > active_nodes2;
OnOffNodeHelper onoff;
int port = 4477;
class PacketSimulator: public ns3::Application 
{
   public:
	int count;
	PacketSimulator()
	      : m_socket (0),
	      m_peer(), 
	      m_packetSize (0),
	      m_nPackets (0),
	      m_dataRate (0),
	      m_sendEvent (),
	      m_running (false),
	      m_packetsSent (0)
	{
		count = 0;
	}
	
	PacketSimulator( PointToPointHelper p)
	      : m_socket (0),
	      m_peer(), 
	      m_packetSize (0),
	      m_nPackets (0),
	      m_dataRate (0),
	      m_sendEvent (),
	      m_running (false),
	      m_packetsSent (0)
	{
		pointtopoint = p;
		count = 0;
	}
  
	~PacketSimulator()
	{
	    m_socket = 0;
	}

	std::set<Ptr<ns3::Node> > Network_Plan(int dest, int node, bool root, std::set<int> visited, int lenght)
	{
		std::set<Ptr<ns3::Node> > path, pathaux;
		ns3::NetDeviceContainer * ndc, *ndcaux;
		ns3::Ptr<ns3::Node> nodeObj;

		std::set<int> visited_new (visited);

		for (int j = 0; j <= columns; j++)
		{
			ndc = *(netdevicestable+(node*(columns + 1) + j));
			if (ndc != NULL && ndc -> Get(1)) {
	                        nodeObj = ndc -> Get(1) -> GetNode();
				break;
			}
		}

		for (std::set<Ptr<ns3::Node> >::iterator it = path.begin(); it != path.end(); it++)
                        if (*it == nodeObj) {
                        	nodeObj = ndc -> Get(0) -> GetNode();
                	}

		if (!nodeObj)
                {
                	printf("Error retrieving nodeObject for node %d\n", node);
                        exit(-1);
                }

		if (node == dest)
		{
			path.insert({ndc -> Get(0) -> GetNode()});
                	path.insert({nodeObj});
			pathLenght = lenght + 1;
			return path;
		}
		else
		{
			for (int j = 0; j <= columns; j++)
			{
				ndcaux = *(netdevicestable+(node*(columns+1) + j));
				if (ndcaux && (visited_new.count(j) == 0) && (pathLenght > lenght + 1))
				{
					visited_new.insert(j);
					pathaux = Network_Plan (dest, j, false, visited_new, lenght + 1);

					/*if (pathaux.empty())
					{
						std::cout << "Error in Path" << std::endl;
						exit(-1);
					}*/

					if (!pathaux.empty() && (((pathaux.size() + 1) < path.size()) || path.empty()))
					{
						path.clear();
						for (std::set< Ptr<Node> >::iterator itSet = pathaux.begin(); itSet != pathaux.end(); ++itSet)
							path.insert({*itSet});
					} 
				}
			}

			if (!path.empty())
			{
			//	for (std::set<Ptr<ns3::Node> >::iterator it = path.begin(); it != path.end(); it++)
			//		if (*it == nodeObj) {
			//			nodeObj = ndc -> Get(0) -> GetNode();
			//		}

				path.insert(nodeObj);
			}

			if (root)
			{
				if (path.empty())
				{
        	                	std::cout << "No path between node " << dest << " and node " << node << std::endl;
        	                        exit(-1);
				}

				nodeObj = ndc -> Get(0) -> GetNode();
				path.insert({nodeObj});
				
				for (std::set< Ptr<Node> >::iterator itSet = path.begin(); itSet != path.end(); ++itSet)
                                {
					used_nodes2.insert({*itSet});
				}
			}
		}

		return path;
	}

        void Setup (ns3::Ipv4Address address, Ptr<Node> node,  std::list<float> aTimes, std::list<int> aSizes, int nodeId, int nodeDestId)
	{
            /*    for (int i = 0; i <= columns; i++)
                         std::cout << i << "  ";

                std::cout << "\n\n\n";

                for (int i = 0; i <= rows; i++){
                        std::cout << i << " | ";
                        for (int j = 0; j <= 142; j++){
                                std::cout << "|" << j << " -> "<< *(netdevicestable+(i*(columns+1) + j) ) << "|";

                        }
                        std::cout << "\n\n";
                }
std::cout << nodeId << std::endl;
exit(-1);
*/
                std::set<int> visited;
		
		pathLenght = std::numeric_limits<int>::max(); 
		Network_Plan (nodeDestId, nodeId, true, visited, 0);
                Switch_On();
		Ipv4GlobalRoutingHelper::RecomputeRoutingTables ();

		if (!node)
		{
			 printf("Error connecting to Node\n");
                         exit(-1);
		}
		
		this->nodeId = nodeId;
		this->nodeDestId = nodeDestId;

		m_dataRate =  DataRate ("1Mbps");
		ns3::Ptr<ns3::Socket> recvSink = ns3::Socket::CreateSocket (nodes.Get(nodeDestId), ns3::UdpSocketFactory::GetTypeId ());
		ns3::InetSocketAddress r_local = ns3::InetSocketAddress (ns3::Ipv4Address::GetAny (), port);

		if (recvSink-> Bind(r_local) == -1)
		{
                         printf("Error in Bind\n");
                         exit(-1);
                }

        	m_socket = Socket::CreateSocket (node, UdpSocketFactory::GetTypeId ());
		m_peer = InetSocketAddress(address, port);
		if (m_peer.GetLength() == 0)
		{
			printf("Net device does not have an address\n");
			exit(-1);
		}
        	
		port++;
		if (m_socket-> Connect(m_peer) == -1)
                {
                         printf("Error in Connect\n");
                         exit(-1);
                }
		sizes = aSizes;
		times = aTimes;
             
	}

        void Send_Packet ( Ptr<Packet> pkt )
        {
		count++;
	        if(!m_socket)
                        printf("Error creating Socket \n");
                else{   
                        if (m_peer.GetLength() == 0)
                        {
                                printf("Peer missed\n");
                                exit(-1);
                        }
                        
			if ( m_socket->Connect (m_peer) == -1)
                        {
                                //std::cout << m_peer << std::endl;
                                printf("Error CONNECTING to socket %d\n",  m_socket->GetErrno());
                                exit(-1);
                        }
               }

	       if (pkt -> GetSize() <= 0)
	       {
                        printf("Error in package, wrong size\n");
                        exit(-1);
               }


               int result = m_socket->Send (pkt);

	       if (result == -1)
	       {
			std::cout << "Error in package sending: " << m_socket->GetErrno() << std::endl;
			if (m_socket -> GetErrno() == 3)
				 std::cout << "Error in size: " << pkt -> GetSize() << std::endl;
		//	exit(-1);
	       }
        }

	void Switch_On()
	{
		for (std::set<Ptr<ns3::Node> >::iterator it = used_nodes2.begin(); it != used_nodes2.end(); it++)
		{
			if (used_nodes2.count(*it) > 0 && active_nodes2.count(*it) == 0)
			{
				onoff.NodeSwitchOn (*it, Seconds (6.0));
				active_nodes2.insert(*it);
			}
		}
	}	

 	void Schedule_Send ()
	{
		std::list<float>::iterator itTime = times.begin();
		std::list<int>::iterator itSize = sizes.begin();
		//std::set<int> visited;

//		Network_Plan (nodeId, nodeDestId, true, visited);
//		Switch_On();

//`		int i = 0;		
		for (int i = 0; i < (times.size() - 1) ; i++)
		{
			itTime++;
			itSize++;
			std::set<int> visited;

        		//printf("%d %d %d\n", i, times.size() -1, times.size() -1 -i);
//			i++;

			ns3::Ptr<Packet> pkt = Create<Packet> (*itSize);
			//std::cout << *itTime << " " << *itSize << std::endl;
                        ns3::Simulator::Schedule(Seconds (*itTime), &PacketSimulator::Send_Packet, this, pkt);
		}
	}

	void reconfigure (std::pair<Ipv4InterfaceContainer*, ns3::Ptr<NetDevice> > intnetdev, NetDeviceContainer * d2, Ipv4AddressHelper * a, int nodes2[]) 
	{
		int x = nodes2[0];
		int y = nodes2[1];
		int z = nodes2[2];

		std::cout << "TOUCHES NODES " << x << " , " << y << "  AND  " << z << std::endl;

		ns3::Ptr<Node> node1 = nodes.Get(x);
		ns3::Ptr<Node> node2 = nodes.Get(y);
		ns3::Ptr<Node> node3 = nodes.Get(z);

		NetDeviceContainer d = pointtopoint.Install (node1, node2);

		*(netdevicestable+x*max + y) = &d;
		*(netdevicestable+y*max + x) = &d;
		*(netdevicestable+x*max + z) = NULL;
		*(netdevicestable+z*max + x) = NULL;

		Ipv4InterfaceContainer retval;

		Ptr<Ipv4> ipv4 = d.Get(0)->GetNode()->GetObject<Ipv4> ();
		Ptr<Ipv4> ipv4_2 = d.Get(1)->GetNode()->GetObject<Ipv4> ();

		int32_t interface = ipv4->GetInterfaceForDevice (d.Get(0));
		ipv4->GetInterfaceForDevice (d.Get(1));	
		if (interface == -1)
		{
			interface = ipv4->AddInterface (d.Get(0));
		}

		int32_t interface_2 = ipv4_2->GetInterfaceForDevice (d.Get(1));
		if (interface_2 == -1)
		{
			interface_2 = ipv4_2->AddInterface (d.Get(1));
		}

		//a->NewNetwork ();
		Ipv4Address ipAddress = interfacestable[x][z]->GetAddress(0);
		Ipv4InterfaceAddress ipv4Addr = Ipv4InterfaceAddress (ipAddress, "255.255.255.252");
		ipv4->AddAddress (interface, ipv4Addr);
		ipv4->SetMetric (interface, 1);
		
		//a->NewNetwork ();
		Ipv4Address ipAddress_2 = interfacestable[x][z]->GetAddress(1);
		Ipv4InterfaceAddress ipv4Addr_2 = Ipv4InterfaceAddress (ipAddress_2, "255.255.255.252");
		ipv4_2->AddAddress (interface_2, ipv4Addr_2);
		ipv4_2->SetMetric (interface_2, 1);

		ipv4->SetUp (interface);
		retval.Add (ipv4, interface);

		ipv4_2->SetUp (interface_2);
		retval.Add (ipv4_2, interface_2);

		intnetdev.first = &retval;

		d2 = &d;
		linearNetdeviceEnergy.Install(*d2);
		ns3::Ptr<NetDevice> nd (d.Get(0));
		intnetdev.second = nd;
		interfacestable [x] [y] =  interfacestable [x] [z]; //&retval;
		interfacestable [y] [x] = interfacestable [x] [z]; //&retval;
		interfacestable [x] [z] = NULL;
		interfacestable [z] [x] = NULL;
                ns3::Ipv4GlobalRoutingHelper::PopulateRoutingTables ();
                ns3::Ipv4GlobalRoutingHelper::RecomputeRoutingTables ();
	}

  private:

    	virtual void StartApplication (void){
	    m_running = true;
	    m_packetsSent = 0;
	    if (!m_socket)
 	    {
	            printf("Missed socket\n");
                    exit(-1);
	    }
	    Schedule_Send ();
	}
    
	virtual void StopApplication (void)
	{
	    m_running = false;
  
	    if (m_sendEvent.IsRunning ())
	      {
	        Simulator::Cancel (m_sendEvent);
	      }
  
	    if (m_socket)
	      {
	        m_socket->Close ();
	      }
	}

    Ptr<Socket>     		m_socket;
    PointToPointHelper          pointtopoint;
    Address         		m_peer;
    uint32_t        		m_packetSize;
    uint32_t        		m_nPackets;
    DataRate        		m_dataRate;
    EventId         		m_sendEvent;
    bool            		m_running;
    uint32_t        		m_packetsSent;
    std::list<float>    	times;
    std::list<int>      	sizes;
    int 			nodeId;
    int				nodeDestId;
};
