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
	CsmaHelper csma;
    	Student(int number){
    		node.Create(number);
    		stack.Install(node);
    		devices = csma.Install(node);
			//set IPV4
    		address.SetBase (Ipv4Address(nethost.c_str()), Ipv4Mask(netmask.c_str()));
    		interface = address.Assign(devices);
			//set csma
			csma.SetChannelAttribute ("DataRate", DataRateValue (DataRate (5000000)));
  			csma.SetChannelAttribute ("Delay", TimeValue (MilliSeconds (2)));
			//set onoff
			OnOffHelper onoff ("ns3::UdpSocketFactory", Address (InetSocketAddress (Ipv4Address ("255.255.255.255"), port)));
			onoff.SetConstantRate (DataRate ("500kb/s"));
			ApplicationContainer app = onoff.Install (node.Get (0));
			app.Add (onoff.Install (node.Get (1)));
			// Start the application
			app.Start (Seconds (1.0));
			app.Stop (Seconds (10.0));

    		// Create an optional packet sink to receive these packets
  			PacketSinkHelper sink ("ns3::UdpSocketFactory",
            Address (InetSocketAddress (Ipv4Address::GetAny (), port)));
			app = sink.Install (node.Get (2));
			for (int i = 3; i < 5; i++)
			{
				app.Add (sink.Install (node.Get (i)));
			}
			app.Start (Seconds (1.0));
  			app.Stop (Seconds (10.0));
    	};
	uint16_t port = 9;
};


int main(int argc, char *argv[]){
    Student(5);
    AnimationInterface anim ("project.xml");
    Simulator::Run ();
    Simulator::Destroy ();
}
