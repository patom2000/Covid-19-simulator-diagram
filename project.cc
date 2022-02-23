#include "ns3/applications-module.h"
#include "ns3/core-module.h"
#include "ns3/csma-module.h"
#include "ns3/internet-module.h"
#include "ns3/mobility-module.h"
#include "ns3/netanim-module.h"

using namespace ns3;
using namespace std;

class Student{
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
    	Student(int number, int datarate, int delay){
    		node.Create(number);
    		stack.Install(node);
    		devices = csma.Install(node);
			setip(nethost, netmask);
			setcsma(datarate, delay);
			setrecvport(number);
			setsentport(number);
			move(node);
    		 
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
		void move(NodeContainer c){
			mobility.SetPositionAllocator ("ns3::RandomDiscPositionAllocator",
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
	uint16_t port = 9;
};


int main(int argc, char *argv[]){
    Student(5, 5000000, 2);
	Simulator::Stop (Seconds (20.0));
    AnimationInterface anim ("project.xml");
    Simulator::Run ();
    Simulator::Destroy ();
}
