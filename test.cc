/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"
#include "ns3/netanim-module.h"

// Default Network Topology
//
//       10.1.1.0
// n0 -------------- n1
//    point-to-point
//
 
using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("FirstScriptExample");

int
main (int argc, char *argv[])
{
  CommandLine cmd (__FILE__);
  cmd.Parse (argc, argv);
  
  Time::SetResolution (Time::NS);
  LogComponentEnable ("UdpEchoClientApplication", LOG_LEVEL_INFO);
  LogComponentEnable ("UdpEchoServerApplication", LOG_LEVEL_INFO);

  NodeContainer nodes;
  nodes.Create (5);

  // PointToPointHelper pointToPoint;
  // pointToPoint.SetDeviceAttribute ("DataRate", StringValue ("5Mbps"));
  // pointToPoint.SetChannelAttribute ("Delay", StringValue ("2ms"));

  // NetDeviceContainer devices;
  // devices = pointToPoint.Install (nodes);

  InternetStackHelper stack;
  stack.Install (nodes);

  Ipv4AddressHelper address;
  address.SetBase ("10.1.1.0", "255.255.255.0");

  Ipv4InterfaceContainer interfaces = address.Assign (devices);

  UdpEchoServerHelper echoServer (9);

  ApplicationContainer serverApps = echoServer.Install (nodes.Get (1));
  serverApps.Start (Seconds (1.0));
  serverApps.Stop (Seconds (10.0));

  // ApplicationContainer serverApps2 = echoServer.Install (nodes.Get (2));
  // serverApps2.Start (Seconds (1.0));
  // serverApps2.Stop (Seconds (10.0));

  UdpEchoClientHelper echoClient (interfaces.GetAddress (1), 9);
  echoClient.SetAttribute ("MaxPackets", UintegerValue (1));
  echoClient.SetAttribute ("Interval", TimeValue (Seconds (1.0)));
  echoClient.SetAttribute ("PacketSize", UintegerValue (1024));

  ApplicationContainer clientApps = echoClient.Install (nodes.Get (0));
  clientApps.Start (Seconds (2.0));
  clientApps.Stop (Seconds (10.0));

  // OnOffHelper onoff1 ("ns3::UdpSocketFactory", Address());
  // onoff1.SetAttribute ("OnTime", StringValue ("ns3::ConstantRandomVarible[Constant=1.0]"));
  // onoff1.SetAttribute ("OffTime", StringValue ("ns3::ConstantRandomVarible[Constant=0.0]"));
  // onoff1.SetAttribute ("DataRate", DataRateValue (20480));

  // TypeId tid = TypeId::LookupByName ("ns3::UdpSocketFactory");
  // Ptr<Socket> sink = Socket::CreateSocket (nodes.Get (1), tid);
  // InetSocketAddress local = InetSocketAddress (interfaces.GetAddress (1), 9);
  // sink->Bind(local);
  // AddressValue remoteAddress (InetSocketAddress (interfaces.GetAddress (1), 9));
  // onoff1.SetAttribute ("Remote", remoteAddress);
  //   Ptr<UniformRandomVariable> var = CreateObject<UniformRandomVariable>();  ApplicationContainer temp = onoff1.Install (nodes.Get(0));
  // temp.Start (Seconds (1));
  // temp.Stop (Seconds (10));

  AnimationInterface anim ("test.xml");
  Simulator::Run ();
  Simulator::Destroy ();
  return 0;
}
