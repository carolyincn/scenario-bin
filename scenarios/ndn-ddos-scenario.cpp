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
  // Config::SetDefault("ns3::QueueBase::MaxSize", StringValue("20p"));


  // Read optional command-line parameters (e.g., enable visualizer with ./waf --run=<> --visualize
  CommandLine cmd;
  cmd.Parse(argc, argv);

  AnnotatedTopologyReader topologyReader("", 1);
  topologyReader.SetFileName("/home/yin/Desktop/ndn/ndnSIM/scenario-bin/scenarios/topo-large.txt");
  topologyReader.Read();

  NodeContainer routers;
  NodeContainer hosts;
  NodeContainer controllers;
  NodeContainer producers;

  int rnum = 0;
  int hnum = 0;
  int pnum = 0;

  for_each (NodeList::Begin (), NodeList::End (), [&] (Ptr<Node> node) {
    if (Names::FindName (node).compare (0, 1, "R")==0)
      {
        routers.Add (node);
        rnum ++;
      }
    if (Names::FindName (node).compare (0, 1, "H")==0)
      {
        hosts.Add (node);
        hnum ++;
      }
    if (Names::FindName (node).compare (0, 1, "C")==0)
      {
        controllers.Add (node);
      }
    if (Names::FindName (node).compare (0, 1, "P")==0)
      {
        producers.Add (node);
        pnum ++;
      }
  });

  NodeContainer evilNodes;
  NodeContainer goodNodes;

  // set< Ptr<Node> > producers;
  set< Ptr<Node> > evils;
  set< Ptr<Node> > angels;

  int badCount = 7;
  while (evils.size () < badCount){
    Ptr<UniformRandomVariable> rand = CreateObject<UniformRandomVariable>();
    Ptr<Node> node = hosts.Get (rand->GetInteger(0, hnum - 1));
    if (evils.find (node) != evils.end ())
      continue;
    evils.insert (node);
    string name = Names::FindName(node);
    Names::Rename (name, "evil-"+name);
  }
  // evils.insert(hosts.Get(1));
  // evils.insert(hosts.Get(3));
  // evils.insert(hosts.Get(5));
  // evils.insert(hosts.Get(7));
  // evils.insert(hosts.Get(9));
  // evils.insert(hosts.Get(11));

  for_each(hosts.Begin(), hosts.End(), [&] (Ptr<Node> node){
    if (evils.find(node) == evils.end()){
      angels.insert(node);
      string name = Names::FindName(node);
      Names::Rename (name, "good-" + name);
    }
  });

  for_each (evils.begin (), evils.end (), [&] (Ptr<Node> node){
    string name = Names::FindName (node);
    cout << name << " ";
    evilNodes.Add (node);
  });
  cout << endl;

  for_each (angels.begin (), angels.end (), [&] (Ptr<Node> node){
    string name = Names::FindName (node);
    cout << name << " ";
    goodNodes.Add (node);
  });
  cout << endl;


  // Install NDN stack on all nodes
  ndn::StackHelper ndnHelper;
  ndnHelper.setCsSize(1);
  ndnHelper.InstallAll();

  ndn::StrategyChoiceHelper::Install(routers, "/", "/localhost/nfd/strategy/my-strategy");


  ndn::GlobalRoutingHelper ndnGlobalRoutingHelper2;
  ndnGlobalRoutingHelper2.Install(routers);
  ndnGlobalRoutingHelper2.Install(hosts);
  ndnGlobalRoutingHelper2.Install(producers);
  ndnGlobalRoutingHelper2.AddOrigin("/p", producers.Get(0));
  ndnGlobalRoutingHelper2.CalculateRoutes();

  PointToPointHelper p2p;
  for(int i = 0; i < rnum; i ++){
    string name = Names::FindName(routers.Get(i));
    // atoi(name.substr((name.find("-") + 1)));
    p2p.Install(controllers.Get(atoi(name.substr((name.find("-") + 1)).c_str()) - 1), routers.Get(i));
    
  }

  ndnHelper.UpdateAll();

  ndn::GlobalRoutingHelper ndnGlobalRoutingHelper1;
  ndnGlobalRoutingHelper1.Install(controllers);
  ndnGlobalRoutingHelper1.Install(routers);
  for(int i = 0; i < rnum; i ++){
    ndnGlobalRoutingHelper1.AddOrigin("/r" + to_string(i + 1), routers.Get(i));
  }
  ndnGlobalRoutingHelper1.CalculateRoutes();

  ndn::AppHelper evilAppHelper("ns3::ndn::ConsumerCbr"); // 
  evilAppHelper.SetAttribute("Frequency", StringValue("200"));
  evilAppHelper.SetAttribute("Randomize", StringValue("uniform"));
  for (NodeContainer::Iterator node = evilNodes.Begin (); node != evilNodes.End (); node++){
    ApplicationContainer evilApp;
    evilAppHelper.SetPrefix ("/p/evil");
    evilApp.Add (evilAppHelper.Install (*node));
    evilApp.Start (Seconds (30.0));
    evilApp.Stop (Seconds (150.0));
  }

  ndn::AppHelper goodAppHelper("ns3::ndn::ConsumerZipfMandelbrot"); // ConsumerZipfMandelbrot
  // evilAppHelper.SetAttribute("Randomize", StringValue("uniform"));
  goodAppHelper.SetAttribute("Frequency", StringValue("10"));
  for (NodeContainer::Iterator node = goodNodes.Begin (); node != goodNodes.End (); node++){
    ApplicationContainer goodApp;
    goodAppHelper.SetPrefix ("/p/good");
    goodApp.Add (goodAppHelper.Install (*node));
  }

  ndn::AppHelper producerApp("ns3::ndn::Producer");
  // Producer will reply to all requests starting with /prefix
  producerApp.SetPrefix("/p/good");
  producerApp.SetAttribute("PayloadSize", StringValue("1024"));
  producerApp.Install(producers.Get(0)); // last node

  ndn::AppHelper routerHelper("RouterProducer");
  routerHelper.SetAttribute("PayloadSize", StringValue("1"));
  for(int i = 0; i < rnum; i ++){
    ApplicationContainer routerApp;
    routerHelper.SetPrefix ("/r" + to_string(i + 1));
    routerApp.Add (routerHelper.Install (routers.Get(i)));
  }

  ndn::AppHelper ctrlHelper("CtrlConsumer");
  ctrlHelper.SetAttribute("Frequency", StringValue("1"));
  for(int i = 0; i < rnum; i ++){
    ApplicationContainer ctrlApp;
    ctrlHelper.SetPrefix ("/r" + to_string(i + 1));
    string name = Names::FindName(routers.Get(i));
    // atoi(name.substr((name.find("-") + 1)));
    ctrlApp.Add(ctrlHelper.Install(controllers.Get(atoi(name.substr((name.find("-") + 1)).c_str()) - 1)));
  }


  Simulator::Stop(Seconds(180.0));
  ndn::L3RateTracer::InstallAll("rate-trace.txt", Seconds(1.0));
  // ndn::CsTracer::InstallAll("cs-trace.txt", Seconds(1));

  Simulator::Run();
  Simulator::Destroy();
  // outfile.close();

  return 0;
}

} // namespace ns3

int
main(int argc, char* argv[])
{
  return ns3::main(argc, argv);
}
