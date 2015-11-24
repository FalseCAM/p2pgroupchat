#include "nodedetectdelegate.h"

NodeDetectDelegate::NodeDetectDelegate(p2p::Root::NodeDatabase& nodeDatabase, p2p::MessageQueue<p2p::Root::NodePair>& messageQueue)
    : nodeDatabase_(nodeDatabase), messageQueue_(messageQueue) { }

void NodeDetectDelegate::detectedNodePair(const p2p::Root::NodePair& nodePair) {
    if (!nodeDatabase_.isKnownId(nodePair.id)) return;

    if (nodeDatabase_.nodeEntry(nodePair.id).endpointSet.empty()) {
        messageQueue_.send(nodePair);
    }
}
