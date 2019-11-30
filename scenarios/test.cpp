// ndn-ddos-scenario.cpp

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/ndnSIM-module.h"
#include "ns3/point-to-point-module.h"
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
#include "ns3/ndnSIM/helper/ndn-scenario-helper.hpp"
#include "ns3/ndnSIM/model/ndn-l3-protocol.hpp"
#include "ns3/names.h"
#include "ns3/net-device.h"

namespace ns3 {
// ofstream outfile;
int
main(int argc, char* argv[])
{
  // setting default parameters for PointToPoint links and channels
  Config::SetDefault("ns3::PointToPointNetDevice::DataRate", StringValue("1Mbps"));
  Config::SetDefault("ns3::PointToPointChannel::Delay", StringValue("10ms"));
  Config::SetDefault("ns3::QueueBase::MaxPackets", UintegerValue(20));


  // Read optional command-line parameters (e.g., enable visualizer with ./waf --run=<> --visualize
  CommandLine cmd;
  cmd.Parse(argc, argv);

  // Creating nodes
  // Ptr<Node> node = CreateObject<Node>();


  // Creating nodes

  AnnotatedTopologyReader topologyReader("", 1);
  topologyReader.SetFileName("/home/yin/Desktop/ndn/ndnSIM/scenario-bin/scenarios/topo-large.txt");
  topologyReader.Read();

  Ptr<Node> c1 = Names::Find<Node>("C1");
  Ptr<Node> c2 = Names::Find<Node>("C2");
  Ptr<Node> c3 = Names::Find<Node>("C3");
  Ptr<Node> p = Names::Find<Node>("P");

  Ptr<Node> r1 = Names::Find<Node>("R1-1");
  Ptr<Node> r2 = Names::Find<Node>("R2-1");
  Ptr<Node> r3 = Names::Find<Node>("R3-1");
  Ptr<Node> r4 = Names::Find<Node>("R4-1");
  Ptr<Node> r5 = Names::Find<Node>("R5-2");
  Ptr<Node> r6 = Names::Find<Node>("R6-2");
  Ptr<Node> r7 = Names::Find<Node>("R7-2");
  Ptr<Node> r8 = Names::Find<Node>("R8-2");
  Ptr<Node> r9 = Names::Find<Node>("R9-1");
  Ptr<Node> r10 = Names::Find<Node>("R10-1");
  Ptr<Node> r11 = Names::Find<Node>("R11-2");
  Ptr<Node> r12 = Names::Find<Node>("R12-2");
  Ptr<Node> r13 = Names::Find<Node>("R13-1");
  Ptr<Node> r14 = Names::Find<Node>("R14-2");
  Ptr<Node> r15 = Names::Find<Node>("R15-3");
  Ptr<Node> r16 = Names::Find<Node>("R16-3");

  Ptr<Node> h1 = Names::Find<Node>("H1");
  Ptr<Node> h2 = Names::Find<Node>("H2");
  Ptr<Node> h3 = Names::Find<Node>("H3");
  Ptr<Node> h4 = Names::Find<Node>("H4");
  Ptr<Node> h5 = Names::Find<Node>("H5");
  Ptr<Node> h6 = Names::Find<Node>("H6");
  Ptr<Node> h7 = Names::Find<Node>("H7");
  Ptr<Node> h8 = Names::Find<Node>("H8");
  Ptr<Node> h9 = Names::Find<Node>("H9");
  Ptr<Node> h10 = Names::Find<Node>("H10");
  Ptr<Node> h11 = Names::Find<Node>("H11");
  Ptr<Node> h12 = Names::Find<Node>("H12");
  Ptr<Node> h13 = Names::Find<Node>("H13");
  Ptr<Node> h14 = Names::Find<Node>("H14");
  Ptr<Node> h15 = Names::Find<Node>("H15");
  Ptr<Node> h16 = Names::Find<Node>("H16");
  

  // Install NDN stack on all nodes
  ndn::StackHelper ndnHelper;
  ndnHelper.setCsSize(1);
  ndnHelper.InstallAll();

  // ndn::StrategyChoiceHelper::InstallAll("/", "/localhost/nfd/strategy/my-strategy");

  ndn::StrategyChoiceHelper::Install(r1, "/", "/localhost/nfd/strategy/my-strategy");
  ndn::StrategyChoiceHelper::Install(r2, "/", "/localhost/nfd/strategy/my-strategy");
  ndn::StrategyChoiceHelper::Install(r3, "/", "/localhost/nfd/strategy/my-strategy");
  ndn::StrategyChoiceHelper::Install(r4, "/", "/localhost/nfd/strategy/my-strategy");
  ndn::StrategyChoiceHelper::Install(r5, "/", "/localhost/nfd/strategy/my-strategy");
  ndn::StrategyChoiceHelper::Install(r6, "/", "/localhost/nfd/strategy/my-strategy");
  ndn::StrategyChoiceHelper::Install(r7, "/", "/localhost/nfd/strategy/my-strategy");
  ndn::StrategyChoiceHelper::Install(r8, "/", "/localhost/nfd/strategy/my-strategy");
  ndn::StrategyChoiceHelper::Install(r9, "/", "/localhost/nfd/strategy/my-strategy");
  ndn::StrategyChoiceHelper::Install(r10, "/", "/localhost/nfd/strategy/my-strategy");
  ndn::StrategyChoiceHelper::Install(r11, "/", "/localhost/nfd/strategy/my-strategy");
  ndn::StrategyChoiceHelper::Install(r12, "/", "/localhost/nfd/strategy/my-strategy");
  ndn::StrategyChoiceHelper::Install(r13, "/", "/localhost/nfd/strategy/my-strategy");
  ndn::StrategyChoiceHelper::Install(r14, "/", "/localhost/nfd/strategy/my-strategy");
  ndn::StrategyChoiceHelper::Install(r15, "/", "/localhost/nfd/strategy/my-strategy");
  ndn::StrategyChoiceHelper::Install(r16, "/", "/localhost/nfd/strategy/my-strategy");

  ndn::GlobalRoutingHelper ndnGlobalRoutingHelper;
  ndnGlobalRoutingHelper.Install(r1);
  ndnGlobalRoutingHelper.Install(r2);
  ndnGlobalRoutingHelper.Install(r3);
  ndnGlobalRoutingHelper.Install(r4);
  ndnGlobalRoutingHelper.Install(r5);
  ndnGlobalRoutingHelper.Install(r6);
  ndnGlobalRoutingHelper.Install(r7);
  ndnGlobalRoutingHelper.Install(r8);
  ndnGlobalRoutingHelper.Install(r9);
  ndnGlobalRoutingHelper.Install(r10);
  ndnGlobalRoutingHelper.Install(r11);
  ndnGlobalRoutingHelper.Install(r12);
  ndnGlobalRoutingHelper.Install(r13);
  ndnGlobalRoutingHelper.Install(r14);
  ndnGlobalRoutingHelper.Install(r15);
  ndnGlobalRoutingHelper.Install(r16);
  ndnGlobalRoutingHelper.Install(h1);
  ndnGlobalRoutingHelper.Install(h2);
  ndnGlobalRoutingHelper.Install(h3);
  ndnGlobalRoutingHelper.Install(h4);
  ndnGlobalRoutingHelper.Install(h5);
  ndnGlobalRoutingHelper.Install(h6);
  ndnGlobalRoutingHelper.Install(h7);
  ndnGlobalRoutingHelper.Install(h8);
  ndnGlobalRoutingHelper.Install(h9);
  ndnGlobalRoutingHelper.Install(h10);
  ndnGlobalRoutingHelper.Install(h11);
  ndnGlobalRoutingHelper.Install(h12);
  ndnGlobalRoutingHelper.Install(h13);
  ndnGlobalRoutingHelper.Install(h14);
  ndnGlobalRoutingHelper.Install(h15);
  ndnGlobalRoutingHelper.Install(h16);
  ndnGlobalRoutingHelper.Install(p);
  ndnGlobalRoutingHelper.AddOrigin("/p", p);
  ndnGlobalRoutingHelper.CalculateRoutes();

  PointToPointHelper p2p;
  p2p.Install(c1, r1);
  p2p.Install(c1, r2);
  p2p.Install(c1, r3);
  p2p.Install(c1, r4);
  p2p.Install(c1, r9);
  p2p.Install(c1, r10);
  p2p.Install(c1, r13);
  p2p.Install(c2, r5);
  p2p.Install(c2, r6);
  p2p.Install(c2, r7);
  p2p.Install(c2, r8);
  p2p.Install(c2, r11);
  p2p.Install(c2, r12);
  p2p.Install(c2, r14);
  p2p.Install(c3, r15);
  p2p.Install(c3, r16);

  ndnHelper.UpdateAll();

  ndn::GlobalRoutingHelper ndnGlobalRoutingHelper1;
  ndnGlobalRoutingHelper1.Install(c1);
  ndnGlobalRoutingHelper1.Install(c2);
  ndnGlobalRoutingHelper1.Install(c3);
  ndnGlobalRoutingHelper1.Install(r1);
  ndnGlobalRoutingHelper1.Install(r2);
  ndnGlobalRoutingHelper1.Install(r3);
  ndnGlobalRoutingHelper1.Install(r4);
  ndnGlobalRoutingHelper1.Install(r5);
  ndnGlobalRoutingHelper1.Install(r6);
  ndnGlobalRoutingHelper1.Install(r7);
  ndnGlobalRoutingHelper1.Install(r8);
  ndnGlobalRoutingHelper1.Install(r9);
  ndnGlobalRoutingHelper1.Install(r10);
  ndnGlobalRoutingHelper1.Install(r11);
  ndnGlobalRoutingHelper1.Install(r12);
  ndnGlobalRoutingHelper1.Install(r13);
  ndnGlobalRoutingHelper1.Install(r14);
  ndnGlobalRoutingHelper1.Install(r15);
  ndnGlobalRoutingHelper1.Install(r16);
  ndnGlobalRoutingHelper1.AddOrigin("/r1", r1);
  ndnGlobalRoutingHelper1.AddOrigin("/r2", r2);
  ndnGlobalRoutingHelper1.AddOrigin("/r3", r3);
  ndnGlobalRoutingHelper1.AddOrigin("/r4", r4);
  ndnGlobalRoutingHelper1.AddOrigin("/r5", r5);
  ndnGlobalRoutingHelper1.AddOrigin("/r6", r6);
  ndnGlobalRoutingHelper1.AddOrigin("/r7", r7);
  ndnGlobalRoutingHelper1.AddOrigin("/r8", r8);
  ndnGlobalRoutingHelper1.AddOrigin("/r9", r9);
  ndnGlobalRoutingHelper1.AddOrigin("/r10", r10);
  ndnGlobalRoutingHelper1.AddOrigin("/r11", r11);
  ndnGlobalRoutingHelper1.AddOrigin("/r12", r12);
  ndnGlobalRoutingHelper1.AddOrigin("/r13", r13);
  ndnGlobalRoutingHelper1.AddOrigin("/r14", r14);
  ndnGlobalRoutingHelper1.AddOrigin("/r15", r15);
  ndnGlobalRoutingHelper1.AddOrigin("/r16", r16);
  ndnGlobalRoutingHelper1.CalculateRoutes();

  // ndn::FibHelper fibHelper;
  // std::string prefix("/");
  // fibHelper.AddRoute(r1, prefix, r9, 0);
  // fibHelper.AddRoute(r2, prefix, r9, 0);

  NodeContainer goodNodes;
  goodNodes.Add(h1);
  goodNodes.Add(h3);
  goodNodes.Add(h5);
  goodNodes.Add(h7);
  goodNodes.Add(h9);
  goodNodes.Add(h11);
  goodNodes.Add(h13);
  goodNodes.Add(h15);
  goodNodes.Add(h16);
  // goodNodes.Add(h14);
  // goodNodes.Add(h6);
  // goodNodes.Add(h4);
  // goodNodes.Add(h12);
  // goodNodes.Add(h10);
  // goodNodes.Add(h8);

  NodeContainer evilNodes;
  evilNodes.Add(h2);
  evilNodes.Add(h4);
  evilNodes.Add(h6);
  evilNodes.Add(h8);
  evilNodes.Add(h10);
  evilNodes.Add(h12);
  evilNodes.Add(h14);


  // normal
  ndn::AppHelper h1_app("ns3::ndn::ConsumerZipfMandelbrot"); //ConsumerZipfMandelbrot
  h1_app.SetPrefix("/p/good");
  h1_app.SetAttribute("Frequency", StringValue("50"));
  for (NodeContainer::Iterator node = goodNodes.Begin (); node != goodNodes.End (); node++) {
    h1_app.Install (*node);
  }

  ApplicationContainer evilApp;
  ndn::AppHelper h2_app("ns3::ndn::ConsumerCbr"); //ConsumerZipfMandelbrot
  h2_app.SetPrefix("/p/evil");
  h2_app.SetAttribute("Frequency", StringValue("200"));
  for (NodeContainer::Iterator node = evilNodes.Begin (); node != evilNodes.End (); node++) {
    evilApp.Add (h2_app.Install (*node));
    evilApp.Start (Seconds (10.0));
    evilApp.Stop (Seconds (20.0));
  }

  // h1_app,Install()
  // h1_app.Install(h1);
  // // h1_app.Install(h2);
  // h1_app.Install(h3);
  // h1_app.Install(h4);
  // h1_app.Install(h5);
  // h1_app.Install(h6);
  // h1_app.Install(h7);
  // h1_app.Install(h8);
  // h1_app.Install(h9);
  // h1_app.Install(h10);
  // h1_app.Install(h11);
  // h1_app.Install(h12);
  // h1_app.Install(h13);
  // h1_app.Install(h14);
  // h1_app.Install(h15);
  // h1_app.Install(h16);

  // ndn::AppHelper h3_app("ns3::ndn::ConsumerCbr");
  // h3_app.SetPrefix("/p/good");
  // h3_app.SetAttribute("Frequency", StringValue("10"));
  // h3_app.Install(h3);

  // evil
  // ApplicationContainer evilApps;
  // ndn::AppHelper h2_app("ns3::ndn::ConsumerCbr");
  // h2_app.SetPrefix("/p/evil");
  // h2_app.SetAttribute("Frequency", StringValue("100"));
  // for (NodeContainer::Iterator node = evilNodes.Begin (); node != evilNodes.End (); node++) {
  //     ApplicationContainer evilApp;
  //     evilApp.Add (h2_app.Install (*node));
  //     evilApp.Start (Seconds (40.0));
  //     evilApp.Stop (Seconds (90.0));
  // }

  // h2_app.SetAttribute("Frequency", StringValue("10"));
  // for (NodeContainer::Iterator node = evilNodes.Begin (); node != evilNodes.End (); node++) {
  //     ApplicationContainer evilApp;
  //     evilApp.Add (h2_app.Install (*node));
  //     evilApp.Start (Seconds (30.0));
  //     evilApp.Stop (Seconds (31.0));
  // }

  // h2_app.SetAttribute("Frequency", StringValue("20"));
  // for (NodeContainer::Iterator node = evilNodes.Begin (); node != evilNodes.End (); node++) {
  //     ApplicationContainer evilApp;
  //     evilApp.Add (h2_app.Install (*node));
  //     evilApp.Start (Seconds (31.0));
  //     evilApp.Stop (Seconds (32.0));
  // }

  // h2_app.SetAttribute("Frequency", StringValue("30"));
  // for (NodeContainer::Iterator node = evilNodes.Begin (); node != evilNodes.End (); node++) {
  //     ApplicationContainer evilApp;
  //     evilApp.Add (h2_app.Install (*node));
  //     evilApp.Start (Seconds (32.0));
  //     evilApp.Stop (Seconds (33.0));
  // }

  // h2_app.SetAttribute("Frequency", StringValue("40"));
  // for (NodeContainer::Iterator node = evilNodes.Begin (); node != evilNodes.End (); node++) {
  //     ApplicationContainer evilApp;
  //     evilApp.Add (h2_app.Install (*node));
  //     evilApp.Start (Seconds (33.0));
  //     evilApp.Stop (Seconds (34.0));
  // }

  // h2_app.SetAttribute("Frequency", StringValue("50"));
  // for (NodeContainer::Iterator node = evilNodes.Begin (); node != evilNodes.End (); node++) {
  //     ApplicationContainer evilApp;
  //     evilApp.Add (h2_app.Install (*node));
  //     evilApp.Start (Seconds (34.0));
  //     evilApp.Stop (Seconds (40.0));
  // }
  // evilApps.Add(h2_app.Install(h2));
  // evilApps.Start(Seconds(40.0));
  // evilApps.Stop(Seconds(90.0));

  // evilApps.Add(h2_app.Install(h4));
  // evilApps.Start(Seconds(40.0));
  // evilApps.Stop(Seconds(90.0));

  // evilApps.Add(h2_app.Install(h6));
  // evilApps.Start(Seconds(40.0));
  // evilApps.Stop(Seconds(90.0));

  // evilApps.Add(h2_app.Install(h8));
  // evilApps.Start(Seconds(40.0));
  // evilApps.Stop(Seconds(90.0));

  // evilApps.Add(h2_app.Install(h10));
  // evilApps.Start(Seconds(40.0));
  // evilApps.Stop(Seconds(90.0));

  // evilApps.Add(h2_app.Install(h12));
  // evilApps.Start(Seconds(40.0));
  // evilApps.Stop(Seconds(90.0));

  // evilApps.Add(h2_app.Install(h14));
  // evilApps.Start(Seconds(40.0));
  // evilApps.Stop(Seconds(90.0));

  
  // ndn::AppHelper h3_app("ns3::ndn::ConsumerCbr");
  // h3_app.SetPrefix("/p/evil");
  // h3_app.SetAttribute("Frequency", StringValue("10"));
  // h3_app.Install(h2);
  // h3_app.Install(h4);
  // h3_app.Install(h6);
  // h3_app.Install(h8);
  // h3_app.Install(h10);
  // h3_app.Install(h12);
  // h3_app.Install(h14);

  
  // consumer on controllers
  ndn::AppHelper c1_r1_app("CtrlConsumer");
  c1_r1_app.SetPrefix("/r1");
  c1_r1_app.SetAttribute("Frequency", StringValue("1"));
  c1_r1_app.Install(c1);
  
  ndn::AppHelper c1_r2_app("CtrlConsumer");
  c1_r2_app.SetPrefix("/r2");
  c1_r2_app.SetAttribute("Frequency", StringValue("1"));
  c1_r2_app.Install(c1);  

  ndn::AppHelper c1_r3_app("CtrlConsumer");
  c1_r3_app.SetPrefix("/r3");
  c1_r3_app.SetAttribute("Frequency", StringValue("1"));
  c1_r3_app.Install(c1); 

  ndn::AppHelper c1_r4_app("CtrlConsumer");
  c1_r4_app.SetPrefix("/r4");
  c1_r4_app.SetAttribute("Frequency", StringValue("1"));
  c1_r4_app.Install(c1); 

  ndn::AppHelper c1_r5_app("CtrlConsumer");
  c1_r5_app.SetPrefix("/r5");
  c1_r5_app.SetAttribute("Frequency", StringValue("1"));
  c1_r5_app.Install(c2);

  ndn::AppHelper c1_r6_app("CtrlConsumer");
  c1_r6_app.SetPrefix("/r6");
  c1_r6_app.SetAttribute("Frequency", StringValue("1"));
  c1_r6_app.Install(c2);

  ndn::AppHelper c1_r7_app("CtrlConsumer");
  c1_r7_app.SetPrefix("/r7");
  c1_r7_app.SetAttribute("Frequency", StringValue("1"));
  c1_r7_app.Install(c2);

  ndn::AppHelper c1_r8_app("CtrlConsumer");
  c1_r8_app.SetPrefix("/r8");
  c1_r8_app.SetAttribute("Frequency", StringValue("1"));
  c1_r8_app.Install(c2);

  ndn::AppHelper c2_r9_app("CtrlConsumer");
  c2_r9_app.SetPrefix("/r9");
  c2_r9_app.SetAttribute("Frequency", StringValue("1"));
  c2_r9_app.Install(c1);

  ndn::AppHelper c2_r10_app("CtrlConsumer");
  c2_r10_app.SetPrefix("/r10");
  c2_r10_app.SetAttribute("Frequency", StringValue("1"));
  c2_r10_app.Install(c1);

  ndn::AppHelper c2_r11_app("CtrlConsumer");
  c2_r11_app.SetPrefix("/r11");
  c2_r11_app.SetAttribute("Frequency", StringValue("1"));
  c2_r11_app.Install(c2);

  ndn::AppHelper c2_r12_app("CtrlConsumer");
  c2_r12_app.SetPrefix("/r12");
  c2_r12_app.SetAttribute("Frequency", StringValue("1"));
  c2_r12_app.Install(c2);

  ndn::AppHelper c2_r13_app("CtrlConsumer");
  c2_r13_app.SetPrefix("/r13");
  c2_r13_app.SetAttribute("Frequency", StringValue("1"));
  c2_r13_app.Install(c1);

  ndn::AppHelper c2_r14_app("CtrlConsumer");
  c2_r14_app.SetPrefix("/r14");
  c2_r14_app.SetAttribute("Frequency", StringValue("1"));
  c2_r14_app.Install(c2);

  ndn::AppHelper c3_r15_app("CtrlConsumer");
  c3_r15_app.SetPrefix("/r15");
  c3_r15_app.SetAttribute("Frequency", StringValue("1"));
  c3_r15_app.Install(c3);

  ndn::AppHelper c3_r16_app("CtrlConsumer");
  c3_r16_app.SetPrefix("/r16");
  c3_r16_app.SetAttribute("Frequency", StringValue("1"));
  c3_r16_app.Install(c3);

  // producer on routers
  ndn::AppHelper r1_app("RouterProducer");
  r1_app.SetPrefix("/r1");
  r1_app.Install(r1);

  ndn::AppHelper r2_app("RouterProducer");
  r2_app.SetPrefix("/r2");
  r2_app.Install(r2);

  ndn::AppHelper r3_app("RouterProducer");
  r3_app.SetPrefix("/r3");
  r3_app.Install(r3);

  ndn::AppHelper r4_app("RouterProducer");
  r4_app.SetPrefix("/r4");
  r4_app.Install(r4);

  ndn::AppHelper r5_app("RouterProducer");
  r5_app.SetPrefix("/r5");
  r5_app.Install(r5);

  ndn::AppHelper r6_app("RouterProducer");
  r6_app.SetPrefix("/r6");
  r6_app.Install(r6);

  ndn::AppHelper r7_app("RouterProducer");
  r7_app.SetPrefix("/r7");
  r7_app.Install(r7);

  ndn::AppHelper r8_app("RouterProducer");
  r8_app.SetPrefix("/r8");
  r8_app.Install(r8);

  ndn::AppHelper r9_app("RouterProducer");
  r9_app.SetPrefix("/r9");
  r9_app.Install(r9);

  ndn::AppHelper r10_app("RouterProducer");
  r10_app.SetPrefix("/r10");
  r10_app.Install(r10);

  ndn::AppHelper r11_app("RouterProducer");
  r11_app.SetPrefix("/r11");
  r11_app.Install(r11);

  ndn::AppHelper r12_app("RouterProducer");
  r12_app.SetPrefix("/r12");
  r12_app.Install(r12);

  ndn::AppHelper r13_app("RouterProducer");
  r13_app.SetPrefix("/r13");
  r13_app.Install(r13);

  ndn::AppHelper r14_app("RouterProducer");
  r14_app.SetPrefix("/r14");
  r14_app.Install(r14);

  ndn::AppHelper r15_app("RouterProducer");
  r15_app.SetPrefix("/r15");
  r15_app.Install(r15);

  ndn::AppHelper r16_app("RouterProducer");
  r16_app.SetPrefix("/r16");
  r16_app.Install(r16);

  // Producer
  ndn::AppHelper p_app("ns3::ndn::Producer");
  // Producer will reply to all requests starting with /prefix
  p_app.SetPrefix("/p/good");
  p_app.SetAttribute("PayloadSize", StringValue("1024"));
  p_app.Install(p); // last node  

  Simulator::Stop(Seconds(20.0));
  ndn::L3RateTracer::InstallAll("rate-trace.txt", Seconds(1.0));
  // ndn::CsTracer::InstallAll("cs-trace.txt", Seconds(1));

  Simulator::Run();
  Simulator::Destroy();

  return 0;
}

} // namespace ns3

int
main(int argc, char* argv[])
{
  return ns3::main(argc, argv);
}
