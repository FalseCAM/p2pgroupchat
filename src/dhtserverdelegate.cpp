#include "dhtserverdelegate.h"

DHTServerDelegate::DHTServerDelegate(p2p::Kademlia::BucketSet<p2p::Root::NodeId>& dhtBucketSet, p2p::Root::NodeDatabase& nodeDatabase)
    : dhtBucketSet_(dhtBucketSet), nodeDatabase_(nodeDatabase) { }

std::vector<p2p::Root::NodeInfo> DHTServerDelegate::getNearestNodes(const p2p::Root::NodeId& targetId) {
    const auto idList = dhtBucketSet_.getNearest(targetId);
    std::vector<p2p::Root::NodeInfo> nodeList;
    for (const auto& id: idList) {
        nodeList.push_back(nodeDatabase_.nodeEntry(id).toNodeInfo());
    }
    return nodeList;
}

void DHTServerDelegate::subscribe(const p2p::Root::NodeId& targetId, const p2p::Root::NodeInfo& nodeInfo) {
    (void) targetId;
    (void) nodeInfo;
}

std::vector<p2p::Root::NodeInfo> DHTServerDelegate::getSubscribers(const p2p::Root::NodeId& targetId) {
    (void) targetId;
    return {};
}
