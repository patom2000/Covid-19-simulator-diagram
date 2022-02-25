#include "ns3/applications-module.h"
#include "ns3/core-module.h"
#include "ns3/csma-module.h"
#include "ns3/internet-module.h"
#include "ns3/mobility-module.h"
#include "ns3/netanim-module.h"

using namespace ns3;
using namespace std;

class member
{
  string nethost = "192.168.1.0";
  string netmask = "255.255.255.0";

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

  member (int amount, int datarate, int delay)
  {
    node.Create (amount);
    stack.Install (node);
    devices = csma.Install (node);
    setip (nethost, netmask);
    setcsma (datarate, delay);
    setposition (node);
    // setrecvport(amount);
    // setsentport(amount);
  };
  void
  setcsma (int datarate, int delay)
  {
    // set csma
    csma.SetChannelAttribute ("DataRate", DataRateValue (DataRate (datarate)));
    csma.SetChannelAttribute ("Delay", TimeValue (MilliSeconds (delay)));
  }

  void
  setip (string nethost, string netmask)
  {
    // set IPV4
    address.SetBase (Ipv4Address (nethost.c_str ()), Ipv4Mask (netmask.c_str ()));
    interface = address.Assign (devices);
  }

  void
  setsentport (int amount)
  {
    OnOffHelper onoff ("ns3::UdpSocketFactory",
                       Address (InetSocketAddress (Ipv4Address ("255.255.255.255"), port)));
    onoff.SetConstantRate (DataRate ("5kb/s"));
    app.Add (onoff.Install (node.Get (49)));
    time (1, 20);
  }

  void
  time (float start, float stop)
  {
    app.Start (Seconds (start));
    app.Stop (Seconds (stop));
  }

  void
  setrecvport (int amount)
  {
    PacketSinkHelper sink ("ns3::UdpSocketFactory",
                           Address (InetSocketAddress (Ipv4Address::GetAny (), port)));
    for (int node_id = 0; node_id < amount; node_id++)
      {
        app = sink.Install (node.Get (node_id));
      }
    time (1, 20);
  }
  static void
  CourseChange (std::string context, Ptr<const MobilityModel> mobility)
  {
    Vector pos = mobility->GetPosition ();
    Vector vel = mobility->GetVelocity ();
    std::cout << Simulator::Now () << ", model=" << mobility << ", POS: x=" << pos.x
              << ", y=" << pos.y << ", z=" << pos.z << "; VEL:" << vel.x << ", y=" << vel.y
              << ", z=" << vel.z << std::endl;
  }
  void
  setposition (NodeContainer c)
  {
    float x = 4;
    float y = 1.35;
    for (int col = 0; col < 6; col++)
      {
        for (int row = 0; row < 9; row++)
          {
            uint32_t node_id = (col * 9) + row;

            if (49 <= ((col * 9) + row))
              {
                addnode (c, 0.7, 5, node_id);
                break;
              }
            else
              {
                addnode (c, x, y, node_id);
                if ((row + 1) % 3 == 0)
                  {
                    y += 1.15;
                  }
                else
                  {
                    y += 0.8;
                  }
              }
          }
        x += 1.5;
        y = 1.35;
      }
  }
  void
  addnode (NodeContainer c, float x, float y, uint32_t node_id)
  {
    mobility.SetMobilityModel ("ns3::WaypointMobilityModel");
    mobility.Install (c.Get (node_id));
    movetorackserver (c, node_id, x, y);
  }
  void
  movetorackserver (NodeContainer c, uint32_t node_id, float x, float y)
  {
    Ptr<WaypointMobilityModel> wayMobility;
    wayMobility = c.Get (node_id)->GetObject<WaypointMobilityModel> ();
	//row1
    if (node_id % 9 == 0)
      {
        move (wayMobility, 0.0, 4.9, x, y, x, 0.2);
        move (wayMobility, 6, 9.9, x, 0.2, 13.7, 0.2);
        move (wayMobility, 10, 15, 13.7, 0.2, 13.7, 1);
      }
	//row2
    else if (node_id % 9 == 1)
      {
        move (wayMobility, 0.0, 4.9, x, y, x, 0.2);
        move (wayMobility, 6, 9.9, x, 0.2, 13.7, 0.2);
        move (wayMobility, 10, 15, 13.7, 0.2, 13.7, 1);
      }
	//row3
    else if (node_id % 9 == 2)
      {
        move (wayMobility, 0.0, 4.9, x, y, x, 3.7);
        move (wayMobility, 6, 9.9, x, 3.7, 13.7, 3.7);
        move (wayMobility, 10, 15, 13.7, 3.7, 13.7, 2.4);
      }
	//row4
    else if (node_id % 9 == 3)
      {
        move (wayMobility, 0.0, 4.9, x, y, x, 3.7);
        move (wayMobility, 6, 9.9, x, 3.7, 13.7, 3.7);
        move (wayMobility, 10, 15, 13.7, 3.7, 13.7, 3.8);
      }
	//row5
    else if (node_id % 9 == 4)
      {
        move (wayMobility, 0.0, 4.9, x, y, x, 3.7);
        move (wayMobility, 6, 9.9, x, 3.7, 13.7, 3.7);
        move (wayMobility, 10, 15, 13.7, 3.7, 13.7, 3.8);
      }
	//row6
    else if (node_id % 9 == 5)
      {
        move (wayMobility, 0.0, 4.9, x, y, x, 6.8);
        move (wayMobility, 6, 9.9, x, 6.8, 13.7, 6.8);
        move (wayMobility, 10, 15, 13.7, 6.8, 13.7, 5.2);
      }
	//row7
    else if (node_id % 9 == 6)
      {
        move (wayMobility, 0.0, 4.9, x, y, x, 6.8);
        move (wayMobility, 6, 9.9, x, 6.8, 13.7, 6.8);
        move (wayMobility, 10, 15, 13.7, 6.8, 13.7, 7.6);
      }
	//row8
    else if (node_id % 9 == 7)
      {
        move (wayMobility, 0.0, 4.9, x, y, x, 6.8);
        move (wayMobility, 6, 9.9, x, 6.8, 13.7, 6.8);
        move (wayMobility, 10, 15, 13.7, 6.8, 13.7, 7.6);
      }
	//row9
    else if (node_id % 9 == 8)
      {
        move (wayMobility, 0.0, 4.9, x, y, x, 9.7);
        move (wayMobility, 6, 9.9, x, 9.7, 13.7, 9.7);
        move (wayMobility, 10, 15, 13.7, 9.7, 13.7, 8);
      }
  }
  void
  move (Ptr<WaypointMobilityModel> wayMobility, double time_str, double time_end, float x_str,
        float y_str, float x_end, float y_end)
  {
    Waypoint waypointStart (Seconds (time_str), Vector3D (x_str, y_str, 0));
    Waypoint waypointEnd (Seconds (time_end), Vector3D (x_end, y_end, 0));

    wayMobility->AddWaypoint (waypointStart);
    wayMobility->AddWaypoint (waypointEnd);
  }

  void
  movetostayhere ()
  {
  }
  uint16_t port = 9;
};

int
main (int argc, char *argv[])
{
  member (50, 5000000, 2);
  Simulator::Stop (Seconds (20.0));
  AnimationInterface anim ("project_2.xml");
  for (uint32_t i = 0; i < 50; i++)
    {
      anim.UpdateNodeSize (i, 0.2, 0.2);
    }

  Simulator::Run ();
  Simulator::Destroy ();
}
