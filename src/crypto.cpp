#include "crypto.h"
#include <cryptopp/files.h>
#include <p2p/Crypt/AutoSeededRandomPool.hpp>

Crypto::Crypto()
{
}

void Crypto::generateNew()
{
    //rsaPrivate.GenerateRandomWithKeySize(rng, 3072);

    p2p::Crypt::AutoSeededRandomPool rand;
    rsaPrivate = p2p::Crypt::ECDSA::PrivateKey(rand, p2p::Crypt::ECDSA::brainpoolP256r1);

    rsaPublic = p2p::Crypt::ECDSA::PublicKey(rand, rsaPrivate);
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

std::string Crypto::encrypt(std::string plain)
{
    std::string cipher;
    CryptoPP::RSAES_OAEP_SHA_Encryptor e( rsaPrivate );

    CryptoPP::StringSource ss1( plain, true,
                                new CryptoPP::PK_EncryptorFilter( rng, e,
                                                                  new CryptoPP::StringSink( cipher )
                                                                  )
                                );
    return cipher;
}

std::string Crypto::decrypt(std::string cipher)
{
    std::string recovered;
    CryptoPP::RSAES_OAEP_SHA_Decryptor d( rsaPrivate );

    CryptoPP::StringSource ss2( cipher, true,
                                new CryptoPP::PK_DecryptorFilter( rng, d,
                                                                  new CryptoPP::StringSink( recovered )
                                                                  )
                                );
    return recovered;
}

p2p::Crypt::ECDSA::PrivateKey Crypto::getPrivate()
{
    return rsaPrivate;
}

p2p::Crypt::ECDSA::PublicKey Crypto::getPublic()
{
    return rsaPublic;
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
