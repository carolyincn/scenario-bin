// RouterProducer.hpp

#ifndef ROUTERPRODUCER_H_
#define ROUTERPRODUCER_H_

#include "ns3/ndnSIM/apps/ndn-app.hpp"
#include "ns3/ndnSIM/model/ndn-common.hpp"
#include "util.hpp"

// vector<vector<string>> vs(7, vector<string>());

namespace ns3 {
namespace ndn {

class RouterProducer : public ndn::App {
public:
  static TypeId
  GetTypeId();

  RouterProducer();

  // Receive all Interests but do nothing in response
  void
  OnInterest(std::shared_ptr<const ndn::Interest> interest);

protected:
  // inherited from Application base class.
  virtual void
  StartApplication();

  virtual void
  StopApplication();

private:
  Name m_prefix;
  // Name m_postfix;
  uint32_t m_virtualPayloadSize;
  Time m_freshness;

  uint32_t m_signature;
  Name m_keyLocator;
  int temp_inInterest = 0;
  int temp_outInterest = 0;
  int temp_inNack = 0;
  int temp_outNack = 0;
  int temp_csHit = 0;
  int temp_csMiss= 0;

  int fd = 0; // pipe


};

} // namespace ndn
} // namespace ns3

#endif // ROUTERPRODUCER_H_