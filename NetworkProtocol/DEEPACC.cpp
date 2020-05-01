#include "DEEPACC.h"

using namespace ns3;

static std::set< ns3::Ptr<SmartNode> > resent_nodes;

/***
* The mergeMicroclouds method is called whenever two microclouds have become too similar, so that the information is merged together into
* the same provider. This is done to avoid having too redundant information which increases energy consumption. The process of merging microclouds
* is decided upon a heuristic, which needs to be decided when called.
**/
std::pair< ns3::Ptr<SmartMigration>, std::set< ns3::Ptr<SmartGraph> > >  mergeMicroclouds(ns3::Ptr<SmartGraph> microcloud, std::set< ns3::Ptr<SmartGraph> > microcloudsList)
{
        ns3::Ptr<SmartMigration> migration, migrationAux;

        ns3::Ptr<SmartGraph> microcloudResult;
        std::set< ns3::Ptr<SmartGraph> > microcloudListResult(microcloudsList);

	// Select which nodes will be migrated
	for (std::set< ns3::Ptr<SmartGraph> >::iterator it = microcloudsList.begin(); it != microcloudsList.end(); ++it)
        {
		migrationAux = microcloud->merge(*it);

		if (migration == NULL || migrationAux != NULL && migrationAux->getEC() < migration->getEC())// && migrationAux->getEC() > 0)
                {
                        migration = migrationAux;
			microcloudResult = *it;
                }
        }

	// To ensure that no error was made
	if (migration == NULL || migration->getEC() == 0 || migration->getEC() == std::numeric_limits<float>::max())
		throw std::invalid_argument( "Generated empty Migration" );	
	
	if (migration != NULL) {
		microcloud->Migrate(migration);
		microcloudsList.insert(microcloud);
        	microcloudsList.erase(microcloudResult);

		std::pair < ns3::Ptr<SmartMigration>, std::set< ns3::Ptr<SmartGraph> > > aux(migration, microcloudsList);

		return aux;
	}

	return std::pair < ns3::Ptr<SmartMigration>, std::set< ns3::Ptr<SmartGraph> > >();
}

void DEEPACC::getEdges(ns3::Ptr<SmartNet::SmartNode> node1, ns3::Ptr<SmartNet::SmartNode> node2)
{
	std::set< ns3::Ptr<SmartNode> > nodesAux = node1->getNeighbors();

	for (std::set< ns3::Ptr<SmartNode> >::iterator it = nodesAux.begin(); it!= nodesAux.end(); ++it)
	{
		node1->Check_Neighbors( *it, node2, 0, "");
	}
}

void DEEPACC::onNewSmartNode (ns3::Ptr<SmartNet::SmartGraph> graphIn, Ptr<SmartNet::SmartNode> node)
{
	if (graphIn == NULL || graphIn->isEmpty() )
	{
		graphIn = new SmartGraph(node);
	} else {
		//For logging purposes
		printf("%d\n", graphIn->getEdges().size());
		ns3::Ptr<SmartMigration> mig = graphIn->merge(node);
		
		//If there is no resulting merge, there was an error
		if (mig->getMigrations().size() == 0)
		         throw std::invalid_argument("NULL MERGE\n");
		graphIn->Migrate(mig);
	}
}

void DEEPACC::onDeleteSmartNode (Ptr<SmartNet::SmartNode> node)
{
	Nodes.erase(node);
	Graph.Delete(node);

	std::set< Ptr<SmartNet::SmartNode> > neighbors = node->getNeighbors();

	for (std::set< Ptr<SmartNet::SmartNode> >::iterator it = neighbors.begin(); it != neighbors.end(); ++it)
	{
		OrphanNodes.insert(*it);
	}

	onDeletedSmartNode();
}

bool DEEPACC::onBrokenLink(Ptr<SmartNet::SmartNode> node)
{
	if (!ping(node))
	{
		onDeleteSmartNode(node);
		return true;
	}

	return false;
}

/***
* Once we have a graph, the process is using a depth search first algorithm to find the most suitable path between each node and the provider
* reducing the number of paths open. To reduce the computation time, we used a heuristic-based trim, so that the minimum threshold is 
* surpassed, we don't keep searching.
**/
void DEEPACC::buildMicrocloud (std::set< ns3::Ptr<SmartNet::SmartGraph> > microclouds, Ptr<SmartNet::SmartMigration> migrations)
{
	pathLenght = std::numeric_limits<float>::max();
 
	if (microclouds.size() <= 1) 
	{
       		throw std::invalid_argument( "Received empty Set" );			
	}

	if (microclouds.size() == 2)
	{
		//Base case. If size == 2 -> merge and finish. 
		//If size == 1, the result is itself so we do nothing

		ns3::Ptr<SmartNet::SmartGraph> graphAux = *(microclouds.begin());
                ns3::Ptr<SmartNet::SmartGraph> graphAux2 = *(++microclouds.begin());

		ns3::Ptr<SmartMigration> mig = graphAux2->merge(graphAux);
		migrations->add(mig);
	} else {
		std::set< Ptr<SmartNet::SmartGraph> >::iterator it = microclouds.begin();
		std::advance(it, rand()%(microclouds.size()));

		//Delete the microcloud from the set and recover the output (the deleted element) to merge
		ns3::Ptr<SmartNet::SmartGraph> element = (*it);
		if( microclouds.erase(element) == 0)
			printf("AQUI ERROR\n");
		//Merge
		std::pair< ns3::Ptr<SmartMigration>, std::set< ns3::Ptr<SmartGraph> > >  merged = mergeMicroclouds(element, microclouds);
	
		//If there is no resulting merge, there was an error
		if (merged.second.size() == 0)
			 throw std::invalid_argument("NULL MERGE\n");

		//Add migrations to the set
		migrations->add(merged.first);

		//Recursive call
		buildMicrocloud(merged.second, migrations);
	}
}

// Route function
ns3::Ptr<SmartNet::SmartGraph> DEEPACC::Run (ns3::Ptr<SmartNet::SmartGraph> graphIn)
{      
	std::set< ns3::Ptr<SmartNet::SmartNode> > graphNodes = graphIn->getNodes();
	
        std::set< ns3::Ptr<SmartNet::SmartGraph> >  microcloudsList;

	ns3::Ptr<SmartNet::SmartGraph> graphResult;

        ns3::Ptr<SmartNet::SmartMigration> migrations = new SmartNet::SmartMigration ();

	Ptr<SmartNet::SmartGraph> graphAux;

        for (std::set< Ptr<SmartNet::SmartNode> >::iterator it = graphNodes.begin(); it != graphNodes.end(); ++it)
        { 
                graphAux =  new SmartNet::SmartGraph(*it);
		microcloudsList.insert(graphAux);
        }

	buildMicrocloud(microcloudsList, migrations);

	graphResult = *(microcloudsList.begin());
	
	graphResult->Migrate(migrations);

	selectProvider(graphResult);

	return graphResult;
}

// Route function
ns3::Ptr<SmartGraph> DEEPACC::RunNow (ns3::Ptr<SmartNet::SmartGraph> graphIn, std::set< ns3::Ptr<SmartNode> > talking_nodes)
{

	printf("%d %d", graphIn->getNodes().size(), talking_nodes.size());	
	std::set< ns3::Ptr<SmartNet::SmartNode> > graphNodesAux = graphIn->getNodes(), graphNodes;
	
	std::set< ns3::Ptr<SmartNet::SmartNode> >::iterator error = graphNodesAux.end(), nodeIterator;


	for (std::set< ns3::Ptr<SmartNode> >::iterator talkingIterator = talking_nodes.begin(); talkingIterator != talking_nodes.end(); ++talkingIterator)
	{
		nodeIterator = graphNodesAux.find(*talkingIterator);
		if (nodeIterator == error)
			throw std::invalid_argument( "Talking node not in graph" );
		graphNodes.insert(*nodeIterator);
	}

	if (talking_nodes.size() != graphNodes.size())
		throw std::invalid_argument( "Error while parsing talking nodes" );	

	graphNodes = talking_nodes;
        
	std::set< ns3::Ptr<SmartNet::SmartGraph> >  microcloudsList;

	ns3::Ptr<SmartNet::SmartGraph> graphResult;

        ns3::Ptr<SmartNet::SmartMigration> migrations = new SmartNet::SmartMigration ();
	Ptr<SmartNet::SmartGraph> graphAux;

        for (std::set< Ptr<SmartNet::SmartNode> >::iterator it = graphNodes.begin(); it != graphNodes.end(); ++it)
        { 
                graphAux =  new SmartNet::SmartGraph(*it);
		microcloudsList.insert(graphAux);
        }

	if (microcloudsList.size() != 1)
		buildMicrocloud(microcloudsList, migrations);

	graphResult = *(microcloudsList.begin());
	
	graphResult->Migrate(migrations);

	used_nodes = graphResult->getNodes();

	selectProvider(graphResult);

	//graph = graphResult;

	return graphResult;
}

// Route function
ns3::Ptr<SmartNet::SmartGraph> DEEPACC::Run (ns3::Ptr<SmartNet::SmartGraph> graphIn, std::set< ns3::Ptr<SmartNode> > talking_nodes)
{
	std::set< ns3::Ptr<SmartNet::SmartNode> > graphNodesAux = graphIn->getNodes(), graphNodes;
	std::set< ns3::Ptr<SmartNet::SmartNode> >::iterator error = graphNodesAux.end(), nodeIterator;

	graphNodes = talking_nodes;
        
	std::set< ns3::Ptr<SmartNet::SmartGraph> >  microcloudsList;
	ns3::Ptr<SmartNet::SmartGraph> graphResult;

        ns3::Ptr<SmartNet::SmartMigration> migrations = new SmartNet::SmartMigration ();
	Ptr<SmartNet::SmartGraph> graphAux;

        for (std::set< Ptr<SmartNet::SmartNode> >::iterator it = graphNodes.begin(); it != graphNodes.end(); ++it)
        { 
                graphAux =  new SmartNet::SmartGraph(*it);
		microcloudsList.insert(graphAux);
        }

	if (microcloudsList.size() != 1)
		buildMicrocloud(microcloudsList, migrations);

	graphResult = *(microcloudsList.begin());
	
	graphResult->Migrate(migrations);

	used_nodes = graphResult->getNodes();

	selectProvider(graphResult);

	return graphResult;
}

// Route function
ns3::Ptr<SmartNet::SmartGraph> DEEPACC::Run (ns3::Ptr<SmartNet::SmartGraph> graphIn, std::set< ns3::Ptr<SmartNode> > talking_nodes, int mcloud)
{
	std::set< ns3::Ptr<SmartNet::SmartNode> > graphNodesAux;
	std::set< ns3::Ptr<SmartNet::SmartGraph> >  microcloudsList;
	ns3::Ptr<SmartNet::SmartGraph> graphResult = new SmartGraph(), graphAux;
        ns3::Ptr<SmartNet::SmartMigration> migrations = new SmartNet::SmartMigration ();
	int mc;

	microcloudsList.clear();

        for (std::set< Ptr<SmartNet::SmartNode> >::iterator it2 = talking_nodes.begin(); it2 != talking_nodes.end(); ++it2)
        {
		mc = (*it2)->getMCloud ();
		if (mc == mcloud)
		{
                	graphAux =  new SmartNet::SmartGraph(*it2);
			microcloudsList.insert(graphAux);
		}
        }

	if (microcloudsList.size() > 1)
	{
		buildMicrocloud(microcloudsList, migrations);
		graphResult =  *(microcloudsList.begin());
		graphResult->Migrate(migrations);
		graphNodesAux = graphResult->getNodes();

		used_nodes.insert(graphNodesAux.begin(), graphNodesAux.end());

		selectProvider(graphResult);
	} 
	else if (microcloudsList.size() == 1) 
	{
		graphResult =  *(microcloudsList.begin());
		graphNodesAux = graphResult->getNodes();
		used_nodes.insert(graphNodesAux.begin(), graphNodesAux.end());
                selectProvider(graphResult);
	}

	return graphResult;
}

//To Improve
void DEEPACC::placeSmartNodeInList (ns3::Ptr<SmartNet::SmartNode> node)
{
	std::map< ns3::Ptr<SmartNet::SmartNode>, int> SmartNodesCopy = Nodes;
	SmartNet::SmartGraph microAux(node);

	SmartMigration migration;

	while (!SmartNodesCopy.empty())
	{
		migration.add(Graph.mergeMicroclouds(SmartNodesCopy));
		SmartNodesCopy.erase(migration.getLastSmartNode());
	}

	SmartNodes.insert(std::pair< Ptr<SmartNet::SmartNode> ,int> (node, microAux.getEC()));
	std::map< Ptr<SmartNet::SmartNode>,int>::iterator it = Nodes.begin();

	if(it->first == node)
		migration.Migrate();
}

void DEEPACC::onDeletedSmartNode()
{
	for (std::set< Ptr<SmartNet::SmartNode> >::iterator it = OrphanNodes.begin(); it != OrphanNodes.end(); ++it )
	{
		onNewSmartNode(*it);
	}
}

void DEEPACC::selectProvider(ns3::Ptr<SmartGraph> graph)
{
	//TODO Implement smart provider choosing
	std::set< ns3::Ptr<SmartNode> > graphNodes = graph->getNodes();

	std::set< ns3::Ptr<SmartNode> >::iterator it = graphNodes.begin();
	
	advance(it, rand()%graphNodes.size());

	ns3::Ptr<SmartProvider> prov = new SmartProvider((*it)->getNode());
	graph->setProvider(prov);

	std::set< ns3::Ptr<SmartNode> > neig = prov->getNeighbors();
	for (std::set< ns3::Ptr<SmartNode> >::iterator itNeig = neig.begin(); itNeig != neig.end(); ++itNeig)
		selectRepeater(*itNeig);
}

void DEEPACC::selectRepeater (ns3::Ptr<SmartNode> node)
{
	resent_nodes.insert(node);

	std::set< ns3::Ptr<SmartNode> > neig = node->getNeighbors();
        for (std::set< ns3::Ptr<SmartNode> >::iterator itNeig = neig.begin(); itNeig != neig.end(); ++itNeig)
	{
		if(resent_nodes.find(*itNeig) == resent_nodes.end())
			node->addRepeat(*itNeig);
                	selectRepeater(*itNeig);
	}
}
