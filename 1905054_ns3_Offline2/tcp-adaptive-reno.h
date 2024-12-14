#ifndef TCP_ADAPTIVERENO_H
#define TCP_ADAPTIVERENO_H


#include "ns3/sequence-number.h"
#include "ns3/traced-value.h"
#include "ns3/event-id.h"
#include "tcp-congestion-ops.h"
#include "tcp-westwood-plus.h"
#include "ns3/tcp-recovery-ops.h"


namespace ns3 {



class Time;

class EventId;

class Packet;

class TcpHeader;



class TcpAdaptiveReno : public TcpWestwoodPlus
{


public:

  static TypeId Get_Type_Id (void);

  virtual ~TcpAdaptiveReno (void);

  TcpAdaptiveReno (void);

  TcpAdaptiveReno (const TcpAdaptiveReno& sock);


  enum FilterType 
  {

    NONE,
    TUSTIN

  };



 

  virtual Ptr<TcpCongestionOps> Fork ();


  virtual uint32_t GetSsThresh (Ptr<const TcpSocketState> tcb,
                                uint32_t bytesInFlight);


  virtual void PktsAcked (Ptr<TcpSocketState> tcb, uint32_t packetsAcked,
                          const Time& rtt);



private:


  double EstimateCongestionLevel();

  void EstimateBW (const Time& rtt, Ptr<TcpSocketState> tcb);

  void EstimateIncWnd(Ptr<TcpSocketState> tcb);


protected:


  virtual void CongestionAvoidance (Ptr<TcpSocketState> tcb, uint32_t segmentsAcked);

  Time                   param_jPacketLRtt;            
  Time                   param_conjRtt;                
  Time                   param_prevConjRtt;            
  Time                   param_minRtt;                 
  Time                   param_currentRtt;             
  
  
  int32_t                param_incWnd;                                 
  int32_t                param_probeWnd; 
  uint32_t               param_baseWnd;    
            
};

} 

#endif 
