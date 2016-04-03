#ifndef CRYPTO_H
#define CRYPTO_H

#include <cryptopp/rsa.h>
#include <p2p/Crypt/ECDSA.hpp>
#include <cryptopp/osrng.h>

class Crypto
{
public:
    Crypto();
    void generateNew();
    bool load(std::string privateKey, std::string publicKey);
    bool save(std::string privateKey, std::string publicKey);

    std::string encrypt(std::string plain);
    std::string decrypt(std::string cipher);

    p2p::Crypt::ECDSA::PrivateKey getPrivate();
    p2p::Crypt::ECDSA::PublicKey getPublic();

protected:
    p2p::Crypt::ECDSA::PrivateKey rsaPrivate;
    p2p::Crypt::ECDSA::PublicKey rsaPublic;
    void save(const std::string &filename, const CryptoPP::BufferedTransformation &bt);

    void savePrivateKey(const std::string &filename, const CryptoPP::PrivateKey &key);
    void savePublicKey(const std::string &filename, const CryptoPP::PublicKey &key);
    void loadPrivateKey(const std::string &filename, CryptoPP::PrivateKey &key);
    void loadPublicKey(const std::string &filename, CryptoPP::PublicKey &key);
    void load(const std::string &filename, CryptoPP::BufferedTransformation &bt);

    CryptoPP::AutoSeededRandomPool rng;
};

#endif // CRYPTO_H
