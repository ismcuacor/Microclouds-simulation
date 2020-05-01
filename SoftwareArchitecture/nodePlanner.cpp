#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/csma-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"
#include "ns3/ecofen-module.h"
#include "ns3/application.h"
#include "ns3/olsr-routing-protocol.h"

class NodePlanning
{
	public:
		void Setup (ns3::Ipv4Address address, bool )
		{
		        ns3::Ptr<ns3::Node> node = this->node;
		        if (!node)
		        {
		                printf("Error connecting to Node\n");
		                exit(-1);
		        }
		
		        m_peer = ns3::InetSocketAddress(address);
		        if (m_peer.GetLength() == 0)
		        {
		                printf("Net device does not have an address\n");
		                exit(-1);
		        }
		
		        if (node == NULL)
		        {
		                 printf("Missed node\n");
		                 exit(-1);
		        }
		
		        m_dataRate =  ns3::DataRate ("1Mbps");
		        m_socket = ns3::Socket::CreateSocket (node, ns3::UdpSocketFactory::GetTypeId ());
		        r_socket = ns3::Socket::CreateSocket (node, ns3::UdpSocketFactory::GetTypeId ());

		        ns3::InetSocketAddress r_local = ns3::InetSocketAddress (ns3::Ipv4Address::GetAny (), 4477);
		        ns3::InetSocketAddress local = ns3::InetSocketAddress (address, 4478);
		        r_socket->SetRecvCallback (ns3::MakeCallback (&ReceivePacket, this));
		
	        	if (m_socket-> Bind(local) == -1)
		        {
		                 printf("Error in Bind\n");
		                 exit(-1);
		        }
		
		        if (r_socket-> Bind(r_local) == -1)
		        {
		                 printf("Error in Bind\n");
		                 exit(-1);
		        }
		
		        if (m_socket->Connect (local) == -1)
		        {
		                printf("Error in Connect\n");
		                exit(-1);
		        }
		
		        sizes = aSizes;
		        times = aTimes;
		}

                void Send_Packet ( ns3::Ptr<ns3::Packet> pkt )
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
                        }
                }

	protected:
	        virtual void StartApplication (void) {
			
	        }
	
	        virtual void StopApplication (void)
	        {
	        }

		void ReceivePacket(ns3::Ptr<ns3::Socket> socket)
		{
        		ns3::Ptr<ns3::Packet> packet = socket->Recv ();
        		this->buffer += packet->GetSize();
		}

};
