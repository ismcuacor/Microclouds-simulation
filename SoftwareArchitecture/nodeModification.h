#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/csma-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"
#include "ns3/ecofen-module.h"
#include "ns3/application.h"
#include "ns3/olsr-routing-protocol.h"

class NodeModification: public ns3::Application
{
	ns3::Ptr<SmartNode> node;
	ns3::Ptr<SmartManager> manager;
	float seconds;
	public:
		NodeModification(ns3::Ptr<SmartNode> n, ns3::Ptr<SmartManager> m, float seconds)
                {
                        node = n;
                        manager = m;
			this->seconds = seconds;
                }
	
		~NodeModification()
                {
                }

	protected:
	        virtual void StartApplication (void){
			manager->RunAdditionProtocol (node);
			manager->Switch_On(m_startTime.GetSeconds());
			ns3::Ipv4GlobalRoutingHelper::RecomputeRoutingTables ();
	        }
	
	        virtual void StopApplication (void)
	        {
	        }
};
