#include "querynodethread.h"

QueryNodeThread::QueryNodeThread(Logger& logger,
        const p2p::Root::NodeId& myId, p2p::Kademlia::BucketSet<p2p::Root::NodeId>& dhtBucketSet,
        p2p::Root::Core::Service& coreService, p2p::Root::DHT::Service& dhtService,
        p2p::MessageQueue<p2p::Root::NodePair>& messageQueue)
    : logger_(logger),
    myId_(myId), dhtBucketSet_(dhtBucketSet),
    coreService_(coreService), dhtService_(dhtService),
    messageQueue_(messageQueue) { }

void QueryNodeThread::run() {
    std::set<p2p::Root::NodeId> knownNodes;

    while (!signal_.isActive()) {
        while (!messageQueue_.empty()) {
            const auto nodePair = messageQueue_.receive();
            const auto& peerId = nodePair.id;
            const auto& peerEndpoint = nodePair.endpoint;

            if (myId_ == peerId) {
                // Node is current node.
                continue;
            }

            if (knownNodes.find(peerId) != knownNodes.end()) {
                // Node already queried.
                continue;
            }

            logger_.log(STR("--- Querying node '%s'...", peerId.hexString().c_str()));

            knownNodes.insert(peerId);

            const auto endpoint = coreService_.ping(peerEndpoint, peerId).get();

            logger_.log(STR("Node reports my endpoint is '%s'.", endpoint.udpEndpoint.toString().c_str()));
            // TODO: add this endpoint to our set of endpoints.

            const auto networks = coreService_.queryNetworks(peerEndpoint, peerId).get();

            logger_.log(STR("Node supports %llu networks.", (unsigned long long) networks.size()));

            bool supportsDHT = false;
            for (size_t i = 0; i < networks.size(); i++) {
                logger_.log(STR("    Network %llu: %s.", (unsigned long long) i, networks.at(i).hexString().c_str()));
                if (networks.at(i) == p2p::Root::NetworkId::Generate("p2p.rootdht")) {
                    logger_.log("        -> Supports DHT network.");
                    supportsDHT = true;
                }
            }

            if (!supportsDHT) {
                logger_.log("Node doesn't support DHT.");
                continue;
            }

            dhtBucketSet_.add(peerId);

            const auto peerNearestNodes = dhtService_.getNearestNodes(peerId, myId_).get();

            if (peerNearestNodes.empty()) {
                logger_.log("Node doesn't seem to know any other nodes.");
                continue;
            }

            logger_.log("Node reports our nearest nodes as:");

            for (const auto& dhtNode: peerNearestNodes) {
                logger_.log(STR("    Node '%s'", dhtNode.id.hexString().c_str()));
                if (dhtNode.endpointSet.empty()) continue;

                // Query all our nearest nodes.
                messageQueue_.send(p2p::Root::NodePair(dhtNode.id, *(dhtNode.endpointSet.begin())));
            }
        }

        p2p::Event::Wait({ messageQueue_.eventSource(), signal_.eventSource() });
    }
}

void QueryNodeThread::cancel() {
    signal_.activate();
}
