// ctrlconsumer.cpp

#include "ctrlconsumer.hpp"
#include "util.hpp"

#include "ns3/ptr.h"
#include "ns3/log.h"
#include "ns3/simulator.h"
#include "ns3/packet.h"
#include "ns3/string.h"
#include "ns3/boolean.h"
#include "ns3/uinteger.h"
#include "ns3/integer.h"
#include "ns3/double.h"
#include "ns3/node.h"

#include "ns3/ndnSIM/helper/ndn-stack-helper.hpp"
#include "ns3/ndnSIM/helper/ndn-fib-helper.hpp"

#include "ns3/random-variable-stream.h"
#include <fstream>
#include <stdio.h> 
#include <sys/types.h>  
#include <sys/stat.h> 
#include <unistd.h> 
#include <fcntl.h> 
#include <string> 
#include <sstream>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

#define _PIPEPATH_ "pipe-" 
#define _PIPESIZE_ 100

using namespace std;

NS_LOG_COMPONENT_DEFINE("CtrlConsumer");

namespace ns3 {
namespace ndn {

NS_OBJECT_ENSURE_REGISTERED(CtrlConsumer);

// register NS-3 type
TypeId
CtrlConsumer::GetTypeId()
{
  static TypeId tid = 
    TypeId("CtrlConsumer")
      .SetParent<ndn::App>()
      .AddConstructor<CtrlConsumer>()
      .AddAttribute("Frequency", "Frequency of interest packets", StringValue("1.0"),
                    MakeDoubleAccessor(&CtrlConsumer::m_frequency), MakeDoubleChecker<double>())
      .AddAttribute("Prefix", "Prefix, for which consumer wants the data", StringValue("/"),
                    MakeNameAccessor(&CtrlConsumer::m_prefix), MakeNameChecker());
  
  return tid;
}

// Processing upon start of the application
void
CtrlConsumer::StartApplication()
{
  // initialize ndn::App
  ndn::App::StartApplication();

  // Add entry to FIB for `/prefix/sub`
  ndn::FibHelper::AddRoute(GetNode(), m_prefix, m_face, 0);

  // Create and open pipe
  int nid = GetNode()->GetId();

  string path_out = _PIPEPATH_;
  path_out = path_out + to_string(nid);
  NS_LOG_DEBUG("Build pipe for node " << nid << ": " << path_out);
  int ret = mkfifo(path_out.c_str(), 0666 | S_IFIFO);
  if (ret == -1){
    printf("Pipe existed.\n");
  }
  fd_out = open(path_out.c_str(), O_WRONLY);
  if(fd_out < 0){
    printf("Failed to open pipe.\n");
    return;
  }

  string path_in = _PIPEPATH_;
  path_in = path_in + "res-" + to_string(nid);
  NS_LOG_DEBUG("Build pipe for node " << nid << ": " << path_in);
  ret = mkfifo(path_in.c_str(), 0666 | S_IFIFO);
  if (ret == -1){
    printf("Pipe existed.\n");
  }
  fd_in = open(path_in.c_str(), O_RDONLY|O_NONBLOCK);
  if(fd_in < 0){
    printf("Failed to open pipe.\n");
    return;
  }

  // Schedule send of first interest
  // Simulator::Schedule(Seconds(1.0 / m_frequency), &CtrlConsumer::SendInterest, this);
  SendInterest();
}

// Processing when application is stopped
void
CtrlConsumer::StopApplication()
{
  m_nextSend.Cancel();
  // cleanup ndn::App
  ndn::App::StopApplication();
}

void
CtrlConsumer::SendInterest()
{
  /////////////////////////////////////
  // Sending one Interest packet out //
  /////////////////////////////////////

  // Create and configure ndn::Interest
  Ptr<UniformRandomVariable> rand = CreateObject<UniformRandomVariable>();
  // std::string prefix = "/prefix/" + std::to_string(rand->GetValue(0, 10));
  auto interest = std::make_shared<ndn::Interest>(m_prefix);
  interest->setNonce(rand->GetValue(0, std::numeric_limits<uint32_t>::max()));
  interest->setInterestLifetime(ndn::time::seconds(1));
  interest->setMustBeFresh(1);
  NS_LOG_DEBUG("Sending Interest packet for " << *interest);

  // Call trace (for logging purposes)
  m_transmittedInterests(interest, this, m_face);

  m_appLink->onReceiveInterest(*interest);
  m_nextSend = Simulator::Schedule(Seconds(1.0 / m_frequency), &CtrlConsumer::SendInterest, this);
}

// Callback that will be called when Interest arrives
// void
// CtrlConsumer::OnInterest(std::shared_ptr<const ndn::Interest> interest)
// {
//   ndn::App::OnInterest(interest);

//   NS_LOG_DEBUG("Received Interest packet for " << interest->getName());

//   // Note that Interests send out by the app will not be sent back to the app !

//   auto data = std::make_shared<ndn::Data>(interest->getName());
//   data->setFreshnessPeriod(ndn::time::milliseconds(1000));
//   data->setContent(std::make_shared< ::ndn::Buffer>(1024));
//   ndn::StackHelper::getKeyChain().sign(*data);

//   NS_LOG_DEBUG("Sending Data packet for " << data->getName());

//   // Call trace (for logging purposes)
//   m_transmittedDatas(data, this, m_face);

//   m_appLink->onReceiveData(*data);
// }

// Callback that will be called when Data arrives
void
CtrlConsumer::OnData(std::shared_ptr<const ndn::Data> data)
{
  // NS_LOG_DEBUG("Get Data packet from " << data->getName());
  
  string to_prefix(data->getName().toUri());
  // NS_LOG_DEBUG("Get Data packet from " << data->getName());
  int type = getPrefixType(to_prefix);
  int int_size = sizeof(int);

  if(type == 1){ // if just asked for info, then deal with info
    uint8_t *buffer = ((uint8_t*)data->getContent().value());
    int int_inInterest, int_outInterest, int_inNack, int_outNack, int_csHit, int_csMiss;
    
    memcpy(&int_inInterest, buffer, int_size);
    memcpy(&int_outInterest, buffer + int_size, int_size);
    memcpy(&int_inNack, buffer + int_size * 2, int_size);
    memcpy(&int_outNack, buffer + int_size * 3, int_size);
    memcpy(&int_csHit, buffer + int_size * 4, int_size);
    memcpy(&int_csMiss, buffer + int_size * 5, int_size);

    abnormal = 0;
    // if (double(int_inNack) /int_outInterest >= 0.1){  // TODO // change threshold
    if (int_inNack > 0){  // TODO // change threshold
      abnormal = 1;
    }

    if (abnormal){ // if turned out to be an abnormal, send anathor interest to the router to ask for more info
      Ptr<UniformRandomVariable> rand = CreateObject<UniformRandomVariable>();
      std::string prefix = m_prefix.toUri() + "/info";
      auto interest = std::make_shared<ndn::Interest>(prefix);
      interest->setNonce(rand->GetValue(0, std::numeric_limits<uint32_t>::max()));
      interest->setInterestLifetime(ndn::time::seconds(1));
      interest->setMustBeFresh(1);
      NS_LOG_DEBUG("Sending Interest packet for " << *interest);
      // std::cout << "face id " << m_face->getLocalUri() << std::endl;

      m_transmittedInterests(interest, this, m_face);

      m_appLink->onReceiveInterest(*interest);
    }
  } else if (type == 2){ // check if routers have done what the controller asked // TODO
    
    uint8_t *buffer = ((uint8_t*)data->getContent().value());
    
    int content_len; // length of the received content
    memcpy(&content_len, buffer, int_size);
    
    stringstream ss;
    ss << Simulator::Now().GetSeconds();
    string time = ss.str(); // present time
    time += " ";
    
    string prefix = getPrefixHeader(to_prefix, 1);
    prefix += "\t"; // the info of router <prefix>
    
    int lenlen= sizeof(char); // let the length of the info to send be no longer than the size of a char (0 ~ 255)

    int len = lenlen + time.size() + prefix.size() + content_len; // the whole length of the info to send to the pipe
    char lenstr = len - lenlen; // the length of the info to send, the length itself excluded

    uint8_t buf[len];
    memcpy(buf, &lenstr, lenlen);
    memcpy(buf + lenlen, time.c_str(), time.size());
    memcpy(buf + lenlen + time.size(), prefix.c_str(), prefix.size());
    memcpy(buf + lenlen + time.size() + prefix.size(), buffer + int_size, content_len);    

    // NS_LOG_DEBUG("Receiving Data packet for " << data->getName().toUri() << " : " << int_inInterest << " " << int_csHit);
    // send to pipe
    if (write(fd_out, buf, len) < 0){
      printf("Write Error %d\n", errno);
    }

    NS_LOG_DEBUG("Controller receieved from " << to_prefix << " with data: " << buf);
    // uint8_t *buffer = ((uint8_t*)data->getContent().value());
    
    char evillenchr;

    // std::cout << "read " << read(fd_in, &evillenchr, 1) << std::endl;
    // std::cout << "lenchr " << evillenchr << std::endl;
    // receive the result of the util.py
    while (read(fd_in, &evillenchr, 1) > 0){
      int evillen = int(evillenchr);
      if (evillen == 0)
        continue;
      std::cout << "lenint " << evillen << std::endl;
      char evil[evillen + 1];
      if (read(fd_in, evil, evillen) < 0)
        printf("Read Error in Controller.\n");
      evil[evillen] = '\0';
      std::cout << "evil " << evil << std::endl;
    

      // string prefixtosend;
      string evilprefix;
      string name;
      string temp;
      string facestr;
      // int flag = 0;
      for (int i = 0; i < evillen; i ++){
        if(evil[i] == '\t'){
          facestr = temp;
          std::cout << "name " << name << " evilprefix " << evilprefix << " face " << facestr << std::endl;
          Ptr<UniformRandomVariable> rand = CreateObject<UniformRandomVariable>();
          name = name + "/p" + evilprefix;
          auto interest = std::make_shared<ndn::Interest>(name);
          interest->setNonce(rand->GetValue(0, std::numeric_limits<uint32_t>::max()));
          interest->setInterestLifetime(ndn::time::seconds(1));
          interest->setMustBeFresh(1);
          std::cout << "interest name " << interest->toUri() << std::endl;

          NS_LOG_DEBUG("Sending Interest packet for " << *interest);
          shared_ptr< Face > face = L3Protocol::getL3Protocol(GetNode())->getFaceById(atoi(facestr.c_str()));
          // std::cout << "face id " << face->getLocalUri() << std::endl;

          m_transmittedInterests(interest, this, m_face);

          // m_appLink->sendInterest(*interest);
          face->getLinkService()->sendInterest(*interest);

          // evilprefix = "";
          // name = "";
          temp = "";
        } else if(evil[i] == ' '){
          evilprefix = temp;
          temp = "";
          // flag = 1;
        } else if(evil[i] == '?'){
          name = temp;
          temp = "";
        } else{
          temp = temp + evil[i];
        }
      }
    }
      // Ptr<UniformRandomVariable> rand = CreateObject<UniformRandomVariable>();
      // std::string prefix = m_prefix.toUri() + "/info";
      // auto interest = std::make_shared<ndn::Interest>(prefix);
      // interest->setNonce(rand->GetValue(0, std::numeric_limits<uint32_t>::max()));
      // interest->setInterestLifetime(ndn::time::seconds(1));
      // interest->setMustBeFresh(1);
      // NS_LOG_DEBUG("Sending Interest packet for " << *interest);

      // m_transmittedInterests(interest, this, m_face);

      // m_appLink->onReceiveInterest(*interest);
    // int evillen = int(evillenchr);
    // std::cout << evillen << std::endl;
    // char evil[evillen + 1];
    // read(fd_in, evil, evillen);
    // evil[evillen] = '\0';
    // std::cout << evil << std::endl;



  } else if (type == 3){

  }
}

void
CtrlConsumer::OnNack(std::shared_ptr<const lp::Nack> nack)
{
  NS_LOG_DEBUG("Get NACK from " << nack->getInterest().getName());
}

} // namespace ndn
} // namespace ns3
