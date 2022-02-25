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
	// int nodeSpeed = 20;
    // int nodePause = 0;
    // int64_t streamIndex = 0;
    // ObjectFactory pos;
	
    	member(int amount, int datarate, int delay){
    		node.Create(amount);
    		stack.Install(node);
    		devices = csma.Install(node);
			setip(nethost, netmask);
			setcsma(datarate, delay);
			setposition(node);
			// setrecvport(amount);
			// setsentport(amount);
			
			
    		 
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
			onoff.SetConstantRate (DataRate ("5kb/s"));
  			app.Add (onoff.Install (node.Get (49)));
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
		void move(NodeContainer c, uint32_t node_id){	
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
			// Ptr<ListPositionAllocator> positionAlloc = CreateObject<ListPositionAllocator> ();
  			// positionAlloc->Add (Vector (x, y, 0.0));
  			// mobility.SetPositionAllocator (positionAlloc);
            // mobility.SetMobilityModel ("ns3::GaussMarkovMobilityModel",
            // "Bounds", BoxValue (Box (0, 16, 0, 10.8, 0, 10)),
            // "TimeStep", TimeValue (Seconds (0.5)),
            // "Alpha", DoubleValue (0.85),
            // "MeanVelocity", StringValue ("ns3::UniformRandomVariable[Min=1|Max=2]"),
            // "MeanDirection", StringValue ("ns3::UniformRandomVariable[Min=0|Max=3]"),
            // "MeanPitch", StringValue ("ns3::UniformRandomVariable[Min=0.05|Max=0.05]"),
            // "NormalVelocity", StringValue ("ns3::NormalRandomVariable[Mean=0.0|Variance=0.0|Bound=0.0]"),
            // "NormalDirection", StringValue ("ns3::NormalRandomVariable[Mean=0.0|Variance=0.2|Bound=0.4]"),
            // "NormalPitch", StringValue ("ns3::NormalRandomVariable[Mean=0.0|Variance=0.02|Bound=0.04]"));
			mobility.SetMobilityModel ("ns3::WaypointMobilityModel");
			mobility.Install (c.Get(node_id));
  			Ptr<WaypointMobilityModel> wayMobility;

  			wayMobility = c.Get(node_id)->GetObject<WaypointMobilityModel>();

  			Waypoint waypointStart(Seconds(0), Vector3D(x, y, 0));
  			Waypoint waypointEnd(Seconds(4.9), Vector3D(x-0.5, y+1, 0));

  			wayMobility->AddWaypoint(waypointStart);
  			wayMobility->AddWaypoint(waypointEnd);

  			Waypoint waypointStart1(Seconds(6), Vector3D(x-0.5, y+1, 0));
  			Waypoint waypointEnd1(Seconds(9.9), Vector3D(10, 8, 0));

		  	wayMobility->AddWaypoint(waypointStart1);
  			wayMobility->AddWaypoint(waypointEnd1);

			Waypoint waypointStart2(Seconds(10), Vector3D(10, 8, 0));
  			Waypoint waypointEnd2(Seconds(15), Vector3D(x, y, 0));

		  	wayMobility->AddWaypoint(waypointStart2);
  			wayMobility->AddWaypoint(waypointEnd2);
			
			
   			//Nodes.Get(numberOfnodes)->AggregateObject(nodesWaypointMobility);
  			// mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
  			
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
    AnimationInterface anim ("project_2.xml");
    for (uint32_t i = 0; i < 50; i++)
    {
      anim.UpdateNodeSize(i, 0.2, 0.2);
    }
    
    Simulator::Run ();
    Simulator::Destroy ();
}
