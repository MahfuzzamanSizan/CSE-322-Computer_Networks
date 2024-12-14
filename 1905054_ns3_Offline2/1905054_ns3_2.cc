#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <fstream>
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/point-to-point-layout-module.h"
#include "ns3/applications-module.h"
#include "ns3/stats-module.h"
#include "ns3/callback.h"
#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/flow-monitor-module.h"
#include "ns3/csma-module.h"


using namespace ns3;



class MyApplication : public Application
{

public:


  MyApplication ();


  virtual ~MyApplication ();
  
   
  void Setup_Function (Ptr<Socket> socket, Address address, uint32_t packet_size, DataRate data_rate, uint32_t simulation_time);
  
  
  static TypeId Get_Type_Id (void);


private:


  void Schedule_Tx (void);
  void Send_Packet (void);

  virtual void Start_Application (void);
  virtual void Stop_Application (void);

  

 
  EventId         param_sendEvent;
  bool            param_running;
  uint32_t        param_packetsSent;
  uint32_t        param_simultime;
  Ptr<Socket>     param_socket;
  Address         param_peer;
  uint32_t        param_packetSize;
  DataRate        param_dataRate;




};



MyApplication::MyApplication ()
  : param_sendEvent (),
    param_running (false),
    param_packetsSent (0),
    param_simultime (0),
    param_socket (0),
    param_peer (),
    param_packetSize (0),
    param_dataRate (0)
{
}

MyApplication::~MyApplication ()
{


  param_socket = 0;


}


void
MyApplication::Send_Packet (void)
{

  Ptr<Packet> packet = Create<Packet> (param_packetSize);

  param_socket->Send (packet);

  if(Simulator::Now().GetSeconds() < param_simultime) Schedule_Tx();

}

void
MyApplication::Schedule_Tx (void)
{

  if (param_running)
    {

      Time tNext (Seconds (param_packetSize * 8 / static_cast<double> (param_dataRate.GetBitRate ())));

      param_sendEvent = Simulator::Schedule (tNext, &MyApplication::Send_Packet, this);

    }

}


TypeId MyApplication::Get_Type_Id (void)
{

  static TypeId type_id = TypeId ("MyApplication")
    .SetParent<Application> ()
    .SetGroupName ("Tutorial")
    .AddConstructor<MyApplication> ()
    ;

  return type_id;

}


static void
CwndChange (Ptr<OutputStreamWrapper> stream, uint32_t old_cwnd, uint32_t new_cwnd)
{
  

  *stream->GetStream () << Simulator::Now ().GetSeconds () << " " << new_cwnd << std::endl;


}

void
MyApplication::Setup_Function (Ptr<Socket> socket, Address address, uint32_t packet_size, DataRate data_rate, uint32_t simulation_time)
{


  param_packetSize = packet_size;

  param_dataRate = data_rate;

  param_simultime = simulation_time;

  param_socket = socket;

  param_peer = address;

  
}


void
MyApplication::Start_Application (void)
{
  param_running = true;


  param_packetsSent = 0;


    if (InetSocketAddress::IsMatchingType (param_peer))
    {

      param_socket->Bind ();

    }

  else
    {

      param_socket->Bind6 ();

    }



  param_socket->Connect (param_peer);

  Send_Packet ();


}

void
MyApplication::Stop_Application (void)
{
  param_running = false;

  if (param_sendEvent.IsRunning ())
    {

      Simulator::Cancel (param_sendEvent);

    }

  if (param_socket)
    {

      param_socket->Close ();

    }

}



static Ptr<OutputStreamWrapper> TPvsDR1, TPvsDR2, TPvsPL1,  TPvsPL2,  osCwndTime1,osCwndTime2;


int main(int argc, char *argv[]){



    int no_of_leaf = 2;
    int no_of_flows = 2;
    int simulation_time = 60;
    int cleanup_time = 2;
    int bttlnkRate = 50;
    int bttlnkDelay = 100;
    int packet_loss_exp = 6;
    int exp = 1;
    int flag=1;
    int j = 0;
    



    float AvgThroughput = 0;
    float CurThroughput = 0;
    float CurThroughputArr[] = {0, 0};
    


    uint16_t sp = 8080;
    uint32_t pay_load_size = 1472;
    uint32_t sent_packets = 0;
    uint32_t received_packets = 0;
    uint32_t lost_packets = 0;


    std::string tcpVariant1 = "ns3::TcpNewReno"; // TcpNewReno
    std::string tcpVariant2 = "ns3::TcpNewReno"; //TcpAdaptiveReno TcpWestwood, TcpHighSpeed
    std::string sender_data_rate = "1Gbps";
    std::string sender_delay = "1ms";
    std::string output_folder = "scratch/1905054_output2";

    
    double packet_loss_rate = (1.0 / std::pow(10, packet_loss_exp));


    CommandLine cmd (__FILE__);


    
    cmd.AddValue ("bttlnkRate","Max Packets allowed in the device queue", bttlnkRate);
    cmd.AddValue ("packet_loss_rate","Max Packets allowed in the device queue", packet_loss_rate);
    cmd.AddValue ("flag","Max Packets allowed in the device queue", flag);
    cmd.AddValue ("tcpVariant1","Max Packets allowed in the device queue", tcpVariant1);
    cmd.AddValue ("tcpVariant2","Max Packets allowed in the device queue", tcpVariant2);





    TPvsDR1 = Create<OutputStreamWrapper>("./scratch/1905054_output2/flow1/TPvsDR.txt", std::ios::app);
    TPvsDR2 = Create<OutputStreamWrapper>("./scratch/1905054_output2/flow2/TPvsDR.txt", std::ios::app);
    TPvsPL1 = Create<OutputStreamWrapper>("./scratch/1905054_output2/flow1/TPvsPL.txt", std::ios::app);
    TPvsPL2 = Create<OutputStreamWrapper>("./scratch/1905054_output2/flow2/TPvsPL.txt", std::ios::app);



    
    cmd.Parse (argc,argv);



    std::string file = output_folder + "/data_" + std::to_string(exp) + ".txt";
    std::string bottleNeckDataRate = std::to_string(bttlnkRate) + "Mbps";
    std::string bottleNeckDelay = std::to_string(bttlnkDelay) + "ms";

    no_of_flows = no_of_leaf;

    
   
    Config::SetDefault ("ns3::TcpSocket::SegmentSize", UintegerValue (pay_load_size));

    
  

    PointToPointHelper p2pLeaf;
    p2pLeaf.SetDeviceAttribute  ("DataRate", StringValue (sender_data_rate));
    p2pLeaf.SetChannelAttribute ("Delay", StringValue (sender_delay));



    PointToPointHelper p2pBottleLink;
    p2pBottleLink.SetDeviceAttribute  ("DataRate", StringValue (bottleNeckDataRate));
    p2pBottleLink.SetChannelAttribute ("Delay", StringValue (bottleNeckDelay));
    


    p2pLeaf.SetQueue ("ns3::DropTailQueue", "MaxSize", StringValue (std::to_string (bttlnkDelay * bttlnkRate) + "p"));



    PointToPointDumbbellHelper dumble (no_of_leaf, p2pLeaf,
                                  no_of_leaf, p2pLeaf,
                                  p2pBottleLink);




   
    Config::SetDefault ("ns3::TcpL4Protocol::SocketType", StringValue (tcpVariant1));



    InternetStackHelper stack_helper1;


    for (uint32_t i = 0; i < dumble.LeftCount (); i+=2)
      {

        stack_helper1.Install (dumble.GetLeft (i)); 

      }


    for (uint32_t i = 0; i < dumble.RightCount (); i+=2)
      {

        stack_helper1.Install (dumble.GetRight (i)); 

      }



    stack_helper1.Install (dumble.GetLeft ());

    stack_helper1.Install (dumble.GetRight ());

    



    Config::SetDefault ("ns3::TcpL4Protocol::SocketType", StringValue (tcpVariant2));




    InternetStackHelper stack_helper2;

    for (uint32_t i = 1; i < dumble.LeftCount (); i+=2)
      {

        stack_helper2.Install (dumble.GetLeft (i));

      }
      


    for (uint32_t i = 1; i < dumble.RightCount (); i+=2)
      {

        stack_helper2.Install (dumble.GetRight (i)); 

      }




    Ptr<RateErrorModel> error_model = CreateObject<RateErrorModel> ();
    error_model->SetAttribute ("ErrorRate", DoubleValue (packet_loss_rate));
    dumble.m_routerDevices.Get(1)->SetAttribute ("ReceiveErrorModel", PointerValue (error_model)); 


   
    dumble.AssignIpv4Addresses (Ipv4AddressHelper ("10.1.1.0", "255.255.255.0"), 
                          Ipv4AddressHelper ("10.2.1.0", "255.255.255.0"),  
                          Ipv4AddressHelper ("10.3.1.0", "255.255.255.0")); 



    Ipv4GlobalRoutingHelper::PopulateRoutingTables (); 

    

    FlowMonitorHelper flowHelper;
    flowHelper.SetMonitorAttribute("MaxPerHopDelay", TimeValue(Seconds(cleanup_time)));
    Ptr<FlowMonitor> monitor = flowHelper.InstallAll ();


    


    Ptr<Ipv4FlowClassifier> classifier = DynamicCast<Ipv4FlowClassifier> (flowHelper.GetClassifier ());
    FlowMonitor::FlowStatsContainer stats_container = monitor->GetFlowStats ();


    for(int i=0;i<no_of_flows; i++)
    {

      Address sink (InetSocketAddress (dumble.GetRightIpv4Address (i), sp));

      PacketSinkHelper packet_sink ("ns3::TcpSocketFactory", InetSocketAddress (Ipv4Address::GetAny(), sp));

      ApplicationContainer sink_apps = packet_sink.Install (dumble.GetRight (i));

      Ptr<Socket> tcp_socket = Socket::CreateSocket (dumble.GetLeft (i), TcpSocketFactory::GetTypeId ());

      Ptr<MyApplication> application = CreateObject<MyApplication> ();

      AsciiTraceHelper asciiTrace;

      std::ostringstream string_stream;

      Ptr<OutputStreamWrapper> stream = asciiTrace.CreateFileStream (string_stream.str());


      sink_apps.Start (Seconds (0));
      sink_apps.Stop (Seconds (simulation_time+cleanup_time));


      application->Setup_Function (tcp_socket, sink, pay_load_size, DataRate (sender_data_rate), simulation_time);


      dumble.GetLeft (i)->AddApplication (application);
      application->SetStartTime (Seconds (1));
      application->SetStopTime (Seconds (simulation_time));


      
      string_stream << output_folder << "/flow" << i+1 <<  ".cwnd";

      tcp_socket->TraceConnectWithoutContext ("CongestionWindow", MakeBoundCallback (&CwndChange, stream));


    }

    
  
    Simulator::Stop (Seconds (simulation_time+cleanup_time));
    Simulator::Run ();


    


    std::ofstream MyFile(file, std::ios_base::app);



    for (auto iter = stats_container.begin (); iter != stats_container.end (); ++iter) 
    
    {


      CurThroughput = iter->second.rxBytes * 8.0/((simulation_time+cleanup_time)*1000);
      
      if(j%2 == 0) { CurThroughputArr[0] += iter->second.rxBytes; }
      if(j%2 == 1) { CurThroughputArr[1] += iter->second.rxBytes; }


      sent_packets = sent_packets +(iter->second.txPackets);
      received_packets = received_packets + (iter->second.rxPackets);
      lost_packets = lost_packets + (iter->second.lostPackets);
      j = j + 1;

    }

    
    CurThroughputArr[0] /= ((simulation_time + cleanup_time)*1000);
    CurThroughputArr[1] /= ((simulation_time + cleanup_time)*1000);
    AvgThroughput = CurThroughputArr[0] + CurThroughputArr[1];


    if(flag==1)
    {

        *TPvsDR1->GetStream()<<bttlnkRate<<" "<<CurThroughputArr[0]<<"\n";

        *TPvsDR2->GetStream()<<bttlnkRate<<" "<<CurThroughputArr[1]<<"\n";

    }

    if(flag==2)
    {

        *TPvsPL1->GetStream()<<packet_loss_rate<<" "<<CurThroughputArr[0]<<"\n";

        *TPvsPL2->GetStream()<<packet_loss_rate<<" "<<CurThroughputArr[1]<<"\n";

    }

    Simulator::Destroy ();


    return 0;

}