#include "SmartNet.cpp"

#include <set>
#include <stdexcept>
#include <stdlib.h>

using namespace ns3;

std::map< Ptr<SmartNet::SmartNode>, int> Nodes;
std::set< Ptr<SmartNet::SmartNode> > OrphanNodes;

std::pair < ns3::Ptr<SmartMigration>, std::set< ns3::Ptr<SmartGraph> > > mergeMicroclouds(ns3::Ptr<SmartGraph> microcloud, std::set< ns3::Ptr<SmartGraph> > microcloudsList);

//static NetDeviceContainer **netdevicestable;

class DEEPACC: public SmartProtocol 
{
   public:
	// Inherited	
	ns3::Ptr<SmartNet::SmartGraph> Run (ns3::Ptr<SmartNet::SmartGraph> graphIn);

	ns3::Ptr<SmartNet::SmartGraph> Run (ns3::Ptr<SmartGraph> graphIn, std::set< ns3::Ptr<SmartNode> > talking_nodes);
	
	ns3::Ptr<SmartNet::SmartGraph> Run (ns3::Ptr<SmartGraph> graphIn, std::set< ns3::Ptr<SmartNode> > talking_nodes, int mcloud);
	std::map<int, ns3::Ptr<SmartNet::SmartGraph> > RunSet (ns3::Ptr<SmartGraph> graphIn, std::set< ns3::Ptr<SmartNode> > talking_nodes, std::set<int> mcloud);
	
	//void RunNow (ns3::Ptr<SmartGraph> graphIn, std::set< ns3::Ptr<SmartNode> > talking_nodes, ns3::Ptr<SmartGraph> graph);
	ns3::Ptr<SmartGraph> RunNow (ns3::Ptr<SmartGraph> graphIn, std::set< ns3::Ptr<SmartNode> > talking_nodes);
	
	void onStart();

	void onNewSmartNode (ns3::Ptr<SmartNet::SmartGraph> graphIn, Ptr<SmartNet::SmartNode> node);

	void onDeleteSmartNode (Ptr<SmartNet::SmartNode> node);

	bool onBrokenLink(Ptr<SmartNet::SmartNode> node);
	
	void getEdges(ns3::Ptr<SmartNet::SmartNode> node1, ns3::Ptr<SmartNet::SmartNode> node2);

   private:
	void selectProvider(ns3::Ptr<SmartGraph> graph);
	
	void selectRepeater(ns3::Ptr<SmartNode> node);

	void buildMicrocloud (std::set< Ptr<SmartNet::SmartGraph> > microclouds, Ptr<SmartNet::SmartMigration> migrations);

	void placeSmartNodeInList (Ptr<SmartNet::SmartNode> node);

	void onDeletedSmartNode();
};

bool ping (ns3::Ptr<SmartNet::SmartNode> node)
{
	return true;
}
