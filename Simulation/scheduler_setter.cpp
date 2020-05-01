#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/csma-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"
#include "ns3/ecofen-module.h"

#ifndef DEEPAC_CPP
#include "DEEPACC.cpp"
#endif

std::set<ns3::Ptr<SmartEdge> > edgesList;
ns3::Ptr<EdgeConfiguration> configuration = new EdgeConfiguration();
NodeContainer nodes;

LinearNetdeviceEnergyHelper linearNetdeviceEnergy;

using namespace SmartNet;

class SchedulerSetter: public ns3::Application 
{
   private:
	ns3::Ptr<SmartGraph> graph; 
	ns3::Ptr<DEEPACC> protocol; 
	std::set< ns3::Ptr<SmartNode> > talking_nodes;
  	ns3::Ptr<SmartManager> manager;
	PointToPointHelper pointtopoint;
	std::set<int> sInt;
   public:
	SchedulerSetter ()
	{
		this->graph;
		this->protocol;
		this->talking_nodes;
		linearNetdeviceEnergy.Set ("IdleConso", DoubleValue (1.5));
		linearNetdeviceEnergy.Set ("OffConso", DoubleValue (0.0));
		linearNetdeviceEnergy.Set ("ByteEnergy", DoubleValue (3.4));
	}

	SchedulerSetter (ns3::Ptr<SmartGraph> graph, ns3::Ptr<DEEPACC> protocol, std::set< ns3::Ptr<SmartNode> > talking_nodes)
	{
		this->graph = graph;
		this->protocol = protocol;
		this->talking_nodes = talking_nodes;
		linearNetdeviceEnergy.Set ("IdleConso", DoubleValue (1.5));
		linearNetdeviceEnergy.Set ("OffConso", DoubleValue (0.0));
		linearNetdeviceEnergy.Set ("ByteEnergy", DoubleValue (3.4));
	}

	SchedulerSetter (ns3::Ptr<SmartGraph> graph, ns3::Ptr<DEEPACC> protocol, std::set< ns3::Ptr<SmartNode> > talking_nodes, PointToPointHelper ptp, std::set<int> sInt)
	{
		this->graph = graph;
		this->protocol = protocol;
		this->talking_nodes = talking_nodes;
		pointtopoint = ptp;
		this->sInt = sInt;
		linearNetdeviceEnergy.Set ("IdleConso", DoubleValue (1.5));
		linearNetdeviceEnergy.Set ("OffConso", DoubleValue (0.0));
		linearNetdeviceEnergy.Set ("ByteEnergy", DoubleValue (3.4));
	}

	void schedulerSet()
	{
		manager = new SmartManager(graph, protocol, talking_nodes, sInt);

		for (std::set<ns3::Ptr<SmartNode> >::iterator it = used_nodes.begin(); it != used_nodes.end(); ++it)
			active_nodes.insert((*it)->getNode());

		for (ns3::NodeContainer::Iterator it = nodes.Begin(); it != nodes.End(); ++it)
		{
			if (active_nodes.find(*it) == active_nodes.end())
			        //TODO
				onoff.NodeSwitchOff (*it, Seconds(Simulator::Now().GetSeconds()));				
		}

                for (std::set< ns3::Ptr<SmartNode> >::iterator it = talking_nodes.begin(); it != talking_nodes.end(); ++it)
                        for (std::set< ns3::Ptr<SmartNode> >::iterator it2 = talking_nodes.begin(); it2 != talking_nodes.end(); ++it2)
                                if (*it != *it2)
                                        schedule_path_finding(*it, *it2);

                ns3::Ipv4GlobalRoutingHelper::PopulateRoutingTables ();
                ns3::Ipv4GlobalRoutingHelper::RecomputeRoutingTables ();
	}

	void addEdge (ns3::Ptr<SmartNode> n1, ns3::Ptr<SmartNode> n2, Ipv4AddressHelper * address)
	{
		if (!Graph.getEdge(n1, n2))
		{
			int x = n1->getNode()->GetId(), y = n2->getNode()->GetId();
			Ipv4AddressHelper address2;
			address2.SetBase ("192.168.0.8", "255.255.255.252");
			NetDeviceContainer d = pointtopoint.Install (n1->getNode(), n2->getNode());

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

			//address->NewNetwork ();
			Ipv4Address ipAddress = address->NewAddress ();
			Ipv4InterfaceAddress ipv4Addr = Ipv4InterfaceAddress (ipAddress, "255.255.255.252");
			ipv4->AddAddress (interface, ipv4Addr);
			ipv4->SetMetric (interface, 1);
		
			Ipv4Address ipAddress_2 = address->NewAddress ();
			Ipv4InterfaceAddress ipv4Addr_2 = Ipv4InterfaceAddress (ipAddress_2, "255.255.255.252");
			ipv4_2->AddAddress (interface_2, ipv4Addr_2);
			ipv4_2->SetMetric (interface_2, 1);
	
			ipv4->SetUp (interface);
			retval.Add (ipv4, interface);

			ipv4_2->SetUp (interface_2);
			retval.Add (ipv4_2, interface_2);

			linearNetdeviceEnergy.Install(d);
			interfacestable [x] [y] = &retval;
			interfacestable [y] [x] = &retval;
			ns3::Ptr<SmartEdge> edge = new SmartEdge(n1, n2, configuration, 0, retval);
			Graph.addEdge(edge);
			edgesList.insert(edge);
			recalculate();
		}
	}

	void deleteNode (ns3::Ptr<SmartNode> n)
	{
        	Graph.Delete(n);		

		for (std::set< ns3::Ptr<SmartEdge> >::iterator it = edgesList.begin(); it != edgesList.end(); ++it)
		{
			if ((*it)->getNode1() == n || (*it)->getNode2() == n)
			{
				edgesList.erase(it);
			}
		}

		recalculate();
	}

	void recalculate ()
	{
		graph = new SmartGraph(edgesList);
		used_nodes.clear();

		manager = new SmartManager(graph, protocol, talking_nodes, sInt);
		std::set< ns3::Ptr<Node> > switch_off_nodes;
		
		for (std::set< ns3::Ptr<Node> >::iterator it = active_nodes.begin(); it != active_nodes.end(); ++it)
		{
			switch_off_nodes.insert(*it);
		}

		for (std::set< ns3::Ptr<SmartNode> >::iterator it = used_nodes.begin(); it != used_nodes.end(); ++it)
		{
			if (switch_off_nodes.find((*it)->getNode()) != switch_off_nodes.end())
			{
				switch_off_nodes.erase((*it)->getNode());
			}
		}

		for (std::set< ns3::Ptr<Node> >::iterator it = switch_off_nodes.begin(); it != switch_off_nodes.end(); ++it)
                {
			active_nodes.erase(*it);
		}

                ns3::Ipv4GlobalRoutingHelper::PopulateRoutingTables ();
                ns3::Ipv4GlobalRoutingHelper::RecomputeRoutingTables ();
		Simulator::Schedule(ns3::Seconds(Simulator::Now().GetSeconds() + 1), &SchedulerSetter::scheduleAfter, this);
	}

	void reconfigure (std::pair<Ipv4InterfaceContainer*, ns3::Ptr<NetDevice> > intnetdev, NetDeviceContainer * d2, Ipv4AddressHelper * a, int nodes2[]) 
	{
		int x = nodes2[0];
		int y = nodes2[1];
		int z = nodes2[2];

		std::cout << "TOUCHES NODES " << x << " , " << y << "  AND  " << z << std::endl;

		ns3::Ptr<SmartNode> node1 = smartnodesReferences[x];
		ns3::Ptr<SmartNode> node2 = smartnodesReferences[y];
		ns3::Ptr<SmartNode> node3 = smartnodesReferences[z];

		NetDeviceContainer d = pointtopoint.Install (nodes.Get(x), nodes.Get(y));

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

		Ipv4Address ipAddress = interfacestable[x][z]->GetAddress(0);
		Ipv4InterfaceAddress ipv4Addr = Ipv4InterfaceAddress (ipAddress, "255.255.255.252");
		ipv4->AddAddress (interface, ipv4Addr);
		ipv4->SetMetric (interface, 1);
		
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
		interfacestable [x] [y] = interfacestable[x][z];
		interfacestable [y] [x] = interfacestable[x][z];
		//interfacestable [x] [z] = NULL;
		//interfacestable [z] [x] = NULL;

 		edgesList.insert(new SmartEdge(node1, node2, configuration, 0, *intnetdev.first));
		edgesList.erase (Graph.getEdge(node1,node3)); 	
		node1->delNeighbor(node3);
		node3->delNeighbor(node1);
		node2->addNeighbor(node1);
		node1->addNeighbor(node2);

		node1->mod=z;
	}

	void scheduleAfter()
	{
		int x, z, y;
		Ipv4Address ipAddress, ipAddress2;
		std::set< ns3::Ptr<SmartNode> > listNod = Graph.getNodes();

		for (std::set< ns3::Ptr<SmartNode> >::iterator it = listNod.begin(); it != listNod.end(); ++it)
		{
			if ((*it)->mod != -1)
			{
				z = (*it)->mod;
				x = (*it)->getNode()->GetId();
				(*it)->Neighbors2.erase(smartnodesReferences[z]);
				(*it)->Neighbors.erase(smartnodesReferences[z]);
				std::set< ns3::Ptr<SmartNode> > listNodes = (*it)->getNeighbors();
				for (std::set< ns3::Ptr<SmartNode> >::iterator it2 = listNodes.begin(); it2 != listNodes.end(); ++it2)
				{
					y = (*it2)->getNode()->GetId();
					ipAddress = Graph.getIpv4_1((*it), (*it2));
					(*it)->SetupNow(*it2, interfacestable[x][y]->GetAddress(1)); //ipAddress);
                        		(*it)->SetupNow(*it2, interfacestable[x][y]->GetAddress(0)); //ipAddress);
				}
				schedule_path_finding(prov2.find((*it)->getMCloud())->second, *it);

				std::set< ns3::Ptr<SmartNode> > ln = prov2.find((*it)->getMCloud())->second->getNeighbors();
				for ( std::set< ns3::Ptr<SmartNode> >::iterator it3 = ln.begin(); it3 != ln.end(); ++it3 )
				{
					if (used_nodes.find(*it3) != used_nodes.end())
					{
						ipAddress2 = Graph.getIpv4_1(*it3, prov2.find((*it)->getMCloud())->second);
						(*it)->SetupNow2(ipAddress2);
					}
				}
			}
			(*it)->mod = -1;
		}

                ns3::Ipv4GlobalRoutingHelper::PopulateRoutingTables ();
                ns3::Ipv4GlobalRoutingHelper::RecomputeRoutingTables ();
	}

	void add(ns3::Ptr<SmartNode> node)
	{
		manager->RunAdditionProtocol (node);	
		manager->Switch_On(1.6);
		Ipv4GlobalRoutingHelper::PopulateRoutingTables ();
		Ipv4GlobalRoutingHelper::RecomputeRoutingTables ();	
	}

        void check(ns3::Ptr<SmartNode> node1, ns3::Ptr<SmartNode> node2)
        {
		ns3::Ptr<SmartNode> nextNode = *(node1->getNeighbors().begin());
		float lenght = 0;
		ns3::Ptr<SmartNode> nodeFinal = node2;
		uint32_t id = nextNode->getNode()->GetId();
		ns3::Ptr<ns3::Node> node = node1->getNode();
		ns3::Ipv4Address address = Graph.getIpv4_0(node1, nextNode);
		LongTag tag;
		tag.SetSimpleValue(0xFF);

		uint8_t const *buffer = reinterpret_cast<uint8_t*>(&lenght);
		uint32_t size = sizeof(float);
		ns3::Ptr<ns3::Packet> pck = ns3::Create<ns3::Packet> (buffer, size);
		pck->AddPacketTag(tag);

		node1->SmartNode::Send_Packet_Now (nextNode, pck, address);

        }

	void printPath()
	{
		if (finalPath != NULL)
		{
			std::cout << finalPath->getEC() << std::endl;
		}
		exit(-1);
	}

	void schedule_path_finding (ns3::Ptr<SmartNode> node, ns3::Ptr<SmartNode> node2)
	{
		time_t timer;
		time(&timer);
		
		std::set<ns3::Ptr<SmartNode> > neighs = node->getNeighbors();
	        char * aux2 = "ini";
	        for (std::set<ns3::Ptr<SmartNode> >::iterator it = neighs.begin(); it != neighs.end(); ++it)
	        {
	                node->Check_Neighbors(*it, node2, 0, aux2);
	                destNodeGlobal.insert(std::pair<int, ns3::Ptr<SmartNode> > (node2->getMCloud(), node2));
	        }
		std::cout << "ELAPSED TIME " << timer << std::endl;
	}

	protected:
	
        		virtual void StartApplication (void){
				//schedulerSet(graph, protocol, talking_nodes);
				schedulerSet();
			}
        		virtual void StopApplication (void)
        		{
        		}
};
