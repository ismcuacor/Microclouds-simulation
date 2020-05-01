#include "SmartNet.cpp"

#include <set>
#include <stdexcept>
#include <stdlib.h>

using namespace ns3;

// All nodes in the system
std::map< Ptr<SmartNet::SmartNode>, int> Nodes;

// Nodes which have lost their microcloud
std::set< Ptr<SmartNet::SmartNode> > OrphanNodes;

std::pair < ns3::Ptr<SmartMigration>, std::set< ns3::Ptr<SmartGraph> > > mergeMicroclouds(ns3::Ptr<SmartGraph> microcloud, std::set< ns3::Ptr<SmartGraph> > microcloudsList);

//static NetDeviceContainer **netdevicestable;

class DEEPACC: public SmartProtocol 
{
   public:
	// Routes an overlay network 
	ns3::Ptr<SmartNet::SmartGraph> Run (ns3::Ptr<SmartNet::SmartGraph> graphIn);
       
        // Routes an overlay network, with the one or more nodes sending information between them 
	ns3::Ptr<SmartNet::SmartGraph> Run (ns3::Ptr<SmartGraph> graphIn, std::set< ns3::Ptr<SmartNode> > talking_nodes);
	
	// Routes an overlay network, with the one or more nodes sending information between them and a predesigned microcloud
	ns3::Ptr<SmartNet::SmartGraph> Run (ns3::Ptr<SmartGraph> graphIn, std::set< ns3::Ptr<SmartNode> > talking_nodes, int mcloud);
	
	// Routes an overlay network, with the one or more nodes sending information between them, and several microclouds
	std::map<int, ns3::Ptr<SmartNet::SmartGraph> > RunSet (ns3::Ptr<SmartGraph> graphIn, std::set< ns3::Ptr<SmartNode> > talking_nodes, std::set<int> mcloud);
	
	//void RunNow (ns3::Ptr<SmartGraph> graphIn, std::set< ns3::Ptr<SmartNode> > talking_nodes, ns3::Ptr<SmartGraph> graph);
	
	// Routes an overlay network, given a list of nodes sending information. Similar to Run, using a different algorithm
	ns3::Ptr<SmartGraph> RunNow (ns3::Ptr<SmartGraph> graphIn, std::set< ns3::Ptr<SmartNode> > talking_nodes);
	
	// Initialize method
	void onStart();

        // A node is added to the network
	void onNewSmartNode (ns3::Ptr<SmartNet::SmartGraph> graphIn, Ptr<SmartNet::SmartNode> node);

        // A node is deleted from the network
	void onDeleteSmartNode (Ptr<SmartNet::SmartNode> node);

        // A link is broken (for example, when a wireless link is stopped)
	bool onBrokenLink(Ptr<SmartNet::SmartNode> node);
	
	// Completes the routing information between 2 nodes
	void getEdges(ns3::Ptr<SmartNet::SmartNode> node1, ns3::Ptr<SmartNet::SmartNode> node2);

   private:
        // Chooses who is going to be hosting the data
	void selectProvider(ns3::Ptr<SmartGraph> graph);
	
	// Chooses who is keeping a copy of the data for resiliency
	void selectRepeater(ns3::Ptr<SmartNode> node);

        // Creates the overlay network
	void buildMicrocloud (std::set< Ptr<SmartNet::SmartGraph> > microclouds, Ptr<SmartNet::SmartMigration> migrations);

        // Adds a new node to a microcloud
	void placeSmartNodeInList (Ptr<SmartNet::SmartNode> node);

        // Remove a node from the Microcloud
	void onDeletedSmartNode();
};

// Keepalive
bool ping (ns3::Ptr<SmartNet::SmartNode> node)
{
	return true;
}
