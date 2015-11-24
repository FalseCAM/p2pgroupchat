#ifndef CRYPTO_H
#define CRYPTO_H

#include <cryptopp/rsa.h>

class Crypto
{
public:
    Crypto();
    void generateNew();
    bool load(std::string privateKey, std::string publicKey);
    bool save(std::string privateKey, std::string publicKey);

protected:
    CryptoPP::RSA::PrivateKey rsaPrivate;
    CryptoPP::RSA::PublicKey rsaPublic;
    void save(const std::string &filename, const CryptoPP::BufferedTransformation &bt);

    void savePrivateKey(const std::string &filename, const CryptoPP::PrivateKey &key);
    void savePublicKey(const std::string &filename, const CryptoPP::PublicKey &key);
};

#endif // CRYPTO_H
