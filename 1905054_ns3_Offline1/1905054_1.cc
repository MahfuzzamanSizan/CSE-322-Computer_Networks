#include "ns3/point-to-point-layout-module.h"
#include "ns3/traffic-control-module.h"
#include "ns3/yans-wifi-helper.h"
#include "ns3/ssid.h"
#include "ns3/mobility-module.h"
#include "ns3/flow-monitor-module.h"
#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"
#include <iostream>
#include <iomanip>
#include <map>



using namespace ns3;



static void
TraceMetrics (Ptr<FlowMonitor> monitor)
{


  FlowMonitor::FlowStatsContainer stats_container = monitor->GetFlowStats ();


  std::ofstream ofs_thp ("scratch/1905054_output/throughput.dat", std::ios::out | std::ios::app);
  std::ofstream ofs_delay ("scratch/1905054_output/delay.dat", std::ios::out | std::ios::app);


  Time current_time = Now ();
  
  
  
  
  double total_throghput = 0;
  double total_delay = 0;
  double total_rx_packets = 0;
  int num_flows = 0;


  for(auto itr:stats_container)
  {
    total_throghput += (8 * itr.second.rxBytes ) / (1.0 * current_time.GetSeconds () );
    total_delay += itr.second.delaySum.GetSeconds ();
    total_rx_packets += itr.second.rxPackets;
    num_flows++;
  }



  ofs_thp <<  current_time << " " << total_throghput << std::endl; 

  ofs_delay <<  current_time << " " << total_delay/total_rx_packets << std::endl; 


 
  Simulator::Schedule (Seconds (0.1), &TraceMetrics, monitor);


}


int main (int argc, char *argv[])
{
  

  std::string bottleNeckLinkBw = "5Mbps";
  std::string bottleNeckLinkDelay = "50ms";
  std::string app_data_rate = "10Mbps"; 


  uint32_t    maximum_packets = 100;
  uint32_t    packet_size = 500;
  uint16_t port = 5001;
  


  // VARIABLE PARAMETERS
  uint32_t nWifi = 10; 
  int32_t numFlow = 100; 
  int nPackets= 20;
  int32_t speed = 10; 
  int coverageArea = 5;





  CommandLine cmd (__FILE__);


  
  cmd.AddValue ("nWifi","Max Packets allowed in the device queue", nWifi);
  cmd.AddValue ("numFlow","Max Packets allowed in the device queue", numFlow);
  cmd.AddValue ("nPackets","Max Packets allowed in the device queue", nPackets);
  cmd.AddValue ("speed","Max Packets allowed in the device queue", speed);
  cmd.AddValue ("coverageArea","Max Packets allowed in the device queue", coverageArea);
 
  
  
  cmd.Parse (argc,argv);

  

  Config::SetDefault ("ns3::OnOffApplication::PacketSize", UintegerValue (packet_size));
  Config::SetDefault ("ns3::OnOffApplication::DataRate", StringValue (app_data_rate));



  NodeContainer p2pNode;
  p2pNode.Create (2);

  
  PointToPointHelper p2pHelper;
  p2pHelper.SetDeviceAttribute  ("DataRate", StringValue (bottleNeckLinkBw));
  p2pHelper.SetChannelAttribute ("Delay", StringValue (bottleNeckLinkDelay));



  NetDeviceContainer p2pContainer;
  p2pContainer = p2pHelper.Install (p2pNode);




  NodeContainer Left ,Right;
  Left.Create (nWifi);
  Right.Create (nWifi);


  NodeContainer wifiLeft = p2pNode.Get (0);
  NodeContainer wifiRight = p2pNode.Get (1);



  YansWifiChannelHelper channelL = YansWifiChannelHelper::Default ();
  YansWifiChannelHelper channelR = YansWifiChannelHelper::Default ();


  YansWifiPhyHelper phyL,phyR;
  phyL.SetChannel (channelL.Create ()); 
  phyR.SetChannel (channelR.Create ()); 



  WifiHelper wifiHelper;
  wifiHelper.SetRemoteStationManager ("ns3::AarfWifiManager"); 
  wifiHelper.SetStandard(ns3::WIFI_STANDARD_80211b);



  WifiMacHelper macL,macR;
  Ssid ssidL = Ssid ("ns-3-ssid-left"); 
  Ssid ssidR = Ssid ("ns-3-ssid-right"); 


  
  macL.SetType ("ns3::StaWifiMac",
               "Ssid", SsidValue (ssidL),
               "ActiveProbing", BooleanValue (false)); 



               
  macR.SetType ("ns3::StaWifiMac",
               "Ssid", SsidValue (ssidR),
               "ActiveProbing", BooleanValue (false));




  NetDeviceContainer DevicesLeft, DevicesRight;
  DevicesLeft = wifiHelper.Install (phyL, macL, Left);
  DevicesRight = wifiHelper.Install (phyR, macR, Right);



  macL.SetType ("ns3::ApWifiMac",
               "Ssid", SsidValue (ssidL));




  macR.SetType ("ns3::ApWifiMac",
               "Ssid", SsidValue (ssidR));





  NetDeviceContainer appDevicesL, appDevicesR;
  appDevicesL = wifiHelper.Install (phyL, macL, Left); 
  appDevicesR = wifiHelper.Install (phyR, macR, Right); 


  

  //  *******************-----------------------------static--------------------------****************

  MobilityHelper mobilityHelper;

  mobilityHelper.SetPositionAllocator ("ns3::GridPositionAllocator",
                                 "MinX", DoubleValue (0.0),
                                 "MinY", DoubleValue (0.0),
                                 "DeltaX", DoubleValue (0.5),
                                 "DeltaY", DoubleValue (1.0),
                                 "GridWidth", UintegerValue (sqrt(nWifi)),
                                 "LayoutType", StringValue ("RowFirst"));


  
  mobilityHelper.SetMobilityModel ("ns3::ConstantPositionMobilityModel");


  
  mobilityHelper.Install (Left);
  mobilityHelper.Install (Right);
  mobilityHelper.Install (wifiLeft);
  mobilityHelper.Install (wifiRight);

  
  InternetStackHelper stack;
  stack.Install (Left);
  stack.Install (Right);
  stack.Install (wifiLeft);
  stack.Install (wifiRight);



 //--------####----------
  


  Ipv4AddressHelper addressHelper;
  addressHelper.SetBase ("10.1.1.0", "255.255.255.0");



  Ipv4InterfaceContainer p2pI;
  p2pI = addressHelper.Assign (p2pContainer);



  Ipv4InterfaceContainer InterfacesLeft, InterfacesRight;
  Ipv4InterfaceContainer InterfaceLeft, InterfaceRight;



  addressHelper.SetBase ("10.1.2.0", "255.255.255.0");
  InterfacesLeft = addressHelper.Assign (DevicesLeft);
  InterfaceLeft = addressHelper.Assign (DevicesLeft);


  addressHelper.SetBase ("10.1.3.0", "255.255.255.0");
  InterfacesRight = addressHelper.Assign (DevicesRight);
  InterfaceRight = addressHelper.Assign (DevicesRight);


  OnOffHelper clientHelper ("ns3::TcpSocketFactory", Address ());
  clientHelper.SetAttribute ("OnTime", StringValue ("ns3::ConstantRandomVariable[Constant=1]"));
  clientHelper.SetAttribute ("OffTime", StringValue ("ns3::ConstantRandomVariable[Constant=0]"));
 


  Address sinkAddress (InetSocketAddress (Ipv4Address::GetAny (), port));
  PacketSinkHelper packetSink ("ns3::TcpSocketFactory", sinkAddress);
  ApplicationContainer sink;





  for (uint32_t i = 0; i < nWifi; i++)
    {

      sink.Add (packetSink.Install (Left.Get(i)));

    }




  sink.Start (Seconds (1.0));
  sink.Stop (Seconds (6.5));


  
  ApplicationContainer client;



  int cur_flow_count = 0;



  for (uint32_t i = 0; i < nWifi; i++)
    {


      AddressValue remoteAdd (InetSocketAddress (InterfacesLeft.GetAddress(i), port));


      
      for(uint32_t j = 0; j < nWifi; j++)
      {

        clientHelper.SetAttribute ("Remote", remoteAdd);

        client.Add (clientHelper.Install (wifiRight.Get(j)));

        cur_flow_count++;

        if(cur_flow_count >= numFlow)
          break;

      }


      if(cur_flow_count >= numFlow)
          break;


    }




  client.Start (Seconds (2.0)); 
  client.Stop (Seconds (6.0)); 



  Ipv4GlobalRoutingHelper::PopulateRoutingTables ();



  Simulator::Stop (Seconds (6.5)); 


  Ptr<FlowMonitor> flow;
  FlowMonitorHelper Helper;
  flow = Helper.InstallAll();



  Simulator::Schedule (Seconds (2.2), &TraceMetrics, flow);


  std::cout << "Running the simulation" << std::endl;
  Simulator::Run ();

  
  std::cout << "Destroying the simulation" << std::endl;
  Simulator::Destroy ();
  return 0;

}