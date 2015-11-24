#include "kademliarpcsocket.h"

KademliaRPCSocket::KademliaRPCSocket(p2p::Root::DHT::Service& dhtService, Logger& logger)
    : dhtService_(dhtService), logger_(logger) { }

p2p::RPC::Operation<std::vector<p2p::Root::NodeId>> KademliaRPCSocket::performFind(const p2p::Root::NodeId& destId, const p2p::Root::NodeId& searchId) {
    logger_.log(STR("Kademlia querying node '%s'.", destId.hexString().c_str()));
    return p2p::RPC::Chain(dhtService_.getNearestNodes(destId, searchId),
        [] (std::vector<p2p::Root::NodeInfo> nodeList) {
            std::vector<p2p::Root::NodeId> idList;
            for (const auto& nodeInfo: nodeList) {
                idList.push_back(nodeInfo.id);
            }
            return idList;
        });
}
