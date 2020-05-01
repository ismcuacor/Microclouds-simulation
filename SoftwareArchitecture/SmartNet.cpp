#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/applications-module.h"
#include "ns3/application.h"
#include "ns3/olsr-routing-protocol.h"

#include <string> 
#include <time.h>
#include <stdlib.h>
#include <stdio.h>

#include "SmartNet.h"

using namespace SmartNet;
SmartGraph Graph;
std::map< int, ns3::Ptr<SmartNet::SmartNode> > smartnodesReferences;

static std::map< ns3::Ptr<SmartNode>, int > smartNodesNumber;
float pathLenght;
std::map<int, ns3::Ptr<SmartGraph> > graph2;

std::map<ns3::Ptr<SmartNode>, float > explored_nodes;
std::set< ns3::Ptr<SmartNode> > visited_nodes;
ns3::OnOffNodeHelper onoff;
std::set<ns3::Ptr<SmartNode> > used_nodes;
std::set<ns3::Ptr<ns3::Node> > active_nodes;

//To be deleted
static std::map <int, ns3::Ptr<SmartNode> > destNodeGlobal;
ns3::Ptr<SmartPath> finalPath;
 
ns3::TypeId DestinationTag::GetTypeId (void)
{
  static ns3::TypeId tid = ns3::TypeId ("ns3::DestinationTag")
    .SetParent<Tag> ()
    .AddConstructor<DestinationTag> ()
    .AddAttribute ("SimpleValue",
                   "A simple value",
                   ns3::EmptyAttributeValue (),
                   ns3::MakeUintegerAccessor (&DestinationTag::GetSimpleValue),
                   ns3::MakeUintegerChecker<uint8_t> ())
  ;
  return tid;
}

ns3::TypeId DestinationTag::GetInstanceTypeId (void) const
{
  return GetTypeId ();
}

uint32_t DestinationTag::GetSerializedSize (void) const
{
  return 1;
}

void DestinationTag::Serialize (ns3::TagBuffer i) const
{
  i.WriteU8 (m_simpleValue);
}

void DestinationTag::Deserialize (ns3::TagBuffer i)
{
  m_simpleValue = i.ReadU8 ();
}

void DestinationTag::Print (std::ostream &os) const
{
  os << "v=" << (uint8_t)m_simpleValue;
}

void DestinationTag::SetSimpleValue (uint8_t value)
{
  m_simpleValue = value;
}

uint8_t DestinationTag::GetSimpleValue (void) const
{
  return m_simpleValue;
}

ns3::TypeId DistanceTag::GetTypeId (void)
{
  static ns3::TypeId tid = ns3::TypeId ("ns3::DistanceTag")
    .SetParent<Tag> ()
    .AddConstructor<DistanceTag> ()
    .AddAttribute ("SimpleValue",
                   "A simple value",
                   ns3::EmptyAttributeValue (),
                   ns3::MakeUintegerAccessor (&DistanceTag::GetSimpleValue),
                   ns3::MakeUintegerChecker<uint8_t> ())
  ;
  return tid;
}

ns3::TypeId DistanceTag::GetInstanceTypeId (void) const
{
  return GetTypeId ();
}

uint32_t DistanceTag::GetSerializedSize (void) const
{
  return 1;
}

void DistanceTag::Serialize (ns3::TagBuffer i) const
{
  i.WriteU8 (m_simpleValue);
}

void DistanceTag::Deserialize (ns3::TagBuffer i)
{
  m_simpleValue = i.ReadU8 ();
}

void DistanceTag::Print (std::ostream &os) const
{
  os << "v=" << (uint8_t)m_simpleValue;
}

void DistanceTag::SetSimpleValue (uint8_t value)
{
  m_simpleValue = value;
}

uint8_t DistanceTag::GetSimpleValue (void) const
{
  return m_simpleValue;
}

ns3::TypeId LongTag::GetTypeId (void)
{
  static ns3::TypeId tid = ns3::TypeId ("ns3::LongTag")
    .SetParent<Tag> ()
    .AddConstructor<LongTag> ()
    .AddAttribute ("SimpleValue",
                   "A simple value",
                   ns3::EmptyAttributeValue (),
                   ns3::MakeUintegerAccessor (&LongTag::GetSimpleValue),
                   ns3::MakeUintegerChecker<uint8_t> ())
  ;
  return tid;
}

ns3::TypeId LongTag::GetInstanceTypeId (void) const
{
  return GetTypeId ();
}

uint32_t LongTag::GetSerializedSize (void) const
{
  return 1;
}

void LongTag::Serialize (ns3::TagBuffer i) const
{
  i.WriteU8 (m_simpleValue);
}

void LongTag::Deserialize (ns3::TagBuffer i)
{
  m_simpleValue = i.ReadU8 ();
}

void LongTag::Print (std::ostream &os) const
{
  os << "v=" << (uint8_t)m_simpleValue;
}

void LongTag::SetSimpleValue (uint8_t value)
{
  m_simpleValue = value;
}

uint8_t LongTag::GetSimpleValue (void) const
{
  return m_simpleValue;
}

std::set<int> getNodesPathSet (char * path)
{
	std::string delimiter = "-", s (path), token;
	size_t pos = 0;
	int aux;
	std::set<int> nodesAux;
	while ((pos = s.find(delimiter)) != std::string::npos) {
	    token = s.substr(0, pos);
	    if (token.compare("ini") != 0 && token.compare("FIN") != 0)
	    {
		nodesAux.insert(atoi(token.c_str()));
	    }
	    s.erase(0, pos + delimiter.length());
	}

	return nodesAux;
}

void SmartNode::newPath (char * path)
{
	std::string delimiter = "-", s (path), token;
	std::set< ns3::Ptr<SmartNode> > nodesAux;
	std::map<ns3::Ptr<SmartNode>, std::set <ns3::Ptr<SmartNode> > >::iterator pathIteratorAux;
	std::set < ns3::Ptr<SmartNode> > pathIntAux;
	ns3::Ptr<SmartPath> pathAux;
	size_t pos = 0;
	int aux;
	ns3::Ptr<SmartNode> source;
	bool first = true;

	while ((pos = s.find(delimiter)) != std::string::npos) {
	    token = s.substr(0, pos);
	    if (token.compare("ini") != 0 && token.compare("FIN") != 0)
	    {
		aux = atoi(token.c_str());

		for (std::map< int, ns3::Ptr<SmartNode> >::iterator iteratorKey = smartnodesReferences.begin(); iteratorKey != smartnodesReferences.end(); ++iteratorKey)
		{
        		if(iteratorKey->first == aux)
			{
				if (first)
				{
					source = iteratorKey->second;
					first = false;
				}

				nodesAux.insert(iteratorKey->second);
			}
		}
	    }
	    s.erase(0, pos + delimiter.length());
	}

	std::set< ns3::Ptr<SmartEdge> > edges;
	ns3::Ptr<SmartEdge> edge;
	std::set< ns3::Ptr<SmartNode> >::iterator it = nodesAux.begin();
	ns3::Ptr<SmartNode> n1 = *it, n2;
	for (++it; it != (--nodesAux.end()); ++it)
	{
		n2 = *it;
		if (Graph.hasEdge(n1,n2))
			edges.insert(Graph.getEdge(n1,n2));
		else
		{
			edge = new SmartEdge (n1, n2);
			edges.insert (edge);
			Graph.addEdge (edge);
		}
		n1 = n2;
	}

	pathIteratorAux = pathsFinal.find(source);
	if (pathIteratorAux != pathsFinal.end())
	{
		pathIntAux = pathIteratorAux->second;

		if (pathIntAux.size() > nodesAux.size())
		{
			pathsFinal.erase(source);
			pathsFinal.insert( std::pair< ns3::Ptr<SmartNode>, std::set < ns3::Ptr<SmartNode> > > (source, nodesAux) );
		}
	}

	pathAux = new SmartPath (edges);

	if (finalPath == NULL || pathAux->getEC() < finalPath->getEC())
		finalPath = pathAux;

	time_t timer;
	time(&timer);
	std::cout << "ELAPSED TIME " << timer << std::endl;
}

void SmartNode::ReceivePacket( ns3::Ptr<ns3::Socket> socket )
{
	ns3::Ptr<ns3::Packet> packet = socket->Recv ();  
}
	
void SmartNode::ReceivePacketA( ns3::Ptr<ns3::Socket> socket )
{
	ns3::Ptr<ns3::Packet> packet = socket->Recv ();  
	if(!destNodeGlobal.empty()) 
	{
		bool visited = true;
		int i = 1;
	        LongTag tag;
	        DistanceTag * dTag;
	        DestinationTag * destTag;
		int lenght, lenghtAux, dest;	
	        tag.SetSimpleValue(0xFF);
		ns3::PacketTagIterator pIterator = packet->GetPacketTagIterator();

		ns3::Callback<ns3::ObjectBase *> constructor;
		ns3::ObjectBase *instance;
		while (pIterator.HasNext ())
		{
			ns3::PacketTagIterator::Item item = pIterator.Next ();
			constructor = item.GetTypeId ().GetConstructor ();
  			NS_ASSERT (!constructor.IsNull ());
  			instance = constructor ();
  			dTag = dynamic_cast<DistanceTag *> (instance);
			destTag = dynamic_cast<DestinationTag *> (instance);
  			if( destTag != 0 )
			{
  				item.GetTag (*destTag);
				dest = (int)(destTag->GetSimpleValue()) - (int)48;
				delete destTag;
			}
  			else
			{
  				if( dTag != 0 )
				{
  					item.GetTag (*dTag);
					lenght = (int) (dTag->GetSimpleValue()); //- (int)48;
					delete dTag;
				}
			}
		}

		lenghtAux = lenght + 1 - 48;

		if (dest != NULL && lenghtAux != NULL)
		{
			if(packet->PeekPacketTag(tag))
			{
				uint8_t buffer[200];
				char buffer2[200];
				uint32_t size = sizeof(buffer2);
	
				int key = -1;
				for (std::map< int, ns3::Ptr<SmartNode> >::iterator itKey = smartnodesReferences.begin(); itKey != smartnodesReferences.end(); ++itKey)
					if(itKey->second == this)
						key = itKey->first;
	
				packet->CopyData(buffer, size);
				snprintf(buffer2, sizeof(buffer2), "%s", buffer);
				std::map<int, std::set <int> >::iterator setIt = paths.find(dest);
				std::set <int> nodesPathSet = getNodesPathSet (buffer2), setAux = setIt->second;
				if (setIt == paths.end())
				{
					visited = false;
					paths.insert(std::pair<int,std::set<int> > (dest, nodesPathSet));
				} else {
					for (std::set <int>::iterator setIterator = nodesPathSet.begin(); setIterator != nodesPathSet.end(); ++setIterator)
						if (setAux.find(*setIterator) == setAux.end())
						{
							visited = false;
							setIt->second.insert(*setIterator);
						}
				}
				if(!visited)
				{
					if (destNodeGlobal.find(getMCloud())->second == this ){
						ns3::Ptr<SmartNode> destNode = smartnodesReferences.find(dest)->second;
						std::set< ns3::Ptr<SmartNode> > neigh = this->getNeighbors();
						for(std::set< ns3::Ptr<SmartNode> >::iterator it = neigh.begin(); it!= neigh.end(); ++it)
						{
							if(destNodeGlobal.find(getMCloud())->second == *it)
							{

								for (std::map< int, ns3::Ptr<SmartNode> >::iterator itKey = smartnodesReferences.begin(); itKey != smartnodesReferences.end(); ++itKey)
                                                                        if(itKey->second == *it)
                                                                        {
                                                                                key = itKey->first;
                                                                        }
                                                                char buffer3[200];
                                                                snprintf(buffer3, sizeof(buffer3), "%s-%d-FIN", buffer2, key);
                                                                //std::cout << "END " << buffer3 << std::endl;
                                                                destNode->newPath(buffer3);
							} else {
								if ( (*it)->isP2P() )
								{
                                                                	char buffer3[200];
                                                                	snprintf(buffer3, sizeof(buffer3), "%s-%d-FIN", buffer2, key);
									(*it)->newPath(buffer3);
								} else {
									this->Check_Neighbors(*it, destNode, lenght, buffer2);
								}
							}
						}
					} else {
						char buffer3[200];
						snprintf(buffer3, sizeof(buffer3), "%s-FIN", buffer2);
						//std::cout << "END " << buffer3 << std::endl;
						this->newPath(buffer3);
					}
				}
			}
		}
	} 
}

int SmartNode::findSmartNodeRef ()
{
	int result = -1;
	for (std::map< int, ns3::Ptr<SmartNet::SmartNode> >::iterator it = smartnodesReferences.begin(); it!= smartnodesReferences.end(); ++it)
		if (it->second == this)
			return it->first;
	return result;
}

void SmartNode::Schedule_Send (std::list<float> times, std::list<int> sizes)//ns3::Ptr<SmartNode> destNode)
{
	if(this->getMCloud() != -1 && graph2.find(this->getMCloud())->second->getNodes().size() > 0)
	{
		this->sizes = sizes;
		this->times = times;
		ns3::Ptr<SmartNode> destNode = prov2.find(this->getMCloud())->second;
		std::set<ns3::Ptr<SmartNode> > nodes = (graph2.find(this->getMCloud())->second)->getNodes(), Neighbors = destNode->getNeighbors();	
		ns3::Ipv4Address address;
		std::list<float>::iterator itTime;
		std::list<int>::iterator itSize;
	
		int source = this->findSmartNodeRef (), dest = destNode->findSmartNodeRef (), aux = -1;
		
		itTime = times.begin();
		itSize = sizes.begin();
		ns3::Ptr<ns3::Packet> pkt;
	
		if (source == -1)
		{
			printf("Wrong Source\n");
			exit(-1);
		}
	
		if (dest == -1)
		{
			std::cout << "Wrong dest " << destNode << std::endl;
			exit(-1);
		}
	
		for (std::set<ns3::Ptr <SmartNode> >::iterator it2 = Neighbors.begin(); it2 != Neighbors.end(); ++it2)
		{
			if (nodes.find(*it2) != nodes.end())
			{
				aux = (*it2)->findSmartNodeRef ();
				break;
			}
		}

		address = interfacestable[dest][aux]->GetAddress(0);
		SmartNode::SetupNow2 (address);

			while (itTime != times.end() && itSize != sizes.end() )
			{
				pkt = ns3::Create<ns3::Packet> (*itSize); 
				ns3::Simulator::Schedule(ns3::Seconds (*itTime+1.0), &SmartNode::Send_Packet_Now, this, destNode, pkt, address);
	
				itTime++;
			 	itSize++;
				//this->Send_Packet_Now (destNode, pkt, address);
			}
	}
}

void SmartNode::addRepeat (ns3::Ptr<SmartNode> node)
{
	if(!repeat.insert(node).second)
		throw std::invalid_argument( " Cannot add repeat " );
}

void SmartNode::SetupNow (ns3::Ptr<SmartNode> smNode, ns3::Ipv4Address address)
{
        ns3::Ptr<ns3::Socket> m_socket;
        uint32_t ipTos = 0;
        bool ipRecvTos = true;
        uint32_t ipTtl = 0;
        bool ipRecvTtl = true;
        int port = std::max(this->getPort(), smNode->getPort());
        this->setPort(port);
        smNode->setPort(port);

        ns3::Ptr<ns3::Node> node = this->node;
        if (!node)
        {
                printf("Error connecting to Node\n");
                exit(-1);
        }

        ns3::Ptr<ns3::Node> node2 = smNode->getNode();
        if (!node2)
        {
                printf("Error connecting to Node2\n");
                exit(-1);
        }

        if (!smNode)
        {
                printf("Error with Recepient Smartnode\n");
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

        //Setup Receiver
        ns3::Ptr<ns3::Socket> recvSink = ns3::Socket::CreateSocket (node2, ns3::UdpSocketFactory::GetTypeId ());
        ns3::InetSocketAddress r_local = ns3::InetSocketAddress (ns3::Ipv4Address::GetAny (), port);
 
        ns3::Callback<void, ns3::Ptr<ns3::Socket> > socketCb;

        socketCb = ns3::MakeCallback (&SmartNode::ReceivePacketA, smNode);
 
        if( recvSink->Bind ( r_local ) == -1)
        {
                printf("Error in Bind\n");
                exit(-1);
        }

        std::pair < ns3::Ptr<ns3::Socket>, ns3::InetSocketAddress * > r_socketConf (recvSink, &r_local);
        smNode->setSocket(this, r_socketConf);

        //Setup sender
        m_socket = ns3::Socket::CreateSocket (node, ns3::UdpSocketFactory::GetTypeId ());

        ns3::InetSocketAddress local = ns3::InetSocketAddress (address, port);
        if (m_socket->Connect (local) == -1)
        {
                printf("Error in Connect\n");
                exit(-1);
        }

        this->setSocket(smNode, std::pair <ns3::Ptr<ns3::Socket>,  ns3::InetSocketAddress * > (m_socket, &local));
}

void SmartNode::SetupNow2 (ns3::Ipv4Address address)
{
	uint32_t ipTos = 0;
	bool ipRecvTos = true;
	uint32_t ipTtl = 0;
	bool ipRecvTtl = true;
	ns3::Ptr<SmartNode> smNode = prov2.find(this->getMCloud())->second;
	int port = std::max(this->getPort(), smNode->getPort());
	this->setPort(port);
	smNode->setPort(port);

	ns3::Ptr<ns3::Node> node = this->node;
	if (!node)
	{
		printf("Error connecting to Node\n");
		exit(-1);
	}
	
	ns3::Ptr<ns3::Node> node2 = smNode->getNode();
	if (!node2)
        {
                printf("Error connecting to Node2\n");
                exit(-1);
        }

        if (!smNode)
        {
                printf("Error with Recepient Smartnode\n");
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

	//Setup Receiver
	ns3::Ptr<ns3::Socket> recvSink = ns3::Socket::CreateSocket (node2, ns3::UdpSocketFactory::GetTypeId ());
	ns3::InetSocketAddress r_local = ns3::InetSocketAddress (ns3::Ipv4Address::GetAny (), port);
	
	ns3::Callback<void, ns3::Ptr<ns3::Socket> > socketCb;

	socketCb = ns3::MakeCallback (&SmartNode::ReceivePacket, smNode);

	if( recvSink->Bind ( r_local ) == -1)
        {
                printf("Error in Bind\n");
                exit(-1);
        }

	std::pair < ns3::Ptr<ns3::Socket>, ns3::InetSocketAddress * > r_socketConf (recvSink, &r_local);
	smNode->setSocket(this, r_socketConf);

	//Setup sender	
	m_socket = ns3::Socket::CreateSocket (node, ns3::UdpSocketFactory::GetTypeId ());

	ns3::InetSocketAddress local = ns3::InetSocketAddress (address, port);
	if (m_socket->Connect (local) == -1)
	{
		printf("Error in Connect\n");
		exit(-1);
	}
	

	this->setSocket(smNode, std::pair <ns3::Ptr<ns3::Socket>,  ns3::InetSocketAddress * > (m_socket, &local));
}

void SmartProvider::Setup (ns3::Ipv4Address address, std::list<float> aTimes, std::list<int> aSizes)
{
	ns3::Ptr<ns3::Socket> m_socket, r_socket;
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
	r_socket->SetRecvCallback (ns3::MakeCallback (&SmartNode::ReceivePacket, this));	

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

	if (m_socket-> Connect(local) == -1)
        {
                 printf("Error in Connect\n");
                 exit(-1);
        }

	sizes = aSizes;
	times = aTimes;
}
                        
void SmartManager::Switch_On (float seconds)
{
        for (std::set<ns3::Ptr<SmartNode> >::iterator it = used_nodes.begin(); it != used_nodes.end(); it++)
        {
                if ( active_nodes.count((*it)->getNode()) == 0)
                {
                        active_nodes.insert((*it)->getNode());
                        onoff.NodeSwitchOn ((*it)->getNode(), ns3::Seconds (seconds));
                }
        }
}

void SmartManager::RecomputeTables ()
{

		ns3::Ipv4GlobalRoutingHelper::PopulateRoutingTables ();
		ns3::Ipv4GlobalRoutingHelper::RecomputeRoutingTables ();	
}

void SmartManager::Switch_On ()
{
        for (std::set<ns3::Ptr<SmartNode> >::iterator it = used_nodes.begin(); it != used_nodes.end(); it++)
        {
                if ( active_nodes.count((*it)->getNode()) == 0)
                {
                        active_nodes.insert((*it)->getNode());
			onoff.NodeSwitchOn ((*it)->getNode(), ns3::Seconds(ns3::Simulator::Now().GetSeconds()));
                }
        }
}

SmartManager::SmartManager(ns3::Ptr<SmartGraph> graphIn, ns3::Ptr<SmartProtocol> prot)
{
        protocol = prot;
        Graph = *graphIn;
	graph = prot->Run(graphIn);

	provider = graph->getProvider();
}

/*
SmartManager::SmartManager(ns3::Ptr<SmartGraph> graphIn, ns3::Ptr<SmartProtocol> prot, std::set< ns3::Ptr<SmartNode> > talking_nodes)
{
        protocol = prot;
	Graph = *graphIn;
        graph = prot->RunNow(graphIn, talking_nodes);

	provider = Graph.getProvider();
}
*/

SmartManager::SmartManager(ns3::Ptr<SmartGraph> graphIn, ns3::Ptr<SmartProtocol> prot, std::set< ns3::Ptr<SmartNode> > talking_nodes)
{
        protocol = prot;
	Graph = *graphIn;

	graph = prot->Run(graphIn, talking_nodes);
	graph2.insert(std::pair<int, ns3::Ptr<SmartGraph> > ((*(talking_nodes.begin()))->getMCloud(), graph));
 	
 	std::set<ns3::Ptr<SmartNode> > setNodes = graph->getNodes();
 	std::set<ns3::Ptr<SmartNode> >::iterator it = setNodes.begin();
 	int randomN = rand() % setNodes.size();
 	std::advance(it, randomN);
 	prov2.insert(std::pair<int, ns3::Ptr<SmartNode> > (this->getMCloud(), *it));
  
 	provider = new SmartProvider((prov2.find(this->getMCloud())->second)->getNode());
}

SmartManager::SmartManager(ns3::Ptr<SmartGraph> graphIn, ns3::Ptr<SmartProtocol> prot, std::set< ns3::Ptr<SmartNode> > talking_nodes, int mcloud)
{
        protocol = prot;
        Graph = *graphIn;
 
        graph = prot->Run(graphIn, talking_nodes, mcloud);

	if (graph->getNodes().size() > 0)
	{
		graph2.insert(std::pair<int, ns3::Ptr<SmartGraph> > (mcloud, graph));
	        std::set<ns3::Ptr<SmartNode> > setNodes = graph->getNodes();
		std::set<ns3::Ptr<SmartNode> >::iterator it = setNodes.begin();
		for (std::set<ns3::Ptr<SmartNode> >::iterator it2 = setNodes.begin(); it2 != setNodes.end(); ++it2)
			std::cout << (*it2)->getNode()->GetId() << "_";
		std::cout <<std::endl;
	        int randomN = rand() % setNodes.size();
	        std::advance(it, randomN);
		prov2.erase(mcloud);
	        prov2.insert(std::pair<int, ns3::Ptr<SmartNode> > (mcloud, *it));
	        provider = new SmartProvider((*it)->getNode());
	} 
}

SmartManager::SmartManager(ns3::Ptr<SmartGraph> graphIn, ns3::Ptr<SmartProtocol> prot, std::set< ns3::Ptr<SmartNode> > talking_nodes, std::set<int> mclouds)
{
	for (std::set<int>::iterator it = mclouds.begin(); it != mclouds.end(); ++it)
	{
		managers.insert (std::pair <int, ns3::Ptr<SmartManager> > (*it, new SmartManager (graphIn, prot, talking_nodes, *it)));
	}
}

std::set<ns3::Ptr<SmartNode> >  SmartManager::GetActiveNodes()
{
	std::set<ns3::Ptr<SmartNode> > nodesAux = Graph.getNodes();
	return nodesAux;
}

void SmartManager::RunAdditionProtocol (ns3::Ptr<SmartNode> node)
{
	clock_t timer, timer2;

	timer = clock();
	protocol->onNewSmartNode(graph, node);
	used_nodes.insert(node);
	timer2 = clock();
	printf("Elapsed time %f\n", timer2 - timer);
}

SmartPath::SmartPath (ns3::Ptr<SmartNode> n1, ns3::Ptr<SmartNode> n2, ns3::Ptr<SmartPath> pathIn) 
{
	ns3::Ptr<SmartEdge> edge = Graph.getEdge(n1, n2);

	if (edge == NULL)
		throw std::invalid_argument( "Error connecting node 1 to first node in the path. Are they neighbors?" );

        path.insert(edge);
	std::set< ns3::Ptr<SmartEdge> > edges = pathIn->getPath();

	 if (edges.empty())
                throw std::invalid_argument( "Empty path?" );
	
	for (std::set< ns3::Ptr<SmartEdge> >::iterator it = edges.begin(); it != edges.end(); ++it)
		path.insert(*it);
}

void SmartGraph::Migrate( ns3::Ptr<SmartMigration> migration ) 
{

	std::map<  ns3::Ptr<SmartEdge>, EnumMigration > mig = migration->getMigrations();

	mig.erase(NULL);
	for (std::map<  ns3::Ptr<SmartEdge>, EnumMigration >::iterator it = mig.begin(); it != mig.end(); ++it)
	{
		if (it->second == ADD) {
			this->addEdge(it->first);
		}
		if (it->second == REMOVE) {
			this->removeEdge(it->first);
		} 
	}
}

ns3::Ptr<SmartPath> SmartNode::Find_Path (ns3::Ptr<SmartNode> destNode, bool root, std::set< ns3::Ptr<SmartNode> > visited, float lenght)
{
	ns3::Ptr<SmartPath>  pathaux, path;
	int auxLenght = lenght + 1;

	if (!destNode)
        {
        	printf("No destination node\n");
                exit(-1);
        }

	visited.insert(this);

	std::set< ns3::Ptr<SmartNode> > neig = this->getNeighbors();

	for (std::set< ns3::Ptr<SmartNode> >::iterator nodesNumberIt = neig.begin(); nodesNumberIt != neig.end(); ++nodesNumberIt)
	{
		if (visited.find(*nodesNumberIt) == visited.end() && (auxLenght < pathLenght || pathLenght == NULL) )
		{
			visited.insert(*nodesNumberIt);

			if (*nodesNumberIt == destNode) {
				if (path == NULL)
	                	{
	                	        path = new SmartPath();
	                	}
	
				pathLenght = auxLenght;
	                	path->addEdge(Graph.getEdge(this, *nodesNumberIt));
				break;
			} else {
				// To be extended using lenght + edgeLenght
				if (pathLenght == NULL || auxLenght < pathLenght)
				{
					//Recursive call 
					pathaux = (*nodesNumberIt)->Find_Path ( destNode, false, visited, auxLenght );
					if (pathaux != NULL && !pathaux->getPath().empty())
					{
						if (path == NULL || (pathaux->getPath().size() + 1) < path->getPath().size())
						{
							path = pathaux;
							path->addEdge(Graph.getEdge(this, *nodesNumberIt));
						}

						pathaux->getPath().clear();
					}
				}
			}
		}
	}

	if (root){
		if (path != NULL && !path->getPath().empty() )
	        {
			std::set< ns3::Ptr<SmartNode> > pathNodes = path->getNodes();
			visited_nodes.insert(pathNodes.begin(), pathNodes.end());
		}
	}

	return path;
}

void SmartNode::Check_Neighbors(ns3::Ptr<SmartNode> nextNode, ns3::Ptr<SmartNode> destNode, int lenght, char * previousNodes)
{
	uint32_t id = nextNode->getNode()->GetId();
	ns3::Ptr<ns3::Node> node = this->node;
	ns3::Ptr<ns3::Node> node2 = nextNode->getNode();
	if (!node)
	{
		printf("Error connecting to Node\n");
		exit(-1);
	}
	
	if (!node2)
        {
                printf("Error connecting to Node2\n");
                exit(-1);
        }
	
	ns3::Ipv4Address address = Graph.getIpv4_1(this, nextNode);
	LongTag tag;
	DistanceTag dTag;
	DestinationTag destTag;
	tag.SetSimpleValue(0xFF);

	unsigned char buffer2[2], buffer2Aux[2];
	char bufferAux[2], buffer[2], nodeString[200];
	uint32_t size = sizeof(nodeString);
	int key, key2;

	snprintf(buffer, sizeof(buffer), "%d", lenght);
	buffer2[0] = buffer[0];
	dTag.SetSimpleValue(buffer2[0]);

	for (std::map< int, ns3::Ptr<SmartNode> >::iterator it = smartnodesReferences.begin(); it != smartnodesReferences.end(); ++it)
	{
		if(it->second == destNode)
			key = it->first;
		else if(it->second == this)
			key2 = it->first;
	}

	int cx = snprintf(nodeString, sizeof(nodeString), "%s-%d", previousNodes, key2);

	if (cx < 0 || cx > 200)
		throw std::invalid_argument( "Error while copying path information to be sent." );

	snprintf(bufferAux, sizeof(bufferAux), "%d", key2);
	buffer2Aux[0] = bufferAux[0];
	destTag.SetSimpleValue(buffer2Aux[0]);

	ns3::Ptr<ns3::Packet> pck = ns3::Create<ns3::Packet> (reinterpret_cast<uint8_t*>(nodeString), size); 
	pck->AddPacketTag(tag);
	pck->AddPacketTag(destTag);
	pck->AddPacketTag(dTag);

	Send_Packet_Now2 (nextNode, pck, address);
}

ns3::Ptr<SmartPath> Network_Plan(ns3::Ptr<SmartNode> sourceNode, ns3::Ptr<SmartNode> destNode, bool root, std::set< ns3::Ptr<SmartNode> > visited, float lenght)
{
	ns3::Ptr<SmartPath>  pathaux, path; // = new SmartPath();

	if (!destNode)
        {
        	printf("No destination node\n");
                exit(-1);
        }

        if (!sourceNode)
        {       
                printf("No source node\n");
                exit(-1);
        }

	if (sourceNode == destNode)
        {
		 path = new SmartPath();
                 path->addEdge(new SmartEdge(sourceNode));
                 pathLenght = lenght + 1.0;
                 return path;
        } else {
		std::set< ns3::Ptr<SmartNode> > visited_new (visited);
		std::set< ns3::Ptr<SmartNode> > neig = sourceNode->getNeighbors();
		std::set< ns3::Ptr<SmartEdge> > edgesAux;
	
		for (std::set< ns3::Ptr<SmartNode> >::iterator nodesNumberIt = neig.begin(); nodesNumberIt != neig.end(); ++nodesNumberIt)
		{
			if (visited_new.count(*nodesNumberIt) == 0)
			{
				if (*nodesNumberIt == destNode) {
					path = new SmartPath();
					path->addEdge(Graph.getEdge(sourceNode, *nodesNumberIt));
					pathLenght = lenght + 1.0;
					break;
				} else {
					if (lenght + 1.0 < pathLenght)
					{
						visited_new.insert(*nodesNumberIt);
						pathaux = Network_Plan (*nodesNumberIt, destNode, false, visited_new, lenght + 1.0);

						if (pathaux != NULL && !pathaux->getPath().empty())
						{
 							edgesAux = pathaux->getEdges();
 							if (edgesAux.size() == 1 && (*edgesAux.begin())->getNode2() == NULL)
 								pathaux = new SmartPath(Graph.getEdge(sourceNode, (*edgesAux.begin())->getNode1()));

 							if (!path || path->getPath().empty() || (pathaux->getPath().size() + 1) < path->getPath().size())
							{
								path = pathaux;
								pathaux->getPath().clear();
							}

	        	                        	path->addEdge(Graph.getEdge(sourceNode, *nodesNumberIt));
						}
					}
				}
			}
		}
	}

	if (root){
		if(path != NULL)
		{
			if (!path->getPath().empty())
                	{
				std::set< ns3::Ptr<SmartNode> > pathNodes = path->getNodes();
				used_nodes.insert(pathNodes.begin(), pathNodes.end());
			} else
				std::cout << "EMPTY PATH"<< std::endl;
		} else {
			path = new SmartPath();
			std::cout << "NO PATH" << std::endl;
		}
	}

	return path;
}

void SmartGraph::findPath (ns3::Ptr<SmartNode> n1, ns3::Ptr<SmartNode> n2)
{
	std::set< ns3::Ptr<SmartNode> > nodes (Graph.getNodes());
	std::set< ns3::Ptr<SmartNode> > neig;
	nodes.erase(n1);
	ns3::Ptr<SmartNode> c_node = n1, aux_node, old_node;
	ns3::Ptr<SmartEdge> edge, edge_aux, edge_sol;

	float EC, c_EC;

	while (nodes.size() > 0)
	{
		edge = Graph.getEdge(c_node,n2);
                
		if (edge != NULL)
		{
                        pathSolution.insert(edge);
                	break;
                }

		EC = std::numeric_limits<float>::max();
		edge_sol = NULL;

		if (c_node == NULL)
			break;	

		neig = c_node->getNeighbors();
		for (std::set< ns3::Ptr<SmartNode> >::iterator it = neig.begin(); it != neig.end(); ++it)
		{
			if (nodes.find(*it) != nodes.end())
			{
				edge_aux = Graph.getEdge(*it,n2);
				if (edge != NULL)
                		{ 
					edge_sol=edge;
                        		nodes.clear();
					break;
                		}
			
				edge_aux =  Graph.getEdge(c_node, *it);
				if (c_EC + edge_aux->getConsumption() < EC)
				{
					aux_node = *it;
					edge_sol= edge_aux;
					EC = c_EC + edge_aux->getConsumption();
				}
			}
		}

		if (edge_sol != NULL) {
			nodes.erase(aux_node);
			visited_nodes.insert(aux_node);
			old_node = c_node;
			c_node = aux_node;
			pathSolution.insert(edge_sol);
			c_EC = EC;
		} else {
			visited_nodes.insert(c_node);
			nodes.erase(c_node);
			c_node = old_node;
			nodes.insert(c_node);
		}
	}
}

void SmartGraph::findPath (ns3::Ptr<SmartNode> n1, ns3::Ptr<SmartNode> n2, int edges, int nodes)
{
	ns3::Ptr<SmartEdge> pathAux;
	std::set< ns3::Ptr<SmartPath> > result, pathsList, pathsSolution;
	ns3::Ptr<SmartNode> toVisit;
	float EC = std::numeric_limits<float>::max();
	
	ns3::Ptr<SmartEdge> edge;

	if (n1 == NULL)
		throw std::invalid_argument( "NULL Node" );
        
	edge = this->getEdge(n1, n2);
	if (edge != NULL)
        {
        	pathSolution.insert(edge);
        } else {
		toVisit = n1;
		
		if (n1->getNeighbors().empty())
		{
			throw std::invalid_argument( "No neighbors" );
		}

	        for (std::set< ns3::Ptr<SmartNode> >::iterator it = n1->getNeighbors().begin(); it != n1->getNeighbors().end(); ++it)
	        {
			if (visited_nodes.find(*it) == visited_nodes.end())
			{
				std::map<ns3::Ptr< SmartNode >, float >::iterator exploredIt = explored_nodes.find(*it);
				if (exploredIt == explored_nodes.end())
				{
					exploredIt = explored_nodes.insert(std::pair<ns3::Ptr< SmartNode >, float> (*it, (float) 1)).first;
				}
				else {
					if (explored_nodes.find(n1)->second + 1 < exploredIt->second) {
						exploredIt->second = explored_nodes.find(n1)->second + 1;
					}
				}
		
				if (EC > exploredIt->second)
				{
					toVisit = exploredIt->first;
				}
				visited_nodes.insert(n1);	
			}
		}
		
		explored_nodes.erase(n1);
		pathSolution.insert(new SmartEdge (n1, toVisit));

		findPath (toVisit, n2, edges + 1, nodes + 2);
	}
}

void SmartGraph::Delete (ns3::Ptr<SmartNode> node)
{
	std::set<  ns3::Ptr<SmartEdge> > edges = this->getEdges();
	std::set< ns3::Ptr<SmartNode> > neigh = node->getNeighbors();
	
	for (std::set<  ns3::Ptr<SmartEdge> >::iterator it = edges.begin(); it != edges.end(); ++it) {
		if (((*it)->getStructure()).node1 == node || ((*it)->getStructure()).node2 == node){
			this->removeEdge(*it);
		}
	}

	for (std::set< ns3::Ptr<SmartNode> >::iterator it2 = neigh.begin(); it2 != neigh.end(); ++it2)
	{
		(*it2)->delNeighbor(node);
	}
}

ns3::Ptr<SmartMigration> SmartGraph::merge (ns3::Ptr<SmartNode> node)
{
	ns3::Ptr<SmartEdge> edgeAux, edgeSol;
	ns3::Ptr<SmartPath> pathSol = new SmartPath(), result = new SmartPath();
	std::set< ns3::Ptr<SmartNode> > visited;
	
	std::set< ns3::Ptr<SmartPath> > pathsList, pathSetSol;
	std::set<ns3::Ptr<SmartNode> >::iterator it2;
	
	float EC = std::numeric_limits<float>::max();

	ns3::Ptr<SmartMigration> migration = new SmartMigration();

        std::set< ns3::Ptr<SmartNode> >::iterator it;

        ns3::Ptr<SmartGraph> SmartGraphCopy = this->copy();
	
	std::set< ns3::Ptr<SmartNode> > nodesChanged = node->getNeighbors();

	std::set< ns3::Ptr<SmartNode> > nodesGraph = this->getNodes();      

	pathLenght = std::numeric_limits<float>::max(); 

	for (std::set< ns3::Ptr<SmartNode> >::iterator nodesIt = nodesGraph.begin(); nodesIt != nodesGraph.end(); ++nodesIt)
	{
		if (node != *nodesIt )
		{
			pathSol = Network_Plan(node, *nodesIt, true, visited_nodes, 0);
	
			if (!pathSol->getNodes().empty())
			{
				float aux = pathSol->getEC();	
		        	if ( aux < EC) {
		        		EC = pathSol->getEC();
		        		result = pathSol;
					pathSol->getPath().clear();
				}
			}
		}
	}

	if(!result->getNodes().empty())
	{
		std::set< ns3::Ptr<SmartEdge> > auxEdges = result->getPath();
		if (!auxEdges.empty())
		{
			ns3::Ptr<SmartMigration> auxMigration; 
			
			for (std::set< ns3::Ptr<SmartEdge> >::iterator pathIt = auxEdges.begin(); pathIt != auxEdges.end(); ++pathIt)
			{
				auxMigration = new SmartMigration(*pathIt, ADD);
				migration->add(auxMigration);
			}

			float auxEC = migration->getEC();
			migration->setEC(auxEC + result->getPath().size());
		}
	}

	return migration;
}


ns3::Ptr<SmartMigration> SmartGraph::merge (ns3::Ptr<SmartProvider> center, ns3::Ptr<SmartNode> node)
{
        ns3::Ptr<SmartMigration> migration = new SmartMigration(), migrationAux;

        std::set< ns3::Ptr<SmartNode> >::iterator it;

      	ns3::Ptr<SmartGraph> SmartGraphCopy = this->copy();
       	std::set< ns3::Ptr<SmartNode> > nodesChanged = node->getNeighbors();

      	SmartGraphCopy->setProvider(center);

      	// Destroy connections to reconnect them later
       	for (it = nodesChanged.begin(); it != nodesChanged.end(); ++it)
      	        SmartGraphCopy->Delete(*it);
      	
	// Add the new node to the set of nodes to reconnect
      	nodesChanged.insert(node);

      	// Find the best combination from every node in the set towards the Provider "center"
      	it = nodesChanged.begin();
      	while ( nodesChanged.size() > 0 )
      	{
       	        std::advance(it, rand()%(nodesChanged.size()));
		migrationAux = SmartGraphCopy->merge(*it);
		if (migrationAux == NULL)
			printf("migrationAux == NULL\n");
     	        migration->add(migrationAux);
      	        it = nodesChanged.begin();
       	}

      	return migration;
}

ns3::Ptr<SmartMigration> SmartGraph::merge (ns3::Ptr<SmartGraph> microcloud)
{
	std::set< ns3::Ptr<SmartNode> > nodesList = microcloud->getNodes(), nodesListAux = getNodes();
	ns3::Ptr<SmartMigration> migration, migrationAux;
	migration = new SmartMigration();
	migrationAux = new SmartMigration();

	float num = std::numeric_limits<float>::max();
        migration->setEC(num);
	int aux;
	
	for (std::set< ns3::Ptr<SmartNode> >::iterator it = nodesList.begin(); it != nodesList.end(); ++it)
	{
		if (*it == NULL)
		{
			throw std::invalid_argument( "NULL NODE" );
		}

		migrationAux = this->merge(*it);
		if (migrationAux != NULL && !migrationAux->getMigrations().empty()) {
			if(migration->getMigrations().empty() || migrationAux->getEC() < migration->getEC() && migrationAux->getEC() > 0) {
				migration = ns3::Ptr<SmartMigration> (migrationAux);
				aux = (*it)->getNode()->GetId();
			}
		}
		else {	
			printf("ERROR IN MERGE\n");
		}
	}
	
	// Merge the two microclouds
	std::set< ns3::Ptr<SmartEdge> > edges = this->getEdges();

	for (std::set< ns3::Ptr<SmartEdge> >::iterator it = edges.begin(); it != edges.end(); ++it)
	{
		migration->add(new SmartMigration(*it));
	}

	float auxEC = migration->getEC();
	
	migration->setEC(auxEC + edges.size());

	if (migration == NULL)
		printf("error merging\n");

	return migration;
}
