#include "storage.h"

#include <stdio.h>
#include <fstream>

void Storage::savePublicKey(p2p::Root::PublicKey& key){
    p2p::Buffer keyBuffer = key.toBuffer();
    std::ofstream outfile ("public.txt",std::ofstream::binary);
    outfile.write ( (char*)keyBuffer.data(), keyBuffer.size() );
}
