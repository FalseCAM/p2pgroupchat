#include "clientidentitydelegate.h"

ClientIdentityDelegate::ClientIdentityDelegate(p2p::Root::NodeDatabase& nodeDatabase, p2p::Root::PrivateIdentity& privateIdentity)
    : nodeDatabase_(nodeDatabase), privateIdentity_(privateIdentity) { }

p2p::Root::PrivateIdentity& ClientIdentityDelegate::getPrivateIdentity() {
    return privateIdentity_;
}

p2p::Root::PublicIdentity& ClientIdentityDelegate::getPublicIdentity(const p2p::Root::PublicKey& key) {
    const auto nodeId = p2p::Root::NodeId::Generate(key);
    if (!nodeDatabase_.isKnownId(nodeId)) {
        nodeDatabase_.addNode(nodeId, p2p::Root::NodeEntry(p2p::Root::PublicIdentity(key, 0)));
    }
    return nodeDatabase_.nodeEntry(nodeId).identity;
}
