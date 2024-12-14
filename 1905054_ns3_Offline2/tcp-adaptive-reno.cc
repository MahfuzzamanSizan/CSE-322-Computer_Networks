#include "ns3/simulator.h"
#include "rtt-estimator.h"
#include "tcp-socket-base.h"
#include "tcp-adaptive-reno.h"
#include "ns3/log.h"


namespace ns3 {


NS_OBJECT_ENSURE_REGISTERED (TcpAdaptiveReno);



TcpAdaptiveReno::TcpAdaptiveReno (void) :
  TcpWestwoodPlus(),
  param_currentRtt (Time (0)),
  param_jPacketLRtt (Time (0)),
  param_conjRtt (Time (0)),
  param_prevConjRtt (Time(0)),
  param_incWnd (0),
  param_minRtt (Time (0)),
  param_baseWnd (0),
  param_probeWnd (0)
{
  NS_LOG_FUNCTION (this);
}

TcpAdaptiveReno::TcpAdaptiveReno (const TcpAdaptiveReno& sock) :
  TcpWestwoodPlus (sock),
  param_currentRtt (Time (0)),
  param_jPacketLRtt (Time (0)),
  param_conjRtt (Time (0)),
  param_prevConjRtt (Time(0)),
  param_incWnd (0),
  param_minRtt (Time (0)),
  param_baseWnd (0),
  param_probeWnd (0)
{

  NS_LOG_FUNCTION (this);

}



TcpAdaptiveReno::~TcpAdaptiveReno (void)
{

}



TypeId
TcpAdaptiveReno::Get_Type_Id (void)

{

  static TypeId type_id = TypeId("ns3::TcpAdaptiveReno")
    .SetParent<TcpNewReno>()
    .SetGroupName ("Internet")
    .AddConstructor<TcpAdaptiveReno>()
    .AddAttribute("FilterType", "choose no filter",
                  EnumValue(TcpAdaptiveReno::TUSTIN), MakeEnumAccessor(&TcpAdaptiveReno::m_fType),
                  MakeEnumChecker(TcpAdaptiveReno::NONE, "None", TcpAdaptiveReno::TUSTIN, "Tustin"))
    .AddTraceSource("EstimatedBW", "estimated bandwidth",
                    MakeTraceSourceAccessor(&TcpAdaptiveReno::m_currentBW),
                    "ns3::TracedValueCallback::Double")
  ;


  return type_id;


}


Ptr<TcpCongestionOps>
TcpAdaptiveReno::Fork ()
{

  return CreateObject<TcpAdaptiveReno> (*this);

}



void
TcpAdaptiveReno::PktsAcked (Ptr<TcpSocketState> tcb, uint32_t packetsAcked,
                        const Time& rtt)
{


  if (rtt.IsZero ())
    {
      return;
    }

  m_ackedSegments = m_ackedSegments +  packetsAcked;

  
  
  if(param_minRtt.IsZero()) 
  { 

    param_minRtt = rtt; 
    
  }

  else if(rtt <= param_minRtt) 
  { 

    param_minRtt = rtt; 
    
  }


  param_currentRtt = rtt;


  TcpWestwoodPlus::EstimateBW (rtt, tcb);

}



void 
TcpAdaptiveReno::EstimateIncWnd(Ptr<TcpSocketState> tcb)
{


  int scaling_factor = 1000; 
  double congestion = EstimateCongestionLevel();
  double param_maxIncWnd = m_currentBW.Get().GetBitRate() / scaling_factor * static_cast<double> (tcb->m_segmentSize * tcb->m_segmentSize) ; 
  double alpha = 10; 
  double beta = 2 * param_maxIncWnd * ((1/alpha) - ((1/alpha + 1)/(std::exp(alpha))));
  double gamma = 1 - (2 * param_maxIncWnd * ((1/alpha) - ((1/alpha + 0.5)/(std::exp(alpha)))));


  param_incWnd = (int)((param_maxIncWnd / std::exp(alpha * congestion)) + (beta * congestion) + gamma);


}


uint32_t
TcpAdaptiveReno::GetSsThresh (Ptr<const TcpSocketState> tcb,
                          uint32_t bytesInFlight)
{


  double congestion = EstimateCongestionLevel();

  param_prevConjRtt = param_conjRtt; 
  param_jPacketLRtt = param_currentRtt; 
  
  

  uint32_t ssthresh = std::max (
    2*tcb->m_segmentSize,
    (uint32_t) (tcb->m_cWnd / (1.0+congestion))
  );


  param_baseWnd = ssthresh;
  param_probeWnd = 0;
  
  return ssthresh;

}



double
TcpAdaptiveReno::EstimateCongestionLevel()
{

  float param_a = 0.85; 

  if(param_prevConjRtt < param_minRtt) 
  {

    param_a = 0; 

  }
  
  double conjRtt = param_a*param_prevConjRtt.GetSeconds() + (1-param_a)*param_jPacketLRtt.GetSeconds(); 

  param_conjRtt = Seconds(conjRtt); 


 
  return std::min(
    (param_currentRtt.GetSeconds() - param_minRtt.GetSeconds()) / (conjRtt - param_minRtt.GetSeconds()),
    1.0
  );


}





void
TcpAdaptiveReno::CongestionAvoidance (Ptr<TcpSocketState> tcb, uint32_t segmentsAcked)
{


  if (segmentsAcked > 0)
    {

      EstimateIncWnd(tcb);

      double address = static_cast<double> (tcb->m_segmentSize * tcb->m_segmentSize) / tcb->m_cWnd.Get ();

      address = std::max (1.0, address);

      param_baseWnd = param_baseWnd + static_cast<uint32_t> (address);

      param_probeWnd = std::max(
        (double) (param_probeWnd + param_incWnd / (int)tcb->m_cWnd.Get()), 
        (double) 0
      );

      

      tcb->m_cWnd = param_baseWnd + param_probeWnd;


    }


}



} 
