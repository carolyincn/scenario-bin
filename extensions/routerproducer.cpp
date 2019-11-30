// routerproducer.cpp

#include "routerproducer.hpp"
#include "util.hpp"
#include <counter.hpp>
#include "ns3/log.h"
#include "ns3/string.h"
#include "ns3/uinteger.h"
#include "ns3/packet.h"
#include "ns3/simulator.h"
#include <string> 

#include "ns3/ndnSIM/helper/ndn-fib-helper.hpp"
#include "model/ndn-l3-protocol.hpp"
#include "ns3/ndnSIM/NFD/daemon/mgmt/fib-manager.hpp"



NS_LOG_COMPONENT_DEFINE("RouterProducer");
// vector<string> vs9;
#define _PIPEPATH_ "pipe-"
#define _PIPESIZE_ 100

using namespace std;
namespace ns3 {
namespace ndn {

// using namespace ns3;
// using namespace ndn;

// Necessary if you are planning to use ndn::AppHelper
NS_OBJECT_ENSURE_REGISTERED(RouterProducer);


TypeId
RouterProducer::GetTypeId()
{
  static TypeId tid = 
  	TypeId("RouterProducer")
  		.SetParent<ndn::App>()
  		.AddConstructor<RouterProducer>()
  		.AddAttribute("Prefix", "Prefix, for which producer has the data", StringValue("/"),
       						  MakeNameAccessor(&RouterProducer::m_prefix), MakeNameChecker())
  		.AddAttribute("PayloadSize", "Virtual payload size for Content packets", UintegerValue(1024),
                    MakeUintegerAccessor(&RouterProducer::m_virtualPayloadSize),
                    MakeUintegerChecker<uint32_t>())
      .AddAttribute("Freshness", "Freshness of data packets, if 0, then unlimited freshness",
                    TimeValue(Seconds(0.5)), MakeTimeAccessor(&RouterProducer::m_freshness),
                    MakeTimeChecker())
      .AddAttribute(
         "Signature",
         "Fake signature, 0 valid signature (default), other values application-specific",
         UintegerValue(0), MakeUintegerAccessor(&RouterProducer::m_signature),
         MakeUintegerChecker<uint32_t>())
      .AddAttribute("KeyLocator",
                    "Name to be used for key locator.  If root, then key locator is not used",
                    NameValue(), MakeNameAccessor(&RouterProducer::m_keyLocator), MakeNameChecker());
  return tid;
}

RouterProducer::RouterProducer()
{
}

void
RouterProducer::OnInterest(std::shared_ptr<const ndn::Interest> interest)
{
  App::OnInterest(interest); // forward call to perform app-level tracing
  // do nothing else (hijack interest)

  // NS_LOG_DEBUG("Do nothing for incoming interest for" << interest->getName());
  // NS_LOG_DEBUG("received\n");
  

  std::string intname = interest->getName().toUri();
  // NS_LOG_DEBUG(interest->getName());


  Name dataName(interest->getName());
  // dataName.append(m_postfix);
  // dataName.appendVersion();

  auto data = make_shared<Data>();
  data->setName(dataName);
  data->setFreshnessPeriod(::ndn::time::milliseconds(m_freshness.GetMilliSeconds()));

  // // auto buffer = make_shared< ::ndn::Buffer>(10);
  // // const int payload[2] = {0, 1};
  // // memcpy(buffer->get(), &payload, 2*sizeof(int));
  // data->setContent(buffer);
  int type = getPrefixType(intname);
  Ptr<L3Protocol> L3protocol = GetNode()->GetObject<L3Protocol>();
  shared_ptr<nfd::Forwarder> forwarder = L3protocol->getForwarder();
  if (type == 1) { // return basic info
    ::nfd::SimpleCounter::rep nInInterests = forwarder->getCounters().nInInterests;
    ::nfd::SimpleCounter::rep nOutInterests = forwarder->getCounters().nOutInterests;
    ::nfd::SimpleCounter::rep nInNacks = forwarder->getCounters().nInNacks;
    ::nfd::SimpleCounter::rep nOutNacks = forwarder->getCounters().nOutNacks;
    ::nfd::SimpleCounter::rep nCsHits = forwarder->getCounters().nCsHits;
    ::nfd::SimpleCounter::rep nCsMisses = forwarder->getCounters().nCsMisses;
    int int_inInterest = nInInterests - temp_inInterest;
    int int_outInterest = nOutInterests - temp_outInterest;
    int int_inNack = nInNacks - temp_inNack;
    int int_outNack = nOutNacks - temp_outNack;
    int int_csHit = nCsHits - temp_csHit;
    int int_csMiss = nCsMisses - temp_csMiss;

    temp_inInterest = nInInterests;
    temp_outInterest = nOutInterests;
    temp_inNack = nInNacks;
    temp_outNack = nOutNacks;
    temp_csHit = nCsHits;
    temp_csMiss = nCsMisses;

    // int temp = forwarder->getFib().size();
    // forwarder->getPit().size();
    // 
    int int_size = sizeof(int);
    int buffer_size = int_size * 6;
    uint8_t buffer[buffer_size];
    memcpy(buffer, &int_inInterest, int_size);
    memcpy(buffer + int_size, &int_outInterest, int_size);
    memcpy(buffer + int_size * 2, &int_inNack, int_size);
    memcpy(buffer + int_size * 3, &int_outNack, int_size);
    memcpy(buffer + int_size * 4, &int_csHit, int_size);
    memcpy(buffer + int_size * 5, &int_csMiss, int_size);
    data->setContent(reinterpret_cast<const uint8_t*>(buffer), buffer_size);
    NS_LOG_DEBUG("node(" << GetNode()->GetId() << ") responding with Data: " << data->getName() << " " << int_inNack);

  } else if (type == 2){ // return other message, like confirmation or FIT messages // TODO
    char buf[_PIPESIZE_];
    // if(read(fd, buf, _PIPESIZE_) < 0){
    //   printf("Read Error %d\n", errno);
    //   data->setContent(make_shared< ::ndn::Buffer>(m_virtualPayloadSize));
    // } else{
    //   std::cout << "router " << buf << std::endl;
    //   uint8_t buffer[_PIPESIZE_];
    //   memcpy(buffer, &buf, _PIPESIZE_);
    //   data->setContent(reinterpret_cast<const uint8_t*>(buffer), _PIPESIZE_);
    // }
    string buff;
    while(read(fd, buf, _PIPESIZE_) >= 0){
      // std::cout << "router " << buf << std::endl;
      buff = buff + buf + ' ';
      // std::cout << "buff " << buff << std::endl;

      // uint8_t buffer[_PIPESIZE_];

      // memcpy(buffer, &buf, _PIPESIZE_);
      // printf("Read Error %d\n", errno);
      // data->setContent(make_shared< ::ndn::Buffer>(m_virtualPayloadSize));
    }
    // buff.erase(buff.end() - 1); // eliminate the last blank

    // ba buff chuliyixia,zaifachuqu 
    buff = packNackInfo(buff);

    NS_LOG_DEBUG("node(" << GetNode()->GetId() << ") " << buff);
    int len = buff.size() + 1;
    char bufff[len];
    uint8_t buffer[len];
    for(int i = 0; i < len - 1; i ++){
      bufff[i] = buff[i];
      // printf("%d,%d\n", buff[i], bufff[i]);
    }
    bufff[len - 1] = '\0';
    memcpy(buffer, &len, sizeof(int));
    memcpy(buffer + sizeof(int), &bufff, len);
    data->setContent(reinterpret_cast<const uint8_t*>(buffer), len + sizeof(int));

    // NS_LOG_DEBUG("node(" << GetNode()->GetId() << ") responding with Data: " << data->getName());
  } else {
    string evilprefix = "/";
    int slashcount = 0;
    for(int i  = 0; i < intname.size(); i ++){
      if(slashcount < 3){
        if(intname[i] == '/')
          slashcount ++;
        continue;
      }
      evilprefix += intname[i];

    }
    data->setContent(make_shared< ::ndn::Buffer>(m_virtualPayloadSize));
    // int nid = GetNode()->GetId();
    // vs[nid - 8].push_back(evilprefix);
    std::cout << "evilvec size before: " << forwarder->getEvilvec().size() << std::endl;

    // forwarder->setEvilvec(evilprefix);
    std::cout << "evilvec size after: " << forwarder->getEvilvec().size() << std::endl;
    std::cout << evilprefix << std::endl;

    // m_face.setInterestFilter("/p1/evil");
    data->setContent(make_shared< ::ndn::Buffer>(m_virtualPayloadSize));
    NS_LOG_DEBUG("node(" << GetNode()->GetId() << ") responding with Data: " << data->getName());

  }

  Signature signature;
  SignatureInfo signatureInfo(static_cast< ::ndn::tlv::SignatureTypeValue>(255));

  if (m_keyLocator.size() > 0) {
    signatureInfo.setKeyLocator(m_keyLocator);
  }
  signature.setInfo(signatureInfo);
  signature.setValue(::ndn::makeNonNegativeIntegerBlock(::ndn::tlv::SignatureValue, m_signature));

  data->setSignature(signature);

  // to create real wire encoding
  data->wireEncode();

  // cout << "test" << endl;
  m_transmittedDatas(data, this, m_face);
  m_appLink->onReceiveData(*data);
}

void
RouterProducer::StartApplication()
{
  App::StartApplication();

  // equivalent to setting interest filter for "/prefix" prefix
  FibHelper::AddRoute(GetNode(), m_prefix, m_face, 0);
  // NS_LOG_DEBUG(GetNode()->GetId());
  int nid = GetNode()->GetId();
  //if(nid == 8 || nid == 9 || nid == 11){
  string path = _PIPEPATH_;
  path = path + to_string(nid);
  NS_LOG_DEBUG("Build pipe for node " << nid << ": " << path);
  // const char *path = "/tmp/vsd_fifo_router";
  int ret = mkfifo(path.c_str(), 0666 | S_IFIFO);
  if (ret == -1){
    printf("Pipe existed.\n");
    //return;
  }
  fd = open(path.c_str(), O_RDWR|O_NONBLOCK);
  if(fd < 0){
    printf("Failed to open pipe.\n");
    return;
  }

}

void
RouterProducer::StopApplication()
{
  App::StopApplication();
}

} // namespace ndn
} // namespace ns3