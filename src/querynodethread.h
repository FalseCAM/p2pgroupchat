#ifndef QUERYNODETHREAD_H
#define QUERYNODETHREAD_H

#include <p2p/Concurrency.hpp>
#include <p2p/Kademlia.hpp>
#include <p2p/Root.hpp>

#include "Logger.hpp"

class QueryNodeThread: public p2p::Runnable {
    public:
        QueryNodeThread(Logger& logger,
                const p2p::Root::NodeId& myId, p2p::Kademlia::BucketSet<p2p::Root::NodeId>& dhtBucketSet,
                p2p::Root::Core::Service& coreService, p2p::Root::DHT::Service& dhtService,
                p2p::MessageQueue<p2p::Root::NodePair>& messageQueue);

        void run();

        void cancel();

    private:
        Logger& logger_;
        p2p::Root::NodeId myId_;
        p2p::Kademlia::BucketSet<p2p::Root::NodeId>& dhtBucketSet_;
        p2p::Root::Core::Service& coreService_;
        p2p::Root::DHT::Service& dhtService_;
        p2p::MessageQueue<p2p::Root::NodePair>& messageQueue_;
        p2p::Event::Signal signal_;

};

#endif // QUERYNODETHREAD_H
