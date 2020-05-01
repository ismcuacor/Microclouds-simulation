#ifndef SCHEDULERSETTER_CPP
#include "scheduler_setter.cpp"
#endif

#include "ns3/flow-monitor-module.h"
#include "ns3/internet-module.h"
using namespace ns3;
NS_LOG_COMPONENT_DEFINE ("SimpleRenater");

int
main (int argc, char *argv[])
{
	// Log level
	LogComponentEnable ("UdpEchoClientApplication", LOG_LEVEL_INFO);
	LogComponentEnable ("UdpEchoServerApplication", LOG_LEVEL_INFO);

	// Each time there is an interface event, the routing is launched.
	Config::SetDefault ("ns3::Ipv4GlobalRouting::RespondToInterfaceEvents", BooleanValue (true));

 	// Create nodes in a container
