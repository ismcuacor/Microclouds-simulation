#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/csma-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"
#include "ns3/ecofen-module.h"
#include "PacketSimulator.h"

int max ;
ns3::Ipv4InterfaceContainer * interfacestable [40][40] = {};

LinearNetdeviceEnergyHelper linearNetdeviceEnergy;

class SchedulerRenater: public ns3::Application 
{
   private:
	PointToPointHelper pointtopoint;
   public:
	SchedulerRenater (PointToPointHelper p)
	{
		pointtopoint = p;
	}

	void reconfigure (std::pair<Ipv4InterfaceContainer*, ns3::Ptr<NetDevice> > intnetdev, NetDeviceContainer * d2, Ipv4AddressHelper * a, int nodes2[], PacketSimulator * p) 
	{
		int x = nodes2[0];
		int y = nodes2[1];
		int z = nodes2[2];
		int z2 = nodes2[3];

		std::cout << "TOUCHES NODES " << x << " , " << y << "  AND  " << z << std::endl;

		ns3::Ptr<Node> node1 = nodes.Get(x);
		ns3::Ptr<Node> node2 = nodes.Get(y);
		ns3::Ptr<Node> node3 = nodes.Get(z);

		NetDeviceContainer d = pointtopoint.Install (node1, node2);

		*(netdevicestable+x*max + y) = &d;
		*(netdevicestable+y*max + x) = &d;

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
		Simulator::Schedule(ns3::Seconds(Simulator::Now().GetSeconds() + 6), &SchedulerSetter::scheduleAfter, this, x, y, z2, p);
	}
	

	void scheduleAfter(int x, int y, int z, PacketSimulator * p)
	{
		p->Setup(interfacestable[y] [25]->GetAddress(1), nodes.Get(x), TimesSenderList, SizesSenderList, x, 25);
		*(netdevicestable+x*max + z) = NULL;
		*(netdevicestable+z*max + x) = NULL;
                ns3::Ipv4GlobalRoutingHelper::PopulateRoutingTables ();
                ns3::Ipv4GlobalRoutingHelper::RecomputeRoutingTables ();
	}	

	protected:
	
        		virtual void StartApplication (void){
			}
        		virtual void StopApplication (void)
        		{
        		}
};
