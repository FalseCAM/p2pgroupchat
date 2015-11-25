#include "crypto.h"

#include <cryptopp/osrng.h>
#include <cryptopp/files.h>


Crypto::Crypto()
{
}

void Crypto::generateNew()
{
    CryptoPP::AutoSeededRandomPool rnd;

    rsaPrivate.GenerateRandomWithKeySize(rnd, 3072);

    rsaPublic = CryptoPP::RSA::PublicKey(rsaPrivate);
}

bool Crypto::load(std::string privateKey, std::string publicKey)
{
    try{
        loadPrivateKey(privateKey, rsaPrivate);
        loadPublicKey(publicKey, rsaPublic);
    }catch(std::exception &e){
            return false;
    }
    return true;
}

bool Crypto::save(std::string privateKey, std::string publicKey)
{
    savePrivateKey(privateKey, rsaPrivate);
    savePublicKey(publicKey, rsaPublic);
    return true;
}

void Crypto::savePrivateKey(const std::string& filename, const CryptoPP::PrivateKey& key)
{
    CryptoPP::ByteQueue queue;
    key.Save(queue);
    save(filename, queue);
}

void Crypto::loadPrivateKey(const std::string& filename, CryptoPP::PrivateKey& key)
{
    CryptoPP::ByteQueue queue;
    load(filename, queue);

    key.Load(queue);
}

void Crypto::savePublicKey(const std::string& filename, const CryptoPP::PublicKey& key)
{
    CryptoPP::ByteQueue queue;
    key.Save(queue);
    save(filename, queue);
}

void Crypto::loadPublicKey(const std::string& filename, CryptoPP::PublicKey& key)
{
    CryptoPP::ByteQueue queue;
    load(filename, queue);

    key.Load(queue);
}

void Crypto::save(const std::string& filename, const CryptoPP::BufferedTransformation& bt)
{
    CryptoPP::FileSink file(filename.c_str());

    bt.CopyTo(file);
    file.MessageEnd();
}

void Crypto::load(const std::string& filename, CryptoPP::BufferedTransformation& bt)
{
    CryptoPP::FileSource file(filename.c_str(), true /*pumpAll*/);

    file.TransferTo(bt);
    bt.MessageEnd();
}
