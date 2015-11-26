#include "clientthread.h"

#include "stdoutstream.h"

ClientThread::ClientThread(Crypto &crypto) :crypto(crypto) {
}
ClientThread::~ClientThread() { }

p2p::TCP::Stream& ClientThread::getTCPStream() {
    return tcpStream_;
}

void ClientThread::run() {
    try {
        TextIOStream stream(tcpStream_);

        StdOutStream outputStream;

        stream >> outputStream;
        std::cout << crypto.decrypt(outputStream.getString()) << std::endl;
    } catch (const std::exception& e) {
        std::cout << "Failed with error: " << e.what() << std::endl;
    }
}

void ClientThread::cancel() {
    // tcpStream_.close();
}
