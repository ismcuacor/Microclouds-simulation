#include "ns3/core-module.h"
#include <ns3/system-thread.h>
#include <ns3/tag.h>

#include <unistd.h>
#include <time.h>
#include <map>
#include <stdexcept>

//int rows;
//int columns;
ns3::Ipv4InterfaceContainer * interfacestable [140][140] = {};

namespace SmartNet
{
class SmartNode;

ns3::Ptr<SmartNode> prov;
std::map<int, ns3::Ptr<SmartNode> > prov2;

	class DestinationTag : public ns3::Tag
	{
		public:
		  static ns3::TypeId GetTypeId (void);
		  virtual ns3::TypeId GetInstanceTypeId (void) const;
		  virtual uint32_t GetSerializedSize (void) const;
		  virtual void Serialize (ns3::TagBuffer i) const;
		  virtual void Deserialize (ns3::TagBuffer i);
		  virtual void Print (std::ostream &os) const;
		
		  // these are our accessors to our tag structure
		  void SetSimpleValue (uint8_t value);
		  uint8_t GetSimpleValue (void) const;
		private:
		  uint8_t m_simpleValue;
	};

	class DistanceTag : public ns3::Tag
	{
		public:
		  static ns3::TypeId GetTypeId (void);
		  virtual ns3::TypeId GetInstanceTypeId (void) const;
		  virtual uint32_t GetSerializedSize (void) const;
		  virtual void Serialize (ns3::TagBuffer i) const;
		  virtual void Deserialize (ns3::TagBuffer i);
		  virtual void Print (std::ostream &os) const;
		
		  // these are our accessors to our tag structure
		  void SetSimpleValue (uint8_t value);
		  uint8_t GetSimpleValue (void) const;
		private:
		  uint8_t m_simpleValue;
	};

	class LongTag : public ns3::Tag
	{
		public:
		  static ns3::TypeId GetTypeId (void);
		  virtual ns3::TypeId GetInstanceTypeId (void) const;
		  virtual uint32_t GetSerializedSize (void) const;
		  virtual void Serialize (ns3::TagBuffer i) const;
		  virtual void Deserialize (ns3::TagBuffer i);
		  virtual void Print (std::ostream &os) const;
		
		  // these are our accessors to our tag structure
		  void SetSimpleValue (uint8_t value);
		  uint8_t GetSimpleValue (void) const;
		private:
		  uint8_t m_simpleValue;
	};

        class SmartPath;

	//class SmartNode: public ns3::Node, public ns3::Application
	class SmartNode: public ns3::Application
	{
		public:
			int mod = -1;
			bool fixed;
			std::map< int, std::set<int> > paths;
			std::map< ns3::Ptr<SmartNode>, std::set< ns3::Ptr<SmartNode> > > pathsFinal;
			bool visited; //= false;
        		int count;
			int pos;
                	std::map< ns3::Ptr<SmartNode>, std::pair<ns3::Ptr<ns3::Socket>,  ns3::InetSocketAddress * > > Neighbors;
                	std::map< ns3::Ptr<SmartNode>, std::pair<ns3::Ptr<ns3::Socket>,  ns3::InetSocketAddress * > > Neighbors2;
       			SmartNode()
        		      : m_peer(),
        		      m_packetSize (0),
        		      m_nPackets (0),
        		      m_dataRate (0),
        		      m_sendEvent (),
        		      m_running (false),
        		      m_packetsSent (0),
			      buffer (0),
			      mcloud (-1)
        		{ 
				count = 0;
  				pos = 0;
				visited = false;
				fixed = false;
			}

		        ~SmartNode()
		        { }

			SmartNode(ns3::Ptr<ns3::Node> nodeAux, int mc)
			      : m_peer(),
                              m_packetSize (0),
			      m_maxPacketSize(1500),
                              m_nPackets (0),
                              m_dataRate (0),
                              m_sendEvent (),
                              m_running (false),
                              m_packetsSent (0)
			{
			        mcloud = mc;
				count = 0;
 				pos = 0;
				node = nodeAux;
				visited = false;
				fixed = false;
			}

			SmartNode(ns3::Ptr<ns3::Node> nodeAux)
			      : m_peer(),
                              m_packetSize (0),
			      m_maxPacketSize(1500),
                              m_nPackets (0),
                              m_dataRate (0),
                              m_sendEvent (),
                              m_running (false),
                              m_packetsSent (0),
			      mcloud (-1)
			{
				count = 0;
 				pos = 0;
				node = nodeAux;
				visited = false;
				fixed = false;
			}

			void setP2P (bool b) 
			{
				fixed = b;
			}
			
			bool isP2P () 
			{
				return fixed;
			}

			void Send_Neighbor()
			{
				std::set<ns3::Ptr <SmartNode> > Neighbors2 = getNeighbors();
				for (std::set< ns3::Ptr<SmartNode> >::iterator it2 = Neighbors2.begin(); it2 != Neighbors2.end(); *it2++)
				{
					ns3::Ptr<SmartNode> neigh = *it2;
					//Send_Packet_Now(neigh, ns3::Create<ns3::Packet>(), address);
				}
			}

			void addNeighbor (ns3::Ptr<SmartNode> node) 
			{
				std::pair< ns3::Ptr<ns3::Socket>,  ns3::InetSocketAddress * > socketConf;
				Neighbors.insert(std::pair < ns3::Ptr<SmartNode>, std::pair< ns3::Ptr<ns3::Socket>,  ns3::InetSocketAddress * > > (node, socketConf ));
			}

			void delNeighbor (ns3::Ptr<SmartNode> node) 
			{
				Neighbors.erase(node);
			}
			
			void addRepeat (ns3::Ptr<SmartNode> node); 

			std::map <ns3::Ptr<SmartNode>, std::set< ns3::Ptr<SmartNode> > > getActivePaths ()
			{
				return pathsFinal;
			}
 
			std::set< ns3::Ptr<SmartNode> > getNeighbors () 
			{
				std::set< ns3::Ptr<SmartNode> > result;
				
				for (std::map< ns3::Ptr<SmartNode>, std::pair< ns3::Ptr<ns3::Socket>,  ns3::InetSocketAddress * > >::iterator it = Neighbors.begin(); it != Neighbors.end(); *it++)
					result.insert(it->first);
				return result;
			}

			void setPort (int newPort)
			{
				port = newPort;
			}

			int getPort ()
			{
				port++;
				return port;
			}

			bool isProvider () 
			{ 
				return provider; 
			}

			bool wasVisited() { return visited; }

			void setVisited( bool vis ) { visited = vis; }

			ns3::Ptr<SmartNode> clone()
			{
				//PARENT CLONE??
				ns3::Ptr<SmartNode> nodeClone = new SmartNode();
				//nodeClone->setProvider( this->provider );
				//To avoid redundancy, neighbors should be added afterwards from the Graph
				return nodeClone;
			}

			ns3::Ptr<SmartNode> copy()
			{
				ns3::Ptr<SmartNode> copy (this);
				return copy;
			}
		
			double getStartTime()
			{
				return m_startTime.GetSeconds();
			}
	
		//	void setProvider (bool prov) { provider = prov; }
		
			void setBaseConsumption (float BC) { baseConsumption = BC; }
	
			float getBaseConsumption () { return baseConsumption; }

			void setNode (ns3::Ptr<ns3::Node> n)
			{node = n;}

			ns3::Ptr<ns3::Node> getNode()
			{
				return node;
			}


			// Proper from transport
			void Setup (ns3::Ipv4Address address, std::list<float> aTimes, std::list<int> aSizes){}
			void SetupNow (ns3::Ptr<SmartNode> smNode, ns3::Ipv4Address address);
                        void SetupNow2 (ns3::Ipv4Address address);

			std::pair<ns3::Ptr<ns3::Socket>, ns3::InetSocketAddress * > getSocketConfiguration (ns3::Ptr<SmartNode> destNode)
			{
				std::map< ns3::Ptr<SmartNode>, std::pair< ns3::Ptr<ns3::Socket>, ns3::InetSocketAddress * > >::iterator socketIt = Neighbors.find(destNode);
				if(socketIt == Neighbors.end())
                                        printf("Error retrieving Socket \n");

				return socketIt->second;
			}

			void setSocket (ns3::Ptr<SmartNode> destNode, std::pair< ns3::Ptr<ns3::Socket>, ns3::InetSocketAddress * > socket)
			{
				Neighbors2[destNode] = socket;
			}

			void Send_Packet_Now2 (ns3::Ptr<SmartNode> smNode, ns3::Ptr<ns3::Packet> pck,  ns3::Ipv4Address address)
                        {
				if (Neighbors2.find(smNode) != Neighbors2.end())
				{
                               		ns3::Ptr<ns3::Socket> m_socket = Neighbors2.find(smNode)->second.first;
			       		
                               		if (!m_socket)
                               		{
                               		        std::cout << " NO SOCKET 2 " << smNode->getNode()->GetId() << std::endl;
                               		        exit(-1);
                               		}

                               		int result = m_socket->Send (pck);

                               		if (result == -1)// && m_socket -> GetErrno() != ns3::Socket::ERROR_NOTERROR)
                               		{
						m_socket->Connect(address);
						result = m_socket->Send (pck);
						if (result == -1)// && m_socket -> GetErrno() != ns3::Socket::ERROR_NOTERROR)
                        			{
                               		                if (m_socket -> GetErrno() == 3)
                               		                        std::cout << "Error in size: " << pck -> GetSize() << std::endl;
                               		                if (m_socket -> GetErrno() == ns3::Socket::ERROR_NOTERROR)
                               		                        std::cout << "Not an error " << std::endl;
                               		                else if (m_socket -> GetErrno() == ns3::Socket::ERROR_BADF)
                               		                        std::cout << "Error in file descriptor: "<< std::endl;
                               		                else if (m_socket -> GetErrno() == ns3::Socket::ERROR_NOROUTETOHOST)
                               		                        std::cout << "Error in route to host from " << getNode()->GetId() << " to " << prov2.find(getMCloud())->second->getNode()->GetId()<< std::endl;
                               		                else if (m_socket -> GetErrno() == ns3::Socket::ERROR_NOTCONN)
                               		                        std::cout << " Host "<<getNode()->GetId()<<" not connected "<< " to " << smNode->getNode()->GetId() << std::endl;
                               		                else if (m_socket -> GetErrno() == ns3::Socket::ERROR_SHUTDOWN )
                               		                        std::cout << " Host shutdown "<< std::endl;
                               		                else if (m_socket -> GetErrno() == ns3::Socket::ERROR_OPNOTSUPP)
                               		                        std::cout << " Operation not supported "<< std::endl;
                               		                else
                               		                        std::cout << "Error in package sending: " << m_socket->GetErrno() << std::endl;
                               		                exit(-1);
						}
                               		}
				}
                        }


		        void Send_Packet_Now (ns3::Ptr<SmartNode> smNode, ns3::Ptr<ns3::Packet> pck,  ns3::Ipv4Address address)
			{
				if (!m_socket)
					std::cout << " NO SOCKET " << std::endl;
		
				int result = m_socket->Send (pck);

               			if (result == -1)// && m_socket -> GetErrno() != ns3::Socket::ERROR_NOTERROR)
               			{
						if (m_socket -> GetErrno() == 3)
                                			std::cout << "Error in size: " << pck -> GetSize() << std::endl;
               					if (m_socket -> GetErrno() == ns3::Socket::ERROR_NOTERROR)
							std::cout << "Not an error " << std::endl;
						else if (m_socket -> GetErrno() == ns3::Socket::ERROR_BADF)
							std::cout << "Error in file descriptor: "<< std::endl;
						else if (m_socket -> GetErrno() == ns3::Socket::ERROR_NOROUTETOHOST)
                                        	        std::cout << "Error in route to host from " << getNode()->GetId() << " to " << prov2.find(getMCloud())->second->getNode()->GetId()<< std::endl;
						else if (m_socket -> GetErrno() == ns3::Socket::ERROR_NOTCONN)
                               		                std::cout << " Host "<<getNode()->GetId()<<" not connected "<< " to " << prov2.find(getMCloud())->second->getNode()->GetId() << std::endl;
						else if (m_socket -> GetErrno() == ns3::Socket::ERROR_SHUTDOWN )
							std::cout << " Host shutdown "<< std::endl;
						else if (m_socket -> GetErrno() == ns3::Socket::ERROR_OPNOTSUPP)
							std::cout << " Operation not supported "<< std::endl;
						else
							std::cout << "Error in package sending: " << m_socket->GetErrno() << std::endl;
						exit(-1);
				}
			}

		        void Send_Packet (ns3::Ptr<SmartNode> nodeDest, ns3::Ptr<ns3::Packet> pkt )
        		{
				ns3::Ptr<ns3::Socket> m_socket = getSocketConfiguration (nodeDest).first;
				ns3::Ptr<ns3::Socket> r_socket = nodeDest->getSocketConfiguration(this).first;

				ns3::InetSocketAddress * m_address = getSocketConfiguration (nodeDest).second;
				ns3::InetSocketAddress * r_address = nodeDest->getSocketConfiguration (this).second;
				ns3::InetSocketAddress r_local = ns3::InetSocketAddress (ns3::Ipv4Address::GetAny (), r_address->GetPort());
				
				ns3::Ptr<ns3::Node> node = this->node;
				r_socket->SetRecvCallback ( ns3::MakeCallback(&SmartNode::ReceivePacket, nodeDest));
	
				LongTag tag;
				tag.SetSimpleValue(0xFF);
			
				ns3::Ptr<ns3::Packet> pck = pkt;
				pck->RemoveAllPacketTags ();
  				pck->RemoveAllByteTags ();
				
				pck->AddPacketTag(tag);
			
				count++;
                		if(m_socket == NULL)
                        		printf("Error creating Socket \n");
                		else{
                        		if (r_socket == NULL)
                        		{
                                		printf("Peer missed\n");
                                		exit(-1);
                        		}
					
               				int result = m_socket->Send (pck);

               				if (result == -1 && m_socket -> GetErrno() != ns3::Socket::ERROR_NOTERROR)
               				{
						m_socket->Connect(*m_address);
						result = m_socket->Send (pck);
						if (result == -1 && m_socket -> GetErrno() != ns3::Socket::ERROR_NOTERROR)
                        			{
							if (m_socket -> GetErrno() == 3)
                        	        			std::cout << "Error in size: " << pkt -> GetSize() << std::endl;
               						else if (m_socket -> GetErrno() == ns3::Socket::ERROR_BADF)
								std::cout << "Error in file descriptor: "<< std::endl;
							else if (m_socket -> GetErrno() == ns3::Socket::ERROR_NOROUTETOHOST)
                        	                	        std::cout << "Error in route to host "<< std::endl;
							else if (m_socket -> GetErrno() == ns3::Socket::ERROR_NOTCONN)
                        	                	{        
								std::cout << " Host not connected "<< std::endl;
							} else
								std::cout << "Error in package sending: " << m_socket->GetErrno() << std::endl;
						}
						else {
							printf("PACKET SENT THE SECOND TIME\n");
						}
					} else {
						printf("PACKET SENT\n");
					}
				}
        		}

			void ReceivePacket( ns3::Ptr<ns3::Socket> );
			void ReceivePacketA( ns3::Ptr<ns3::Socket> socket );	

			int findSmartNodeRef ();
	
			ns3::Ptr<SmartPath> Find_Path (ns3::Ptr<SmartNode> destNode, bool root, std::set< ns3::Ptr<SmartNode> > visited, float lenght);

			void Check_Neighbors(ns3::Ptr<SmartNode> nextNode, ns3::Ptr<SmartNode> destNode, int lenght, char* buffer);

			//void Schedule_Send();
			void Schedule_Send (std::list<float> times, std::list<int> sizes);
        		void Schedule_SendA (ns3::Ptr<SmartNode> destNode)
        		{
                		std::list<float>::iterator itTime = times.begin();
                		std::list<int>::iterator itSize = sizes.begin();

                		for (int i = 0; i < (times.size() - 1) ; i++)
                		{
                        		itTime++;
                       		 	itSize++;

                        		ns3::Ptr<ns3::Packet> pkt = ns3::Create<ns3::Packet> (*itSize);
                        		//ns3::Simulator::Schedule(ns3::Seconds (*itTime), &SmartNode::Send_Packet, this, destNode, pkt);
                		}
		//		ns3::Simulator::Schedule(ns3::Seconds (1), &SmartNode::ReceivePacket, this);
        		}

			void newPath (char* path);
		
			int getMCloud () { return mcloud; }
			void setMCloud (int i) { mcloud = i; }
		protected:
			int mcloud;
			int port;
		        ns3::Ptr<ns3::Node> node;
                	bool provider;
                	float baseConsumption;
			std::set< ns3::Ptr<SmartNode> > repeat;
		        // Functions from tranport
        		virtual void StartApplication (void){
            			port = 4477;
				m_running = true;
            			m_packetsSent = 0;
				//ns3::Simulator::Schedule(ns3::Seconds(1.0), &SmartNode::Send_Packet, this, *getNeighbors().begin(), ns3::Create<ns3::Packet>());
        		}

        		virtual void StopApplication (void)
        		{
            			m_running = false;

            			if (m_sendEvent.IsRunning ())
              			{
                			ns3::Simulator::Cancel (m_sendEvent);
              			}

				for (std::map< ns3::Ptr<SmartNode>, std::pair< ns3::Ptr<ns3::Socket>,  ns3::InetSocketAddress * > >::iterator it = Neighbors.begin(); it != Neighbors.end(); *it++)
				{
					ns3::Ptr<ns3::Socket> m_socket = it->second.first;
            				if (m_socket)
              				{
                				m_socket->Close ();
              				}
				}
        		}

			ns3::Ptr<ns3::Socket>            m_socket;
    			ns3::Address                     m_peer;
    			uint32_t                         m_packetSize;
    			uint32_t                         m_maxPacketSize;
    			uint32_t                    	 m_nPackets;
    			ns3::DataRate                 	 m_dataRate;
    			ns3::EventId                     m_sendEvent;
    			bool                             m_running;
    			uint32_t                         m_packetsSent;
    			std::list<float>                 times;
    			std::list<int>                   sizes;
    			int                              nodeId;
			int                              nodeDestId;
			int 				 buffer;
	};

        class SmartProvider: public SmartNode
        {
                std::list< ns3::Ptr<ns3::Packet> > pckList;

                public:
                        SmartProvider(){}
                        SmartProvider(ns3::Ptr<ns3::Node> node): SmartNode(node) { }
			SmartProvider(ns3::Ptr<SmartNode> node): SmartNode(node->getNode()){}
                        ~SmartProvider()
                        {
                        //      delete &pckList;
                        }
			
			// Proper from transport
			void Setup (ns3::Ipv4Address address, std::list<float> aTimes, std::list<int> aSizes);
			
			void RequestHoldBlock()
                        {
                                //TODO
                        }

                        std::list< ns3::Ptr<ns3::Packet> > GetCachedInformation()
                        {
                                return pckList;
                        }

                        void ReceivePacket ( ns3::Ptr<ns3::Socket> socket);
			void ReceivePacketA(ns3::Ptr<ns3::Socket>);
                        
			void FreeBlock()
                        {
                                //TODO
                        }
        };

//TODO
	enum Speed {};

	enum State {ON, OFF, STAND_BY};

	class EdgeConfiguration: public ns3::Object
	{
		static const float LENGTHCABLECONSUMPTION = 1; 

		float extraEC; // AMPL, REP...

		Speed frequency;
		std::map<Speed, float> frequencyConsumption;
 
		State state;
                std::map<State, float> stateConsumption;

		float lengthCable;
		float lengthCableConsumption; // To be included in a function
		
		float delay;	// Average time of buffering of info
		float maxDelay;
		
		int maxPacketSize;
		int headers; // Added info to the packet

		int capacity; // Average number of packets per time unit
		
		public:
			int maxPackets ()
			{
				round(frequency/maxPacketSize);
			}

			float maxInformation ()
			{
				round(frequency/ (capacity*(maxPacketSize - headers)));
			}

			float getEC()
			{
				//TODO
			}
	
                float getExtraEC () { return extraEC; }
		void setExtraEC (float EC) { extraEC = EC; }

                Speed getFrequency () { return frequency; }
		void setFrequency (Speed freq) { frequency = freq; }

                std::map<Speed, float> getFrequencyConsumption() { return frequencyConsumption; }
		void setFrequencyConsumption (std::map<Speed, float> freqCon) { frequencyConsumption = freqCon; }

                State getState() { return state; }
		void setState (State stat) { state = stat; }

                std::map<State, float> getStateConsumption () { return stateConsumption; }
		void setStateConsumption (std::map<State, float> sc) { stateConsumption = sc; }

                float getLengthCable () { return lengthCable; }
		void setLengthCable (float lc) { lengthCable = lc; }

                float getLengthCableConsumption () { return lengthCableConsumption; }
		void setLengthCableConsumption () { lengthCableConsumption = lengthCable*LENGTHCABLECONSUMPTION; }

		float getDelay () { return delay; }
		void setDelay (float d) { delay = d; }

                float getMaxDelay () {return maxDelay; }
		void setMaxDelay (float d) { delay = d; }

                int getMaxPacketSize () { return maxPacketSize; }
		void setMaxPacketSize (int size) { maxPacketSize = size; }

                int getHeaders () { return headers; }
		void setHeaders (int h) { headers = h; }

                int getCapacity() { return capacity; }
		void setCapacity (int cap ) { capacity = cap; }

	};

	struct EdgeStruct
	{
		float consumption;// = 0;
                ns3::Ptr<EdgeConfiguration> configuration;
                ns3::Ptr<SmartNode> node1;
                ns3::Ptr<SmartNode> node2;
		ns3::Ipv4InterfaceContainer ipv4;
	};

	class SmartEdge: public ns3::Object
	{
		EdgeStruct structure;

		public:
			SmartEdge(){
				ns3::Ptr<SmartNode> node1 = NULL;
                                ns3::Ptr<SmartNode> node2 = NULL;

                                structure.consumption = 0;
                                structure.node1 = node1;
                                structure.node2 = node2;
			}

			~SmartEdge()
			{
				//delete &structure;
			}
	
                        SmartEdge(ns3::Ptr<ns3::Node> nodeAux1, ns3::Ptr<ns3::Node> nodeAux2, ns3::Ptr<EdgeConfiguration> configuration, ns3::Ipv4InterfaceContainer ipv4)
                        {
				ns3::Ptr<SmartNode> node1 = new SmartNode(nodeAux1);
                                ns3::Ptr<SmartNode> node2 = new SmartNode(nodeAux2);

                                structure.consumption = 0;
                                structure.configuration = configuration;
                                structure.node1 = node1;
                                structure.node2 = node2;
				structure.ipv4 = ipv4;
				node1->addNeighbor(node2);
				node2->addNeighbor(node1);
                        }
		
                        SmartEdge(ns3::Ptr<ns3::Node> nodeAux1, ns3::Ptr<ns3::Node> nodeAux2, ns3::Ptr<EdgeConfiguration> configuration)
                        {
				ns3::Ptr<SmartNode> node1 = new SmartNode(nodeAux1);
                                ns3::Ptr<SmartNode> node2 = new SmartNode(nodeAux2);

                                structure.consumption = 0;
                                structure.configuration = configuration;
                                structure.node1 = node1;
                                structure.node2 = node2;
				node1->addNeighbor(node2);
				node2->addNeighbor(node1);
                        }
		
			SmartEdge(ns3::Ptr<SmartNode> node1, ns3::Ptr<SmartNode> node2, ns3::Ptr<EdgeConfiguration> configuration, float cons)
			{
				structure.consumption = cons;
				structure.configuration = configuration;

				structure.node1 = node1;
				structure.node2 = node2;

                                node1->addNeighbor(node2);
                                node2->addNeighbor(node1);
			}
			SmartEdge(ns3::Ptr<SmartNode> node1, ns3::Ptr<SmartNode> node2, ns3::Ptr<EdgeConfiguration> configuration, float cons, ns3::Ipv4InterfaceContainer ipv4)
			{
				structure.consumption = cons;
				structure.configuration = configuration;

				structure.node1 = node1;
				structure.node2 = node2;

                                node1->addNeighbor(node2);
                                node2->addNeighbor(node1);
				structure.ipv4 = ipv4;
			}

			SmartEdge(ns3::Ptr<SmartNode> node1, ns3::Ptr<SmartNode> node2)
			{
				structure.consumption = 0;
				structure.configuration = new EdgeConfiguration();
				structure.node1 = node1;
				structure.node2 = node2;
			}

			SmartEdge(ns3::Ptr<SmartNode> node)
			{
				structure.consumption = 0;
				//structure.configuration = OFF;
				structure.node1 = node;
				structure.node2 = NULL;
			}

			EdgeStruct getStructure() { return structure; }

			ns3::Ptr<SmartNode> getNode1(){ return structure.node1; }

                        ns3::Ptr<SmartNode> getNode2(){ return structure.node2; }

			float getConsumption() {
				float aux = 1; 

				/*
				TODO FIX THIS
				if (structure.consumption != NULL )
				{
					aux = structure.consumption; 
				}
				*/
		
				return aux; 
			}
			
			ns3::Ptr<SmartEdge> clone()
			{
				ns3::Ptr<SmartEdge> edge = new SmartEdge();
				EdgeStruct structureAux;
				ns3::Ptr<EdgeConfiguration> configurationAux;
	
				structureAux = this->getStructure();
				configurationAux = structureAux.configuration;
				structureAux.configuration = configurationAux;
				structureAux.node1 = structureAux.node1->clone();
                                structureAux.node2 = structureAux.node2->clone();

				edge->setStructure ( &structureAux );
				return edge;
			}

			ns3::Ptr<SmartEdge> copy()
			{
                                ns3::Ptr<SmartEdge> copy (this);
                                return copy;
			}

			void setStructure ( EdgeStruct * structure )
			{
				this->structure = *structure;
			}
	};
        class SmartPath: public ns3::Object
        {
                public:
                        SmartPath ()
                        {}

			~SmartPath ()
                        {
			}

                        SmartPath(ns3::Ptr<SmartNode> node, ns3::Ptr<SmartNode> node2, ns3::Ptr<SmartPath> pathAux);
                        
			SmartPath(std::set< ns3::Ptr<SmartEdge> > edge)
			{
				for (std::set< ns3::Ptr<SmartEdge> >::iterator it = edge.begin(); it != edge.end(); ++it)
					path.insert(*it);
			}
                        
			SmartPath(ns3::Ptr<SmartEdge> edge)
			{
				path.insert(edge);
			}
			
			void addEdge(ns3::Ptr<SmartEdge> edge)
			{
				if (edge)
					path.insert(edge);
			}
 
			std::set< ns3::Ptr<SmartEdge> > getPath () { return path; }

			std::set< ns3::Ptr<SmartNode> > getNodes() 
			{
				std::set< ns3::Ptr<SmartNode> > result;

				for (std::set< ns3::Ptr<SmartEdge> >::iterator itPath = path.begin(); itPath != path.end(); ++itPath)
				{
					result.insert((*itPath)->getStructure().node1);
					result.insert((*itPath)->getStructure().node2);
				}

				return result;
			}       

			void insertPath (std::set < ns3::Ptr<SmartEdge> > edges)
			{
				path.insert(edges.begin(), edges.end());
			}
 
			float getEC ()
			{	
				float aux = 0;
				for (std::set< ns3::Ptr<SmartEdge> >::iterator pathIt = path.begin(); pathIt != path.end(); ++pathIt)
					aux += (float) (*pathIt)->getConsumption();
				return aux;
			}

			std::set< ns3::Ptr<SmartEdge> > getEdges () { return path; }
 
                private:
                        std::set< ns3::Ptr<SmartEdge> > path;
        };

        enum EnumMigration {ADD, REMOVE};

	class SmartMigration: public ns3::Object
        {
		std::map<  ns3::Ptr<SmartEdge>, EnumMigration > migrations;
		float EC;

                public:
			SmartMigration() { EC = 0;}

			~SmartMigration()
			{
			//	delete &migrations;
			}

			SmartMigration ( ns3::Ptr<SmartEdge> mig, EnumMigration migType)
			{
				migrations.insert(std::pair<  ns3::Ptr<SmartEdge>, EnumMigration >(mig, migType));
				//TODO ADD Migration costs
				EC = mig->getConsumption();
			}

			SmartMigration ( ns3::Ptr<SmartEdge> mig)
                        {
                                migrations.insert(std::pair<  ns3::Ptr<SmartEdge>, EnumMigration >(mig, ADD));
                                //TODO ADD Migration costs
                                EC = mig->getConsumption();
                        }

			void add( ns3::Ptr<SmartMigration> migration ) {
				if (migration == NULL)
					throw std::invalid_argument( " No migration passed " );	
			
				std::map<  ns3::Ptr<SmartEdge>, EnumMigration > mig = migration->getMigrations();

				mig.erase(NULL);
				if (!mig.empty())
				{
			        	for (std::map<  ns3::Ptr<SmartEdge>, EnumMigration >::iterator it = mig.begin(); it != mig.end(); ++it) {
						this->migrations.insert( *it );
					}
				}
			}

                        float getEC() 
			{
				return EC;
			}

                        void setEC(float value)
			{ 
				EC = value; 
			}

			void copy (ns3::Ptr<SmartMigration> migration)
			{
				//Copies migrations IN the existent map
				std::map<  ns3::Ptr<SmartEdge>, EnumMigration > copyMap = migration->getMigrations();
				for (std::map<  ns3::Ptr<SmartEdge>, EnumMigration >::iterator it = copyMap.begin(); it != copyMap.end(); ++it)
			        	this->migrations.insert(*it);
			}

			std::map<  ns3::Ptr<SmartEdge>, EnumMigration > getMigrations() 
			{ 
				return migrations; 
			}

			SmartNode getLastSmartNode()
			{
				//TODO
			}
        };
	
	class SmartProtocol;

	class SmartGraph: public ns3::Object
	{
		ns3::Ptr< SmartProvider > provider;
		std::set< ns3::Ptr<SmartEdge> > edges;
		std::set< ns3::Ptr<SmartNode> > talking_nodes;
		public:
	
			SmartGraph () 
			{
			}

                        SmartGraph(ns3::Ptr<SmartProvider> node)
                        {
                                ns3::Ptr<SmartEdge> edgeAux = new SmartEdge(node);
                                edges.insert(edgeAux);

                                provider = node;
                        }

                        SmartGraph(ns3::Ptr<SmartNode> node)
                        {
                                ns3::Ptr<SmartEdge> edgeAux = new SmartEdge(node);
                                edges.insert(edgeAux);

                                provider = new SmartProvider(node);
                        }


			SmartGraph(std::set< ns3::Ptr<SmartEdge> > edgesList)
			{
				edges = edgesList;
			}

			~SmartGraph() 
			{
			//	delete &edges;
			}

			void addNeighbor (ns3::Ptr<SmartNode> sm1, ns3::Ptr<SmartNode> sm2, ns3::Ptr<SmartNode> sm3)
			{
				//sm1->Check_Neighbors(sm2, sm3, 1.0);
			}
	
			bool isEmpty()
			{
				return edges.empty();
			}

			std::set<int> getMCloud()
			{
				std::set< ns3::Ptr <SmartNode> > nodes = getNodes();
				std::set<int> result;

				for (std::set< ns3::Ptr <SmartNode> >::iterator it = nodes.begin(); it != nodes.end(); ++it)
				{
					int i = (*it)->getMCloud();
					if (i != -1)
						result.insert(i);
				}
			}

                        float getEC()
                        {
				float EC = 0;
                                for (std::set< ns3::Ptr<SmartEdge> >::iterator it = edges.begin(); it != edges.end(); ++it)
				{
					EC += (*it)->getConsumption();
				}
                                return EC;
                        }

			ns3::Ptr<SmartProvider> getProvider() { return provider; }
			void setProvider(ns3::Ptr<SmartProvider> node) { provider = node; }
			void setTalkingNodes(std::set< ns3::Ptr<SmartNode> > t_n) { talking_nodes = t_n; }
			void Delete(ns3::Ptr<SmartNode>);
			//ns3::Ptr<SmartGraph> Delete(ns3::Ptr<SmartNode>, ns3::Ptr<SmartProtocol>);

			std::set< ns3::Ptr<SmartNode> > getNodes()
			{
				std::set< ns3::Ptr<SmartNode> > nodesListAux;
				EdgeStruct e;

				for (std::set< ns3::Ptr<SmartEdge> >::iterator it = edges.begin(); it != edges.end(); ++it)
				{
					e = (*it)->getStructure();
					nodesListAux.insert(e.node1);
					nodesListAux.insert(e.node2);
				}

				nodesListAux.erase(NULL);

				return nodesListAux;
			}
 
			std::set< ns3::Ptr<SmartEdge> > getEdges() { return edges; }

			void setEdges(std::set< ns3::Ptr<SmartEdge> > edg) {  edges = edg; }

			ns3::Ptr<SmartEdge> getEdge(ns3::Ptr<SmartNode> n1, ns3::Ptr<SmartNode> n2)
			{
				ns3::Ptr<SmartEdge> edgeResult;
				if(this->edges.size() == 0)
					throw std::invalid_argument( " Invalid graph " );
				for (std::set< ns3::Ptr<SmartEdge> >::iterator it = this->edges.begin(); it != this->edges.end(); ++it)
				{
					if ((*it)->getNode2() == n1 && (*it)->getNode1() == n2 || (*it)->getNode1() == n1 && (*it)->getNode2() == n2)
					{
						edgeResult = *it;
						break;
					} 
				}

				return edgeResult;
			}

			bool hasEdge(ns3::Ptr<SmartNode> n1, ns3::Ptr<SmartNode> n2)
                        {
				bool hasedge = false;

                                if(this->edges.size() == 0)
                                        throw std::invalid_argument( " Invalid graph " );

                                for (std::set< ns3::Ptr<SmartEdge> >::iterator it = this->edges.begin(); it != this->edges.end(); ++it)
                                {
                                        if ((*it)->getNode2() == n1 && (*it)->getNode1() == n2 || (*it)->getNode1() == n1 && (*it)->getNode2() == n2)
                                        {
                                                hasedge = true;
                                                break;
                                        }
                                }

                                return hasedge;
                        }

			ns3::Ipv4Address getIpv4_0(ns3::Ptr<SmartNode> n1, ns3::Ptr<SmartNode> n2)
			{
				ns3::Ipv4Address ipv4Address;
				ns3::Ptr<SmartEdge> edgeResult;
				
				edgeResult = getEdge(n1, n2);

				if(!edgeResult)
					throw std::invalid_argument( " Cannot find neighbour " );
			
				ns3::Ipv4InterfaceContainer ipv4 = edgeResult->getStructure().ipv4;
				
				if ( edgeResult->getNode1() == n2 )
				{
					ipv4Address = ipv4.GetAddress(0 ,0);
				}
				else
				{
					if ( edgeResult->getNode1() == n1 )
					{
						ipv4Address = ipv4.GetAddress(1, 0);
					}
					else 
						throw std::invalid_argument( " Neighbour not in input " );
				}
				return ipv4Address;
			}

			ns3::Ipv4Address getIpv4_1(ns3::Ptr<SmartNode> n1, ns3::Ptr<SmartNode> n2)
			{
				ns3::Ipv4Address ipv4Address;
				ns3::Ptr<SmartEdge> edgeResult;

                                for (std::set< ns3::Ptr<SmartEdge> >::iterator it = this->edges.begin(); it != this->edges.end(); ++it)
                                {

                                        if ((*it)->getNode2() == n1 && (*it)->getNode1() == n2 || (*it)->getNode1() == n1 && (*it)->getNode2() == n2)
                                        {
                                                edgeResult = *it;
                                        }
                                }

				if ( edgeResult->getNode2() == n1 )
					ipv4Address = edgeResult->getStructure().ipv4.GetAddress(0 ,0);
				if ( edgeResult->getNode2() == n2 )
					ipv4Address = edgeResult->getStructure().ipv4.GetAddress(1 ,0);

				return ipv4Address;
			}

			std::set< ns3::Ptr<SmartNode> > getNeighbors(ns3::Ptr<SmartNode> n1)
                        {
                                std::set< ns3::Ptr<SmartNode> > result;

                                for (std::set< ns3::Ptr<SmartEdge> >::iterator it = this->edges.begin(); it != this->edges.end(); ++it)
                                {

                                        if ((*it)->getNode2() == n1 && (*it)->getNode1() != n1 && (*it)->getNode1() != NULL)
                                        {
                                                result.insert((*it)->getNode1());
                                        }
                                        
					if ((*it)->getNode1() == n1 && (*it)->getNode2() != n1 && (*it)->getNode2() != NULL)
                                        {
                                                result.insert((*it)->getNode2());
                                        }
                                }
                                return result;
                        }
			
			void findPath (ns3::Ptr<SmartNode> n1, ns3::Ptr<SmartNode> n2);
			void findPath (ns3::Ptr<SmartNode> n1, ns3::Ptr<SmartNode> n2, int edges,int nodes);

			void addEdge(ns3::Ptr<SmartEdge> edge) { edges.insert(edge); }
			void removeEdge(ns3::Ptr<SmartEdge> edge) { edges.erase(edge); }

			ns3::Ptr<SmartMigration> merge(ns3::Ptr<SmartNet::SmartNode>);
			ns3::Ptr<SmartMigration> merge (ns3::Ptr<SmartProvider> center, ns3::Ptr<SmartNode> node);
	                ns3::Ptr<SmartMigration> merge (ns3::Ptr<SmartGraph> microcloud);

			ns3::Ptr< SmartGraph > copy()
			{
                                ns3::Ptr<SmartGraph> copy  = new SmartGraph();
                                copy->setEdges(std::set< ns3::Ptr<SmartEdge> > (this->getEdges()));
				return copy;
 			}

			ns3::Ptr<SmartGraph> clone()
			{
			        //ns3::Ptr<SmartGraph> SmartGraphClone = new SmartGraph();
				//TODO ns3::Ptr<SmartProvider> nodeClone = this->getProvider()->clone();
        			//SmartGraphClone->setProvider( nodeClone );

			        //for (std::set< ns3::Ptr<SmartEdge> >::iterator it = this->edges.begin(); it != this->edges.end(); ++it) {
			        //        ns3::Ptr< SmartEdge > edge = (*it)->clone();
			        //        SmartGraphClone->addEdge(edge);
			        //}

        			//return SmartGraphClone;
			}
		
			void Migrate ( ns3::Ptr<SmartMigration>  migration );
	};

	class SmartProtocol: public ns3::Object
	{
   		public:
			SmartProtocol(){}
			~SmartProtocol(){}

        		virtual ns3::Ptr<SmartGraph> Run (ns3::Ptr<SmartGraph> graph) 
			{}
			virtual ns3::Ptr<SmartGraph> Run (ns3::Ptr<SmartGraph> graphIn, std::set< ns3::Ptr<SmartNode> > talking_nodes)
			{}
			virtual ns3::Ptr<SmartGraph> Run (ns3::Ptr<SmartGraph> graphIn, std::set< ns3::Ptr<SmartNode> > talking_nodes, int i)
			{}
			//virtual void RunNow (ns3::Ptr<SmartGraph> graphIn, std::set< ns3::Ptr<SmartNode> > talking_nodes,ns3::Ptr<SmartGraph> graph)
			virtual ns3::Ptr<SmartGraph> RunNow (ns3::Ptr<SmartGraph> graphIn, std::set< ns3::Ptr<SmartNode> > talking_nodes)
			{}

			virtual void onNewSmartNode (ns3::Ptr<SmartGraph> graphIn, ns3::Ptr<SmartNet::SmartNode> node){}
			virtual void onDeleteSmartNode (ns3::Ptr<SmartNet::SmartNode> node){}
		private:
	};

	class SmartManager: public SmartNode
	{
		ns3::Ptr<SmartProvider> provider;
		ns3::Ptr<SmartProtocol> managementProtocol;
		ns3::Ptr<SmartProtocol> synchronizationProtocol;
                ns3::Ptr<SmartProtocol> frameControlProtocol;
		ns3::Ptr<SmartGraph> graph;		
		std::map<int, ns3::Ptr<SmartGraph> > graphs;
		ns3::Ptr<SmartProtocol> protocol;
		std::map<int, ns3::Ptr<SmartManager> > managers;

		public:
			SmartManager(ns3::Ptr<SmartGraph> graphIn, ns3::Ptr<SmartProtocol> prot); 

			SmartManager(ns3::Ptr<SmartGraph> graphIn, ns3::Ptr<SmartProtocol> prot, std::set< ns3::Ptr<SmartNode> > talking_nodes);
			SmartManager(ns3::Ptr<SmartGraph> graphIn, ns3::Ptr<SmartProtocol> prot, std::set< ns3::Ptr<SmartNode> > talking_nodes, std::set<int>);
			SmartManager(ns3::Ptr<SmartGraph> graphIn, ns3::Ptr<SmartProtocol> prot, std::set< ns3::Ptr<SmartNode> > talking_nodes, int);

			~SmartManager() 
			{
			}

			std::set<ns3::Ptr<SmartNode> >  GetActiveNodes ();
	
			ns3::Ptr<SmartProvider> GetProvider()
			{
				return graph->getProvider();
			}

			void SetManagementProtocol (ns3::Ptr<SmartProtocol> protocol) 
			{ 
				this->protocol = protocol;
			}

                        void SetSynchronizationProtocol (ns3::Ptr<SmartProtocol> protocol)
                        {
                                synchronizationProtocol = protocol;
                        }

			void SetFrameControl (ns3::Ptr<SmartProtocol> protocol)
			{
                                frameControlProtocol = protocol;
                        }     

			ns3::Ptr<SmartProtocol> GetManagementProtocol ()
                        { 
                                return protocol;
                        }       
                        
                        ns3::Ptr<SmartProtocol> GetSynchronizationProtocol ()
                        {
                                return synchronizationProtocol;
                        }       
                        
                        ns3::Ptr<SmartProtocol> GetFrameControl ()
                        {
                                return frameControlProtocol;
                        }
			
			std::map<int, ns3::Ptr<SmartManager> > getManagers()
			{
				return managers;
			}

			void setManagers (std::map<int, ns3::Ptr<SmartManager> > man)
			{
				managers = man;
			}

                       	void RunAdditionProtocol (ns3::Ptr<SmartNode> node);

			void RunDeleteProtocol (ns3::Ptr<SmartNode> node)
                        { 
                                protocol->onDeleteSmartNode(node);
                        }
                        
                        void RunSynchronizationProtocol ()
                        {
                                //TODO synchronizationProtocol->Run();
                        }       
                                
                        void RunFrameControl ()
                        {
                                //TODO frameControlProtocol->Run();
                        }

			void UpdateCloud()
			{
				std::list< ns3::Ptr<ns3::Packet> > pktList = RequestProviderInformation(provider);
				for (std::list< ns3::Ptr<ns3::Packet> >::iterator it = pktList.begin(); it != pktList.end(); ++it)
					SendToUpperNode(*it);
			}

		 	std::list< ns3::Ptr<ns3::Packet> > RequestProviderInformation (ns3::Ptr<SmartProvider> provider)
			{
				std::list< ns3::Ptr<ns3::Packet> > pckList, pckListAux;

				pckListAux = provider->GetCachedInformation();

				for (std::list< ns3::Ptr<ns3::Packet> >::iterator it = pckListAux.begin(); it != pckListAux.end(); ++it)
					pckList.push_back(*it);

				return pckList;
			}

			void SendToUpperNode( ns3::Ptr<ns3::Packet> pkt )
			{
				printf("Sending");
				//TODO
			}

		
			void setGraph(ns3::Ptr<SmartGraph> graphIn)
			{
				graph = graphIn;
			}

			void setGraphs(std::map<int, ns3::Ptr<SmartGraph> > graphIn)
			{
				graphs = graphIn;
			}
			
		//private:
                        void Switch_On(float seconds);
                        void Switch_On();
                        void RecomputeTables();
	};
}
