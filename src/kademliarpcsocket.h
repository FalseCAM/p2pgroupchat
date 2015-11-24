#ifndef KADEMLIARPCSOCKET_H
#define KADEMLIARPCSOCKET_H

#include <p2p/Kademlia.hpp>
#include <p2p/Root.hpp>

#include "Logger.hpp"

class KademliaRPCSocket: public p2p::Kademlia::RPCSocket<p2p::Root::NodeId> {
    public:
        KademliaRPCSocket(p2p::Root::DHT::Service& dhtService, Logger& logger);

        p2p::RPC::Operation<std::vector<p2p::Root::NodeId>> performFind(const p2p::Root::NodeId& destId, const p2p::Root::NodeId& searchId);

    private:
        p2p::Root::DHT::Service& dhtService_;
        Logger& logger_;

};

#endif // KADEMLIARPCSOCKET_H
