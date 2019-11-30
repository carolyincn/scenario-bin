// ctrlconsumer.hpp

#ifndef CTRLCONSUMER_H_
#define CTRLCONSUMER_H_

#include "ns3/ndnSIM/apps/ndn-app.hpp"
#include <fstream>
using namespace std;

namespace ns3 {
namespace ndn {

class CtrlConsumer : public ndn::App {
public:
  // register NS-3 type "CustomApp"
  static TypeId
  GetTypeId();

  // (overridden from ndn::App) Processing upon start of the application
  virtual void
  StartApplication();

  // (overridden from ndn::App) Processing when application is stopped
  virtual void
  StopApplication();

  // (overridden from ndn::App) Callback that will be called when Interest arrives
  // virtual void
  // OnInterest(std::shared_ptr<const ndn::Interest> interest);

  // (overridden from ndn::App) Callback that will be called when Data arrives
  virtual void
  OnData(std::shared_ptr<const ndn::Data> contentObject);

  virtual void
  OnNack(std::shared_ptr<const lp::Nack> nack);

private:
  void
  SendInterest();

  int fd_out; // write pipe
  int fd_in; // read pipe

protected:
  EventId m_nextSend;
  double m_frequency;
  Name m_prefix;
  bool abnormal = 0; // 0 normal, 1 abnormal



};

} // namespace ndn
} // namespace ns3

#endif // CTRLCONSUMER_H_
