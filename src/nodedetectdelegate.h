#ifndef NODEDETECTDELEGATE_H
#define NODEDETECTDELEGATE_H

#include <p2p/Root.hpp>
#include <p2p/Concurrency.hpp>

class NodeDetectDelegate: public p2p::Root::NodeDetectDelegate {
    public:
        NodeDetectDelegate(p2p::Root::NodeDatabase& nodeDatabase, p2p::MessageQueue<p2p::Root::NodePair>& messageQueue);

        void detectedNodePair(const p2p::Root::NodePair& nodePair);

    private:
        p2p::Root::NodeDatabase& nodeDatabase_;
        p2p::MessageQueue<p2p::Root::NodePair>& messageQueue_;

};

#endif // NODEDETECTDELEGATE_H
