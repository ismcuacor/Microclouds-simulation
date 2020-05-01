#include "ns3/core-module.h"
#include "SmartNet.h"

#include <unistd.h>
#include <time.h>
#include <map>

namespace SmartNet
{
	class SmartManager: public SmartNode
	{
		ns3::Ptr<SmartProvider> provider;
		ns3::Ptr<SmartProtocol> managementProtocol;
		ns3::Ptr<SmartProtocol> synchronizationProtocol;
                ns3::Ptr<SmartProtocol> frameControlProtocol;

		public:
			SmartManager() {}

			~SmartManager() 
			{
			}

			void SetManagementProtocol (ns3::Ptr<SmartProtocol> protocol) 
			{ 
				managementProtocol = protocol;
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
                                return managementProtocol;
                        }       
                        
                        ns3::Ptr<SmartProtocol> GetSynchronizationProtocol ()
                        {
                                return synchronizationProtocol;
                        }       
                        
                        ns3::Ptr<SmartProtocol> GetFrameControl ()
                        {
                                return frameControlProtocol;
                        } 

                       	void RunManagementProtocol ()
                        { 
                                managementProtocol->Run();
                        }       
                        
                        void RunSynchronizationProtocol ()
                        {
                                synchronizationProtocol->Run();
                        }       
                                
                        void RunFrameControl ()
                        {
                                frameControlProtocol->Run();
                        }

			void UpdateCloud()
			{
				ns3::list< ns3::Ptr<Packet> > pktList = RequestProviderInformation(provider);
				for (ns3::list< ns3::Ptr<Packet> >::iterator it = pktList.begin(); it != pktList.end(); ++it)
					SendToUpperNode(*it);
			}

		 	ns3::list< ns3::Ptr<Packet> > RequestProviderInformation (ns3::Ptr<SmartProvider> provider)
			{
				ns3::list< ns3::Ptr<Packet> > pckList, pckListAux;

				pckListAux = provider->GetCachedInformation();

				for (ns3::list< ns3::Ptr<Packet> >::iterator it = pktListAux.begin(); it != pktListAux.end(); ++it)
					pckList.push_back(*it);

				return pckList;
			}

			void SendToUpperNode( Ptr<Packet> pkt )
			{
				//TODO
			}

	};

        class SmartProvider: public SmartNode
        {
		ns3::Ptr<SmartNode> node;
		ns3::list< Ptr<Packet> > pckList;

		public:
			SmartProvider(){}
			~SmartProvider(){}

			void RequestHoldBlock()
			{
				//TODO
			}

			ns3::list< ns3::Ptr<Packet> > GetCachedInformation()
			{
				return pckList;
			}

			void StorePacket ()
			{
				pckList.push_back(this->ReceivePacket());
			}

			void FreeBlock()
			{
				//TODO
			}

	};

        class SmartRepeater: public SmartNode
	{
		public:
			void Multicast (ns3::Packet pck)
			{
				//TODO
			}
	};
}
