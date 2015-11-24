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

void Crypto::savePublicKey(const std::string& filename, const CryptoPP::PublicKey& key)
{
    CryptoPP::ByteQueue queue;
    key.Save(queue);
    save(filename, queue);
}

void Crypto::save(const std::string& filename, const CryptoPP::BufferedTransformation& bt)
{
    CryptoPP::FileSink file(filename.c_str());

    bt.CopyTo(file);
    file.MessageEnd();
}
