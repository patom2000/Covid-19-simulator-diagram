#include "ns3/applications-module.h"
#include "ns3/core-module.h"
#include "ns3/csma-module.h"
#include "ns3/internet-module.h"
#include "ns3/mobility-module.h"
#include "ns3/netanim-module.h"
#include <stdlib.h>

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
	struct memberAttribute{
		int vaccine[3];
		int mask_type;
	}memberData[50];
	// int nodeSpeed = 20;
    // int nodePause = 0;
    // int64_t streamIndex = 0;
    // ObjectFactory pos;
	
    	member(int amount, int datarate, int delay){
    		/*node.Create(amount);
    		stack.Install(node);
    		devices = csma.Install(node);
			Setip(nethost, netmask);
			Setcsma(datarate, delay);
			Setposition(node);
			Setrecvport(amount);
			Setsentport(amount);*/
			SetNodeData(50);
			
			
    		 
    	};
		void Setcsma(int datarate, int delay){
			//set csma
			csma.SetChannelAttribute ("DataRate", DataRateValue (DataRate (datarate)));
  			csma.SetChannelAttribute ("Delay", TimeValue (MilliSeconds (delay)));
		}

		void Setip(string nethost, string netmask){
			//set IPV4
    		address.SetBase (Ipv4Address(nethost.c_str()), Ipv4Mask(netmask.c_str()));
    		interface = address.Assign(devices);
		}
		
		void Setsentport(int amount){
			OnOffHelper onoff ("ns3::UdpSocketFactory", Address (InetSocketAddress (Ipv4Address ("255.255.255.255"), port)));
			onoff.SetConstantRate (DataRate ("5kb/s"));
  			app.Add (onoff.Install (node.Get (49)));
			Time(1, 20);
		}

		void Time(float start, float stop){
			app.Start (Seconds (start));
			app.Stop (Seconds (stop));
		}

		void Setrecvport(int amount){
			PacketSinkHelper sink ("ns3::UdpSocketFactory",
            Address (InetSocketAddress (Ipv4Address::GetAny (), port)));
			for (int node_id = 0; node_id < amount; node_id++) {
			app = sink.Install (node.Get (node_id));
			}
			Time(1, 20);

		}
		void ReceivePacket (Ptr<Socket> socket)
		{
			while (socket->Recv ())
				{
				NS_LOG_UNCOND ("Received one packet!");
				}
		}
		static void CourseChange (std::string context, Ptr<const MobilityModel> mobility)
		{
		Vector pos = mobility->GetPosition ();
		Vector vel = mobility->GetVelocity ();
		std::cout << Simulator::Now () << ", model=" << mobility << ", POS: x=" << pos.x << ", y=" << pos.y
					<< ", z=" << pos.z << "; VEL:" << vel.x << ", y=" << vel.y
					<< ", z=" << vel.z << std::endl;
		}
		void Move(NodeContainer c, uint32_t node_id){	
		// pos.SetTypeId ("ns3::RandomRectanglePositionAllocator");
        // pos.Set ("X", StringValue ("ns3::UniformRandomVariable[Min=0.0|Max=100.0]"));
        // pos.Set ("Y", StringValue ("ns3::UniformRandomVariable[Min=0.0|Max=100.0]"));

        // Ptr<PositionAllocator> nowPosition = pos.Create ()->GetObject<PositionAllocator> ();
        // streamIndex += nowPosition->AssignStreams (streamIndex);

        // std::stringstream rSpeed;
        // rSpeed << "ns3::UniformRandomVariable[Min=0.0|Max=" << nodeSpeed << "]";
        // std::stringstream rPause;
        // rPause << "ns3::ConstantRandomVariable[Constant=" << nodePause << "]";
        // mobility.SetMobilityModel ("ns3::RandomWaypointMobilityModel",
        //                           "Speed", StringValue (rSpeed.str ()),
        //                           "Pause", StringValue (rPause.str ()),
        //                           "PositionAllocator", PointerValue (nowPosition));
        // mobility.SetPositionAllocator (nowPosition);
        
 
        // mobility.SetPositionAllocator ("ns3::RandomBoxPositionAllocator",
        // "X", StringValue ("ns3::UniformRandomVariable[Min=0|Max=108]"),
        // "Y", StringValue ("ns3::UniformRandomVariable[Min=0|Max=160]"),
        // "Z", StringValue ("ns3::UniformRandomVariable[Min=0|Max=100]"));
        // mobility.Install (c.Get(node_id));
        // streamIndex += mobility.AssignStreams (c, streamIndex);
        // NS_UNUSED (streamIndex);
		}
		void Setposition(NodeContainer c){
			float x = 4;
			float y = 1.35;
  			for (int col = 0; col < 6; col++) {
				 	for (int row = 0; row < 9; row++){
					uint32_t node_id = (col*9) + row;
					
					if(49 <= ((col*9)+row) ){
						Addnode(c, 0.7, 5, node_id);
						break;
					}
					else{
						Addnode(c, x, y, node_id);
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
		void Addnode(NodeContainer c, float x, float y, uint32_t node_id){
			Ptr<ListPositionAllocator> positionAlloc = CreateObject<ListPositionAllocator> ();
  			positionAlloc->Add (Vector (x, y, 0.0));
  			mobility.SetPositionAllocator (positionAlloc);
            mobility.SetMobilityModel ("ns3::GaussMarkovMobilityModel",
            "Bounds", BoxValue (Box (0, 10.8, 0, 16, 0, 10)),
            "TimeStep", TimeValue (Seconds (0.5)),
            "Alpha", DoubleValue (0.85),
            "MeanVelocity", StringValue ("ns3::UniformRandomVariable[Min=1|Max=2]"),
            "MeanDirection", StringValue ("ns3::UniformRandomVariable[Min=0|Max=3]"),
            "MeanPitch", StringValue ("ns3::UniformRandomVariable[Min=0.05|Max=0.05]"),
            "NormalVelocity", StringValue ("ns3::NormalRandomVariable[Mean=0.0|Variance=0.0|Bound=0.0]"),
            "NormalDirection", StringValue ("ns3::NormalRandomVariable[Mean=0.0|Variance=0.2|Bound=0.4]"),
            "NormalPitch", StringValue ("ns3::NormalRandomVariable[Mean=0.0|Variance=0.02|Bound=0.04]"));
  			// mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
  			mobility.Install (c.Get(node_id));
		}
		
		void SetNodeData(int amount){
			for (int node_id = 0; node_id < amount; node_id++) { 
			    int dose = rand()%4; 
				memberData[node_id].mask_type = rand()%3; 
				int vaccine_type = rand()%4;
				for (int each_dose = 0; each_dose < dose; each_dose++) { 
					if(dose == 1){
						if(vaccine_type == 2){
							memberData[node_id].vaccine[each_dose] = vaccine_type + 1;
						}
					}else if(dose == 3){
						if(vaccine_type == 3){
							vaccine_type = rand() % 2;
							memberData[node_id].vaccine[each_dose] = vaccine_type;
						}
					}else{
						memberData[node_id].vaccine[each_dose] = vaccine_type;
					}
				}
			}

		}
		
	uint16_t port = 9;
};


int main(int argc, char *argv[]){
    member(50, 5000000, 2);
	Simulator::Stop (Seconds (20.0));
    AnimationInterface anim ("project_2.xml");
    Simulator::Run ();
    Simulator::Destroy ();
}
