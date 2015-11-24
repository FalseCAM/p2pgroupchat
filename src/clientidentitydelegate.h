#ifndef CLIENTIDENTITYDELEGATE_H
#define CLIENTIDENTITYDELEGATE_H

#include <p2p/Kademlia.hpp>
#include <p2p/Root.hpp>

class ClientIdentityDelegate: public p2p::Root::IdentityDelegate {
    public:
        ClientIdentityDelegate(p2p::Root::NodeDatabase& nodeDatabase, p2p::Root::PrivateIdentity& privateIdentity);

        p2p::Root::PrivateIdentity& getPrivateIdentity();

        p2p::Root::PublicIdentity& getPublicIdentity(const p2p::Root::PublicKey& key);

    private:
        p2p::Root::NodeDatabase& nodeDatabase_;
        p2p::Root::PrivateIdentity& privateIdentity_;

};

#endif // CLIENTIDENTITYDELEGATE_H
