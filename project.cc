#include "ns3/applications-module.h"
#include "ns3/core-module.h"
#include "ns3/csma-module.h"
#include "ns3/internet-module.h"
#include "ns3/mobility-module.h"
#include "ns3/netanim-module.h"

using namespace ns3;
using namespace std;

class member{
    string nethost = "192.168.1.0";
    string netmask  = "255.255.255.0";
    public:
    	NodeContainer node;
	NetDeviceContainer devices;
	InternetStackHelper stack;
	Ipv4AddressHelper address;
	Ipv4InterfaceContainer interface;
	ApplicationContainer app;
	CsmaHelper csma;
	MobilityHelper mobility;
	
    	member(int amount, int datarate, int delay){
    		node.Create(amount);
    		stack.Install(node);
    		devices = csma.Install(node);
			setip(nethost, netmask);
			setcsma(datarate, delay);
			setposition(node);
			/*setrecvport(amount);
			setsentport(amount);*/
			
    		 
    	};
		void setcsma(int datarate, int delay){
			//set csma
			csma.SetChannelAttribute ("DataRate", DataRateValue (DataRate (datarate)));
  			csma.SetChannelAttribute ("Delay", TimeValue (MilliSeconds (delay)));
		}

		void setip(string nethost, string netmask){
			//set IPV4
    		address.SetBase (Ipv4Address(nethost.c_str()), Ipv4Mask(netmask.c_str()));
    		interface = address.Assign(devices);
		}
		
		void setsentport(int amount){
			OnOffHelper onoff ("ns3::UdpSocketFactory", Address (InetSocketAddress (Ipv4Address ("255.255.255.255"), port)));
			onoff.SetConstantRate (DataRate ("500kb/s"));
			for (int node_id = 0; node_id < amount; node_id++) {
  				app.Add (onoff.Install (node.Get (node_id)));
			}
			time(1, 20);
		}

		void time(float start, float stop){
			app.Start (Seconds (start));
			app.Stop (Seconds (stop));
		}

		void setrecvport(int amount){
			PacketSinkHelper sink ("ns3::UdpSocketFactory",
            Address (InetSocketAddress (Ipv4Address::GetAny (), port)));
			for (int node_id = 0; node_id < amount; node_id++) {
			app = sink.Install (node.Get (node_id));
			}
			time(1, 20);

		}
		static void CourseChange (std::string context, Ptr<const MobilityModel> mobility)
		{
		Vector pos = mobility->GetPosition ();
		Vector vel = mobility->GetVelocity ();
		std::cout << Simulator::Now () << ", model=" << mobility << ", POS: x=" << pos.x << ", y=" << pos.y
					<< ", z=" << pos.z << "; VEL:" << vel.x << ", y=" << vel.y
					<< ", z=" << vel.z << std::endl;
		}
		void move(NodeContainer c, int amount){
			
			/*for (int node_id = 0; node_id < amount; node_id++) {
			app = sink.Install (node.Get (node_id));
			}*/
			mobility.SetPositionAllocator ("ns3::PositionAllocator",
                                 "X", StringValue ("100.0"),
                                 "Y", StringValue ("100.0"),
                                 "Rho", StringValue ("ns3::UniformRandomVariable[Min=0|Max=100]"));
  			mobility.SetMobilityModel ("ns3::RandomWalk2dMobilityModel",
                             "Mode", StringValue ("Time"),
                             "Time", StringValue ("2s"),
                             "Speed", StringValue ("ns3::ConstantRandomVariable[Constant=1.0]"),
                             "Bounds", StringValue ("0|200|0|200"));
  			mobility.Install (c);
			
  			Config::Connect ("/NodeList/*/$ns3::MobilityModel/CourseChange", MakeCallback (&CourseChange));
		
		}
		void setposition(NodeContainer c){
			float x = 4;
			float y = 1.35;
  			for (int col = 0; col < 6; col++) {
				 	for (int row = 0; row < 9; row++){
					uint32_t node_id = (col*9) + row;
					
					if(49 <= ((col*9)+row) ){
						addnode(c, 0.7, 5, node_id);
						break;
					}
					else{
						addnode(c, x, y, node_id);
						if((row+1) % 3 == 0){
							y += 1.15;
						}
						else{
							y += 0.8;
						}
					}
				}
				x += 1.5;
				y = 1.35;
			}
			
  			
		}
		void addnode(NodeContainer c, float x, float y, uint32_t node_id){
			Ptr<ListPositionAllocator> positionAlloc = CreateObject<ListPositionAllocator> ();
  			positionAlloc->Add (Vector (x, y, 0.0));
  			mobility.SetPositionAllocator (positionAlloc);
  			mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
  			mobility.Install (c.Get(node_id));
		}
		void movetorackserver(){

		}

		void movetostayhere(){

		}
	uint16_t port = 9;
};


int main(int argc, char *argv[]){
    member(50, 5000000, 2);
	Simulator::Stop (Seconds (20.0));
    AnimationInterface anim ("project.xml");
    Simulator::Run ();
    Simulator::Destroy ();
}
