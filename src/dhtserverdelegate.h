#ifndef DHTSERVERDELEGATE_H
#define DHTSERVERDELEGATE_H

#include <p2p/Kademlia.hpp>
#include <p2p/Root.hpp>

class DHTServerDelegate: public p2p::Root::DHT::ServerDelegate {
    public:
        DHTServerDelegate(p2p::Kademlia::BucketSet<p2p::Root::NodeId>& dhtBucketSet, p2p::Root::NodeDatabase& nodeDatabase);

        std::vector<p2p::Root::NodeInfo> getNearestNodes(const p2p::Root::NodeId& targetId);

        void subscribe(const p2p::Root::NodeId& targetId, const p2p::Root::NodeInfo& nodeInfo);

        std::vector<p2p::Root::NodeInfo> getSubscribers(const p2p::Root::NodeId& targetId);

    private:
        p2p::Kademlia::BucketSet<p2p::Root::NodeId>& dhtBucketSet_;
        p2p::Root::NodeDatabase& nodeDatabase_;
};

#endif // DHTSERVERDELEGATE_H
