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
    	Student(int number, int datarate, int delay){
    		node.Create(number);
    		stack.Install(node);
    		devices = csma.Install(node);
			void setip(nethost, netmask);
			void setcsma(datarate, delay);
			void setrecvport(number);
			void setsentport(number);

    		
    	};
		void Student::setcsma(int datarate, int delay){
			//set csma
			csma.SetChannelAttribute ("DataRate", DataRateValue (DataRate (datarate)));
  			csma.SetChannelAttribute ("Delay", TimeValue (MilliSeconds (delay)));
		}

		void Student::setip(string nethost, string netmask){
			//set IPV4
    		address.SetBase (Ipv4Address(nethost.c_str()), Ipv4Mask(netmask.c_str()));
    		interface = address.Assign(devices);
		}
		
		void Student::setsentport(int amount){
			OnOffHelper onoff ("ns3::UdpSocketFactory", Address (InetSocketAddress (Ipv4Address ("255.255.255.255"), port)));
			onoff.SetConstantRate (DataRate ("500kb/s"));
			for (int node_id = 0; node_id < amount; node_id++) {
  				app.Add (onoff.Install (node.Get (node_id)));)
			}
			setstartstoptime(1, 10);
		}

		void Student::time(float start, float stop){
			app.Start (Seconds (start));
			app.Stop (Seconds (stop));
		}

		void Student::setrecvport(int amount){
			PacketSinkHelper sink ("ns3::UdpSocketFactory",
            Address (InetSocketAddress (Ipv4Address::GetAny (), port)));
			for (int node_id = 0; node_id < amount; node_id++) {
			app = sink.Install (node.Get (node_id));
			}
			setstartstoptime(1, 2);

		}
	uint16_t port = 9;
};


int main(int argc, char *argv[]){
    Student(5, 5000000, 2);
    AnimationInterface anim ("project.xml");
    Simulator::Run ();
    Simulator::Destroy ();
}
