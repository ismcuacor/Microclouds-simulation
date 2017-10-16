#ifndef SCHEDULERSETTER_CPP
#include "scheduler_setter.cpp"
#endif

#include "ns3/flow-monitor-module.h"
#include "ns3/internet-module.h"
using namespace ns3;
NS_LOG_COMPONENT_DEFINE ("SimpleRenater");
ns3::OnOffNodeHelper onoff2;

int
main (int argc, char *argv[])
{
	// Log level
	LogComponentEnable ("UdpEchoClientApplication", LOG_LEVEL_INFO);
	LogComponentEnable ("UdpEchoServerApplication", LOG_LEVEL_INFO);

	// Each time there is an interface event, the routing is launched.
	Config::SetDefault ("ns3::Ipv4GlobalRouting::RespondToInterfaceEvents", BooleanValue (true));

 	// Create nodes in a container
	NodeContainer nodes;
	nodes.Create (41);

	// Create Internet stack (including loopback interfaces)
	InternetStackHelper stack;
	stack.Install (nodes);

	// Add Energy Parameters to the nodes
	BasicNodeEnergyHelper basicNodeEnergy;
	basicNodeEnergy.Set ("OnConso", DoubleValue (657.245));
	basicNodeEnergy.Set ("OffConso", DoubleValue (0.2));
	basicNodeEnergy.Install (nodes);

	// Create channels
	PointToPointHelper pointtopoint;
	pointtopoint.SetDeviceAttribute ("DataRate", StringValue ("1000Mbps"));
	pointtopoint.SetChannelAttribute ("Delay", StringValue ("10ms"));

	NetDeviceContainer d0d25,d1d22,d1d38,d2d33,d2d10,d3d22,d3d38,d3d27,d4d28,d4d13,d5d23,d5d19,d5d12,d6d31,d6d38,d7d3,d7d38,d7d16,d8d38,d9d19,d10d38,d11d18,d11d12,d12d38,d13d38,d13d32,d14d38,d14d1,d15d38,d15d8,d16d38,d16d27,d17d36,d17d38,d18d38,d18d7,d18d10,d18d12,d18d19,d18d25,d19d23,d19d38,d19d20,d20d38,d20d34,d21d33,d21d38,d22d38,d22d30,d23d38,d24d38,d24d27,d25d29,d25d8,d25d15,d25d31,d25d24,d25d38,d26d3,d26d38,d27d38,d28d38,d28d17,d29d38,d29d21,d30d6,d30d38,d31d38,d32d30,d32d38,d33d38,d34d3,d34d26,d34d38,d35d24,d35d14,d36d22,d36d38,d37d24,d38d11,d39d8;

	d0d25 = pointtopoint.Install (nodes.Get(0), nodes.Get(25));
	d1d22 = pointtopoint.Install (nodes.Get(1), nodes.Get(22));
	d1d38 = pointtopoint.Install (nodes.Get(1), nodes.Get(38));
	d2d33 = pointtopoint.Install (nodes.Get(2), nodes.Get(33));
	d2d10 = pointtopoint.Install (nodes.Get(2), nodes.Get(10));
	d3d22 = pointtopoint.Install (nodes.Get(3), nodes.Get(22));
	d3d38 = pointtopoint.Install (nodes.Get(3), nodes.Get(38));
	d3d27 = pointtopoint.Install (nodes.Get(3), nodes.Get(27));
	d4d28 = pointtopoint.Install (nodes.Get(4), nodes.Get(28));
	d4d13 = pointtopoint.Install (nodes.Get(4), nodes.Get(13));
	d5d23 = pointtopoint.Install (nodes.Get(5), nodes.Get(23));
	d5d19 = pointtopoint.Install (nodes.Get(5), nodes.Get(19));
	d5d12 = pointtopoint.Install (nodes.Get(5), nodes.Get(12));
	d6d31 = pointtopoint.Install (nodes.Get(6), nodes.Get(31));
	d6d38 = pointtopoint.Install (nodes.Get(6), nodes.Get(38));
	d7d3 = pointtopoint.Install (nodes.Get(7), nodes.Get(3));
	d7d38 = pointtopoint.Install (nodes.Get(7), nodes.Get(38));
	d7d16 = pointtopoint.Install (nodes.Get(7), nodes.Get(16));
	d8d38 = pointtopoint.Install (nodes.Get(8), nodes.Get(38));
	d9d19 = pointtopoint.Install (nodes.Get(9), nodes.Get(19));
	d10d38 = pointtopoint.Install (nodes.Get(10), nodes.Get(38));
	d11d18 = pointtopoint.Install (nodes.Get(11), nodes.Get(18));
	d11d12 = pointtopoint.Install (nodes.Get(11), nodes.Get(12));
	d12d38 = pointtopoint.Install (nodes.Get(12), nodes.Get(38));
	d13d38 = pointtopoint.Install (nodes.Get(13), nodes.Get(38));
	d13d32 = pointtopoint.Install (nodes.Get(13), nodes.Get(32));
	d14d38 = pointtopoint.Install (nodes.Get(14), nodes.Get(38));
	d14d1 = pointtopoint.Install (nodes.Get(14), nodes.Get(1));
	d15d38 = pointtopoint.Install (nodes.Get(15), nodes.Get(38));
	d15d8 = pointtopoint.Install (nodes.Get(15), nodes.Get(8));
	d16d38 = pointtopoint.Install (nodes.Get(16), nodes.Get(38));
	d16d27 = pointtopoint.Install (nodes.Get(16), nodes.Get(27));
	d17d36 = pointtopoint.Install (nodes.Get(17), nodes.Get(36));
	d17d38 = pointtopoint.Install (nodes.Get(17), nodes.Get(38));
	d18d38 = pointtopoint.Install (nodes.Get(18), nodes.Get(38));
	d18d7 = pointtopoint.Install (nodes.Get(18), nodes.Get(7));
	d18d10 = pointtopoint.Install (nodes.Get(18), nodes.Get(10));
	d18d12 = pointtopoint.Install (nodes.Get(18), nodes.Get(12));
	d18d19 = pointtopoint.Install (nodes.Get(18), nodes.Get(19));
	d18d25 = pointtopoint.Install (nodes.Get(18), nodes.Get(25));
	d19d23 = pointtopoint.Install (nodes.Get(19), nodes.Get(23));
	d19d38 = pointtopoint.Install (nodes.Get(19), nodes.Get(38));
	d19d20 = pointtopoint.Install (nodes.Get(19), nodes.Get(20));
	d20d38 = pointtopoint.Install (nodes.Get(20), nodes.Get(38));
	d20d34 = pointtopoint.Install (nodes.Get(20), nodes.Get(34));
	d21d33 = pointtopoint.Install (nodes.Get(21), nodes.Get(33));
	d21d38 = pointtopoint.Install (nodes.Get(21), nodes.Get(38));
	d22d38 = pointtopoint.Install (nodes.Get(22), nodes.Get(38));
	d22d30 = pointtopoint.Install (nodes.Get(22), nodes.Get(30));
	d23d38 = pointtopoint.Install (nodes.Get(23), nodes.Get(38));
	d24d38 = pointtopoint.Install (nodes.Get(24), nodes.Get(38));
	d24d27 = pointtopoint.Install (nodes.Get(24), nodes.Get(27));
	d25d29 = pointtopoint.Install (nodes.Get(25), nodes.Get(29));
	d25d8 = pointtopoint.Install (nodes.Get(25), nodes.Get(8));
	d25d15 = pointtopoint.Install (nodes.Get(25), nodes.Get(15));
	d25d31 = pointtopoint.Install (nodes.Get(25), nodes.Get(31));
	d25d24 = pointtopoint.Install (nodes.Get(25), nodes.Get(24));
	d25d38 = pointtopoint.Install (nodes.Get(25), nodes.Get(38));
	d26d3 = pointtopoint.Install (nodes.Get(26), nodes.Get(3));
	d26d38 = pointtopoint.Install (nodes.Get(26), nodes.Get(38));
	d27d38 = pointtopoint.Install (nodes.Get(27), nodes.Get(38));
	d28d38 = pointtopoint.Install (nodes.Get(28), nodes.Get(38));
	d28d17 = pointtopoint.Install (nodes.Get(28), nodes.Get(17));
	d29d38 = pointtopoint.Install (nodes.Get(29), nodes.Get(38));
	d29d21 = pointtopoint.Install (nodes.Get(29), nodes.Get(21));
	d30d6 = pointtopoint.Install (nodes.Get(30), nodes.Get(6));
	d30d38 = pointtopoint.Install (nodes.Get(30), nodes.Get(38));
	d31d38 = pointtopoint.Install (nodes.Get(31), nodes.Get(38));
	d32d30 = pointtopoint.Install (nodes.Get(32), nodes.Get(30));
	d32d38 = pointtopoint.Install (nodes.Get(32), nodes.Get(38));
	d33d38 = pointtopoint.Install (nodes.Get(33), nodes.Get(38));
	d34d3 = pointtopoint.Install (nodes.Get(34), nodes.Get(3));
	d34d26 = pointtopoint.Install (nodes.Get(34), nodes.Get(26));
	d34d38 = pointtopoint.Install (nodes.Get(34), nodes.Get(38));
	d35d24 = pointtopoint.Install (nodes.Get(35), nodes.Get(24));
	d35d14 = pointtopoint.Install (nodes.Get(35), nodes.Get(14));
	d36d22 = pointtopoint.Install (nodes.Get(36), nodes.Get(22));
	d36d38 = pointtopoint.Install (nodes.Get(36), nodes.Get(38));
	d37d24 = pointtopoint.Install (nodes.Get(37), nodes.Get(24));
	d38d11 = pointtopoint.Install (nodes.Get(38), nodes.Get(11));
	d39d8 = pointtopoint.Install (nodes.Get(39), nodes.Get(8));

	// Add energy parameters to the interfaces
	LinearNetdeviceEnergyHelper linearNetdeviceEnergy;
	linearNetdeviceEnergy.Set ("IdleConso", DoubleValue (1.5));
	linearNetdeviceEnergy.Set ("OffConso", DoubleValue (0.0));
	linearNetdeviceEnergy.Set ("ByteEnergy", DoubleValue (3.4));
	linearNetdeviceEnergy.Install (nodes);

	ConsumptionLogger conso;
	conso.NodeConso (Seconds (0.5), Seconds (2744.139061), nodes);

	// Assign Internet addresses
	Ipv4AddressHelper address0a25,address1a22,address1a38,address2a33,address2a10,address3a22,address3a38,address3a27,address4a28,address4a13,address5a23,address5a19,address5a12,address6a31,address6a38,address7a3,address7a38,address7a16,address8a38,address9a19,address10a38,address11a18,address11a12,address12a38,address13a38,address13a32,address14a38,address14a1,address15a38,address15a8,address16a38,address16a27,address17a36,address17a38,address18a38,address18a7,address18a10,address18a12,address18a19,address18a25,address19a23,address19a38,address19a20,address20a38,address20a34,address21a33,address21a38,address22a38,address22a30,address23a38,address24a38,address24a27,address25a29,address25a8,address25a15,address25a31,address25a24,address25a38,address26a3,address26a38,address27a38,address28a38,address28a17,address29a38,address29a21,address30a6,address30a38,address31a38,address32a30,address32a38,address33a38,address34a3,address34a26,address34a38,address35a24,address35a14,address36a22,address36a38,address37a24,address38a11,address39a8;

	address0a25.SetBase ("10.0.1.136", "255.255.255.252"); 
	address1a22.SetBase ("10.0.1.52", "255.255.255.252"); 
	address1a38.SetBase ("10.0.1.64", "255.255.255.252"); 
	address2a33.SetBase ("10.0.0.188", "255.255.255.252"); 
	address2a10.SetBase ("10.0.0.192", "255.255.255.252"); 
	address3a22.SetBase ("10.0.0.120", "255.255.255.252"); 
	address3a38.SetBase ("10.0.0.148", "255.255.255.252"); 
	address3a27.SetBase ("10.0.0.132", "255.255.255.252"); 
	address4a28.SetBase ("10.0.1.128", "255.255.255.252"); 
	address4a13.SetBase ("10.0.1.132", "255.255.255.252"); 
	address5a23.SetBase ("10.0.0.236", "255.255.255.252"); 
	address5a19.SetBase ("10.0.0.240", "255.255.255.252"); 
	address5a12.SetBase ("10.0.0.244", "255.255.255.252"); 
	address6a31.SetBase ("10.0.0.48", "255.255.255.252"); 
	address6a38.SetBase ("10.0.0.56", "255.255.255.252"); 
	address7a3.SetBase ("10.0.0.168", "255.255.255.252"); 
	address7a38.SetBase ("10.0.1.32", "255.255.255.252"); 
	address7a16.SetBase ("10.0.0.176", "255.255.255.252"); 
	address8a38.SetBase ("10.0.0.36", "255.255.255.252"); 
	address9a19.SetBase ("10.0.0.248", "255.255.255.252"); 
	address10a38.SetBase ("10.0.1.36", "255.255.255.252"); 
	address11a18.SetBase ("10.0.1.80", "255.255.255.252"); 
	address11a12.SetBase ("10.0.1.84", "255.255.255.252"); 
	address12a38.SetBase ("10.0.0.252", "255.255.255.252"); 
	address13a38.SetBase ("10.0.1.140", "255.255.255.252"); 
	address13a32.SetBase ("10.0.1.124", "255.255.255.252"); 
	address14a38.SetBase ("10.0.1.72", "255.255.255.252"); 
	address14a1.SetBase ("10.0.1.68", "255.255.255.252"); 
	address15a38.SetBase ("10.0.1.4", "255.255.255.252"); 
	address15a8.SetBase ("10.0.0.40", "255.255.255.252"); 
	address16a38.SetBase ("10.0.0.180", "255.255.255.252"); 
	address16a27.SetBase ("10.0.0.116", "255.255.255.252"); 
	address17a36.SetBase ("10.0.1.96", "255.255.255.252"); 
	address17a38.SetBase ("10.0.1.108", "255.255.255.252"); 
	address18a38.SetBase ("10.0.1.88", "255.255.255.252"); 
	address18a7.SetBase ("10.0.1.40", "255.255.255.252"); 
	address18a10.SetBase ("10.0.1.44", "255.255.255.252"); 
	address18a12.SetBase ("10.0.1.48", "255.255.255.252"); 
	address18a19.SetBase ("10.0.1.56", "255.255.255.252"); 
	address18a25.SetBase ("10.0.1.60", "255.255.255.252"); 
	address19a23.SetBase ("10.0.0.216", "255.255.255.252"); 
	address19a38.SetBase ("10.0.1.0", "255.255.255.252"); 
	address19a20.SetBase ("10.0.0.224", "255.255.255.252"); 
	address20a38.SetBase ("10.0.0.228", "255.255.255.252"); 
	address20a34.SetBase ("10.0.0.164", "255.255.255.252"); 
	address21a33.SetBase ("10.0.0.8", "255.255.255.252"); 
	address21a38.SetBase ("10.0.0.20", "255.255.255.252"); 
	address22a38.SetBase ("10.0.0.124", "255.255.255.252"); 
	address22a30.SetBase ("10.0.0.72", "255.255.255.252"); 
	address23a38.SetBase ("10.0.0.220", "255.255.255.252"); 
	address24a38.SetBase ("10.0.1.76", "255.255.255.252"); 
	address24a27.SetBase ("10.0.0.108", "255.255.255.252"); 
	address25a29.SetBase ("10.0.1.8", "255.255.255.252"); 
	address25a8.SetBase ("10.0.1.12", "255.255.255.252"); 
	address25a15.SetBase ("10.0.1.16", "255.255.255.252"); 
	address25a31.SetBase ("10.0.1.20", "255.255.255.252"); 
	address25a24.SetBase ("10.0.1.24", "255.255.255.252"); 
	address25a38.SetBase ("10.0.1.28", "255.255.255.252"); 
	address26a3.SetBase ("10.0.0.140", "255.255.255.252"); 
	address26a38.SetBase ("10.0.0.152", "255.255.255.252"); 
	address27a38.SetBase ("10.0.0.136", "255.255.255.252"); 
	address28a38.SetBase ("10.0.1.144", "255.255.255.252"); 
	address28a17.SetBase ("10.0.1.112", "255.255.255.252"); 
	address29a38.SetBase ("10.0.1.92", "255.255.255.252"); 
	address29a21.SetBase ("10.0.0.24", "255.255.255.252"); 
	address30a6.SetBase ("10.0.0.60", "255.255.255.252"); 
	address30a38.SetBase ("10.0.0.76", "255.255.255.252"); 
	address31a38.SetBase ("10.0.0.52", "255.255.255.252"); 
	address32a30.SetBase ("10.0.1.116", "255.255.255.252"); 
	address32a38.SetBase ("10.0.1.148", "255.255.255.252"); 
	address33a38.SetBase ("10.0.0.12", "255.255.255.252"); 
	address34a3.SetBase ("10.0.0.156", "255.255.255.252"); 
	address34a26.SetBase ("10.0.0.160", "255.255.255.252"); 
	address34a38.SetBase ("10.0.0.172", "255.255.255.252"); 
	address35a24.SetBase ("10.0.1.100", "255.255.255.252"); 
	address35a14.SetBase ("10.0.1.104", "255.255.255.252"); 
	address36a22.SetBase ("10.0.1.120", "255.255.255.252"); 
	address36a38.SetBase ("10.0.1.152", "255.255.255.252"); 
	address37a24.SetBase ("10.0.1.156", "255.255.255.252"); 
	address38a11.SetBase ("192.168.0.0", "255.255.255.252"); 
	address39a8.SetBase ("192.168.0.4", "255.255.255.252"); 

	Ipv4InterfaceContainer interfaces0i25 = address0a25.Assign (d0d25);

	Ipv4InterfaceContainer interfaces1i22 = address1a22.Assign (d1d22);

	Ipv4InterfaceContainer interfaces1i38 = address1a38.Assign (d1d38);

	Ipv4InterfaceContainer interfaces2i33 = address2a33.Assign (d2d33);

	Ipv4InterfaceContainer interfaces2i10 = address2a10.Assign (d2d10);

	Ipv4InterfaceContainer interfaces3i22 = address3a22.Assign (d3d22);

	Ipv4InterfaceContainer interfaces3i38 = address3a38.Assign (d3d38);

	Ipv4InterfaceContainer interfaces3i27 = address3a27.Assign (d3d27);

	Ipv4InterfaceContainer interfaces4i28 = address4a28.Assign (d4d28);

	Ipv4InterfaceContainer interfaces4i13 = address4a13.Assign (d4d13);

	Ipv4InterfaceContainer interfaces5i23 = address5a23.Assign (d5d23);

	Ipv4InterfaceContainer interfaces5i19 = address5a19.Assign (d5d19);

	Ipv4InterfaceContainer interfaces5i12 = address5a12.Assign (d5d12);

	Ipv4InterfaceContainer interfaces6i31 = address6a31.Assign (d6d31);

	Ipv4InterfaceContainer interfaces6i38 = address6a38.Assign (d6d38);

	Ipv4InterfaceContainer interfaces7i3 = address7a3.Assign (d7d3);

	Ipv4InterfaceContainer interfaces7i38 = address7a38.Assign (d7d38);

	Ipv4InterfaceContainer interfaces7i16 = address7a16.Assign (d7d16);

	Ipv4InterfaceContainer interfaces8i38 = address8a38.Assign (d8d38);

	Ipv4InterfaceContainer interfaces9i19 = address9a19.Assign (d9d19);

	Ipv4InterfaceContainer interfaces10i38 = address10a38.Assign (d10d38);

	Ipv4InterfaceContainer interfaces11i18 = address11a18.Assign (d11d18);

	Ipv4InterfaceContainer interfaces11i12 = address11a12.Assign (d11d12);

	Ipv4InterfaceContainer interfaces12i38 = address12a38.Assign (d12d38);

	Ipv4InterfaceContainer interfaces13i38 = address13a38.Assign (d13d38);

	Ipv4InterfaceContainer interfaces13i32 = address13a32.Assign (d13d32);

	Ipv4InterfaceContainer interfaces14i38 = address14a38.Assign (d14d38);

	Ipv4InterfaceContainer interfaces14i1 = address14a1.Assign (d14d1);

	Ipv4InterfaceContainer interfaces15i38 = address15a38.Assign (d15d38);

	Ipv4InterfaceContainer interfaces15i8 = address15a8.Assign (d15d8);

	Ipv4InterfaceContainer interfaces16i38 = address16a38.Assign (d16d38);

	Ipv4InterfaceContainer interfaces16i27 = address16a27.Assign (d16d27);

	Ipv4InterfaceContainer interfaces17i36 = address17a36.Assign (d17d36);

	Ipv4InterfaceContainer interfaces17i38 = address17a38.Assign (d17d38);

	Ipv4InterfaceContainer interfaces18i38 = address18a38.Assign (d18d38);

	Ipv4InterfaceContainer interfaces18i7 = address18a7.Assign (d18d7);

	Ipv4InterfaceContainer interfaces18i10 = address18a10.Assign (d18d10);

	Ipv4InterfaceContainer interfaces18i12 = address18a12.Assign (d18d12);

	Ipv4InterfaceContainer interfaces18i19 = address18a19.Assign (d18d19);

	Ipv4InterfaceContainer interfaces18i25 = address18a25.Assign (d18d25);

	Ipv4InterfaceContainer interfaces19i23 = address19a23.Assign (d19d23);

	Ipv4InterfaceContainer interfaces19i38 = address19a38.Assign (d19d38);

	Ipv4InterfaceContainer interfaces19i20 = address19a20.Assign (d19d20);

	Ipv4InterfaceContainer interfaces20i38 = address20a38.Assign (d20d38);

	Ipv4InterfaceContainer interfaces20i34 = address20a34.Assign (d20d34);

	Ipv4InterfaceContainer interfaces21i33 = address21a33.Assign (d21d33);

	Ipv4InterfaceContainer interfaces21i38 = address21a38.Assign (d21d38);

	Ipv4InterfaceContainer interfaces22i38 = address22a38.Assign (d22d38);

	Ipv4InterfaceContainer interfaces22i30 = address22a30.Assign (d22d30);

	Ipv4InterfaceContainer interfaces23i38 = address23a38.Assign (d23d38);

	Ipv4InterfaceContainer interfaces24i38 = address24a38.Assign (d24d38);

	Ipv4InterfaceContainer interfaces24i27 = address24a27.Assign (d24d27);

	Ipv4InterfaceContainer interfaces25i29 = address25a29.Assign (d25d29);

	Ipv4InterfaceContainer interfaces25i8 = address25a8.Assign (d25d8);

	Ipv4InterfaceContainer interfaces25i15 = address25a15.Assign (d25d15);

	Ipv4InterfaceContainer interfaces25i31 = address25a31.Assign (d25d31);

	Ipv4InterfaceContainer interfaces25i24 = address25a24.Assign (d25d24);

	Ipv4InterfaceContainer interfaces25i38 = address25a38.Assign (d25d38);

	Ipv4InterfaceContainer interfaces26i3 = address26a3.Assign (d26d3);

	Ipv4InterfaceContainer interfaces26i38 = address26a38.Assign (d26d38);

	Ipv4InterfaceContainer interfaces27i38 = address27a38.Assign (d27d38);

	Ipv4InterfaceContainer interfaces28i38 = address28a38.Assign (d28d38);

	Ipv4InterfaceContainer interfaces28i17 = address28a17.Assign (d28d17);

	Ipv4InterfaceContainer interfaces29i38 = address29a38.Assign (d29d38);

	Ipv4InterfaceContainer interfaces29i21 = address29a21.Assign (d29d21);

	Ipv4InterfaceContainer interfaces30i6 = address30a6.Assign (d30d6);

	Ipv4InterfaceContainer interfaces30i38 = address30a38.Assign (d30d38);

	Ipv4InterfaceContainer interfaces31i38 = address31a38.Assign (d31d38);

	Ipv4InterfaceContainer interfaces32i30 = address32a30.Assign (d32d30);

	Ipv4InterfaceContainer interfaces32i38 = address32a38.Assign (d32d38);

	Ipv4InterfaceContainer interfaces33i38 = address33a38.Assign (d33d38);

	Ipv4InterfaceContainer interfaces34i3 = address34a3.Assign (d34d3);

	Ipv4InterfaceContainer interfaces34i26 = address34a26.Assign (d34d26);

	Ipv4InterfaceContainer interfaces34i38 = address34a38.Assign (d34d38);

	Ipv4InterfaceContainer interfaces35i24 = address35a24.Assign (d35d24);

	Ipv4InterfaceContainer interfaces35i14 = address35a14.Assign (d35d14);

	Ipv4InterfaceContainer interfaces36i22 = address36a22.Assign (d36d22);

	Ipv4InterfaceContainer interfaces36i38 = address36a38.Assign (d36d38);

	Ipv4InterfaceContainer interfaces37i24 = address37a24.Assign (d37d24);

	Ipv4InterfaceContainer interfaces38i11 = address38a11.Assign (d38d11);

	Ipv4InterfaceContainer interfaces39i8 = address39a8.Assign (d39d8);



	Ptr<NetDevice> netdev0n25,netdev1n22,netdev1n38,netdev2n33,netdev2n10,netdev3n22,netdev3n38,netdev3n27,netdev4n28,netdev4n13,netdev5n23,netdev5n19,netdev5n12,netdev6n31,netdev6n38,netdev7n3,netdev7n38,netdev7n16,netdev8n38,netdev9n19,netdev10n38,netdev11n18,netdev11n12,netdev12n38,netdev13n38,netdev13n32,netdev14n38,netdev14n1,netdev15n38,netdev15n8,netdev16n38,netdev16n27,netdev17n36,netdev17n38,netdev18n38,netdev18n7,netdev18n10,netdev18n12,netdev18n19,netdev18n25,netdev19n23,netdev19n38,netdev19n20,netdev20n38,netdev20n34,netdev21n33,netdev21n38,netdev22n38,netdev22n30,netdev23n38,netdev24n38,netdev24n27,netdev25n29,netdev25n8,netdev25n15,netdev25n31,netdev25n24,netdev25n38,netdev26n3,netdev26n38,netdev27n38,netdev28n38,netdev28n17,netdev29n38,netdev29n21,netdev30n6,netdev30n38,netdev31n38,netdev32n30,netdev32n38,netdev33n38,netdev34n3,netdev34n26,netdev34n38,netdev35n24,netdev35n14,netdev36n22,netdev36n38,netdev37n24,netdev38n11,netdev39n8;
	netdev0n25 = (d0d25).Get (0);
	netdev1n22 = (d1d22).Get (0);
	netdev1n38 = (d1d38).Get (0);
	netdev2n33 = (d2d33).Get (0);
	netdev2n10 = (d2d10).Get (0);
	netdev3n22 = (d3d22).Get (0);
	netdev3n38 = (d3d38).Get (0);
	netdev3n27 = (d3d27).Get (0);
	netdev4n28 = (d4d28).Get (0);
	netdev4n13 = (d4d13).Get (0);
	netdev5n23 = (d5d23).Get (0);
	netdev5n19 = (d5d19).Get (0);
	netdev5n12 = (d5d12).Get (0);
	netdev6n31 = (d6d31).Get (0);
	netdev6n38 = (d6d38).Get (0);
	netdev7n3 = (d7d3).Get (0);
	netdev7n38 = (d7d38).Get (0);
	netdev7n16 = (d7d16).Get (0);
	netdev8n38 = (d8d38).Get (0);
	netdev9n19 = (d9d19).Get (0);
	netdev10n38 = (d10d38).Get (0);
	netdev11n18 = (d11d18).Get (0);
	netdev11n12 = (d11d12).Get (0);
	netdev12n38 = (d12d38).Get (0);
	netdev13n38 = (d13d38).Get (0);
	netdev13n32 = (d13d32).Get (0);
	netdev14n38 = (d14d38).Get (0);
	netdev14n1 = (d14d1).Get (0);
	netdev15n38 = (d15d38).Get (0);
	netdev15n8 = (d15d8).Get (0);
	netdev16n38 = (d16d38).Get (0);
	netdev16n27 = (d16d27).Get (0);
	netdev17n36 = (d17d36).Get (0);
	netdev17n38 = (d17d38).Get (0);
	netdev18n38 = (d18d38).Get (0);
	netdev18n7 = (d18d7).Get (0);
	netdev18n10 = (d18d10).Get (0);
	netdev18n12 = (d18d12).Get (0);
	netdev18n19 = (d18d19).Get (0);
	netdev18n25 = (d18d25).Get (0);
	netdev19n23 = (d19d23).Get (0);
	netdev19n38 = (d19d38).Get (0);
	netdev19n20 = (d19d20).Get (0);
	netdev20n38 = (d20d38).Get (0);
	netdev20n34 = (d20d34).Get (0);
	netdev21n33 = (d21d33).Get (0);
	netdev21n38 = (d21d38).Get (0);
	netdev22n38 = (d22d38).Get (0);
	netdev22n30 = (d22d30).Get (0);
	netdev23n38 = (d23d38).Get (0);
	netdev24n38 = (d24d38).Get (0);
	netdev24n27 = (d24d27).Get (0);
	netdev25n29 = (d25d29).Get (0);
	netdev25n8 = (d25d8).Get (0);
	netdev25n15 = (d25d15).Get (0);
	netdev25n31 = (d25d31).Get (0);
	netdev25n24 = (d25d24).Get (0);
	netdev25n38 = (d25d38).Get (0);
	netdev26n3 = (d26d3).Get (0);
	netdev26n38 = (d26d38).Get (0);
	netdev27n38 = (d27d38).Get (0);
	netdev28n38 = (d28d38).Get (0);
	netdev28n17 = (d28d17).Get (0);
	netdev29n38 = (d29d38).Get (0);
	netdev29n21 = (d29d21).Get (0);
	netdev30n6 = (d30d6).Get (0);
	netdev30n38 = (d30d38).Get (0);
	netdev31n38 = (d31d38).Get (0);
	netdev32n30 = (d32d30).Get (0);
	netdev32n38 = (d32d38).Get (0);
	netdev33n38 = (d33d38).Get (0);
	netdev34n3 = (d34d3).Get (0);
	netdev34n26 = (d34d26).Get (0);
	netdev34n38 = (d34d38).Get (0);
	netdev35n24 = (d35d24).Get (0);
	netdev35n14 = (d35d14).Get (0);
	netdev36n22 = (d36d22).Get (0);
	netdev36n38 = (d36d38).Get (0);
	netdev37n24 = (d37d24).Get (0);
	netdev38n11 = (d38d11).Get (0);
	netdev39n8 = (d39d8).Get (0);

	// Switch off nodes
	for(ns3::NodeContainer::Iterator node = nodes.Begin(); node != nodes.End(); node++)
	{
		onoff2.Set ("SwitchOffEnergy", DoubleValue (30.0));
		onoff2.Set ("SwitchOffDuration", DoubleValue (2.0));
		onoff2.Set ("SwitchOnEnergy", DoubleValue (60.0));
		onoff2.Set ("SwitchOnDuration", DoubleValue (3.0));
		onoff2.Install (*node);
		onoff2.NodeSwitchOff (*node, Seconds (3.0));
		onoff2.NodeSwitchOn (*node, Seconds (6.0));
	}

	// Create GOD routing and tables
	Ipv4GlobalRoutingHelper::PopulateRoutingTables ();


	//Enable the traces
	pointtopoint.EnablePcapAll ("Test-simple");	// Install Flow Monitor
	FlowMonitorHelper flowmon;
	Ptr<FlowMonitor> monitor = flowmon.InstallAll();

	Simulator::Stop (Seconds(2744.139061));
	Simulator::Run ();
	//PrintDrop();

	// Print per flow statistics
	monitor->CheckForLostPackets ();
	Ptr<Ipv4FlowClassifier> classifier = DynamicCast<Ipv4FlowClassifier> (flowmon.GetClassifier ());
	std::map<FlowId, FlowMonitor::FlowStats> stats = monitor->GetFlowStats ();
	std::filebuf fb;
	for (std::map<FlowId, FlowMonitor::FlowStats>::const_iterator iter = stats.begin (); iter != stats.end (); ++iter)
	{
		Ipv4FlowClassifier::FiveTuple t = classifier->FindFlow (iter->first);
		NS_LOG_UNCOND("Flow ID: " << iter->first << " Src Addr " << t.sourceAddress << " Dst Addr " << t.destinationAddress);
		NS_LOG_UNCOND("Tx Packets = " << iter->second.txPackets);
		NS_LOG_UNCOND("Rx Packets = " << iter->second.rxPackets);
		NS_LOG_UNCOND("Throughput: " << iter->second.rxBytes * 8.0 / (iter->second.timeLastRxPacket.GetSeconds()-iter->second.timeFirstTxPacket.GetSeconds()) / 1024  << " Kbps");
	}
	monitor->SerializeToXmlFile("histogram-smarter-sequential.flowmon", true, true);	Simulator::Destroy ();
	return 0;
}
