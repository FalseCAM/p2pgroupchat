#ifndef STORAGE_H
#define STORAGE_H

#include <p2p/Root.hpp>

class Storage {
public:
    static void savePublicKey(p2p::Root::PublicKey& key);
};

#endif // STORAGE_H
