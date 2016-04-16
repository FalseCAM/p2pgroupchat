#include "tcpthread.h"
#include "clientthread.h"
#include <iostream>
#include <p2p/Concurrency.hpp>

TcpThread::TcpThread(p2p::TCP::Acceptor* acceptor, Crypto &crypto):acceptor_(acceptor), crypto(crypto){
}

void TcpThread::run() {
    while (true) {
        ClientThread* c = clientThreads.add(new ClientThread(crypto));

        if (acceptor_->accept(c->getTCPStream())) {
            //thread per connection
            threads.add(new p2p::Thread(*c));
        } else {
            std::cout << "Error: Accept failed." << std::endl;
            return ;
        }
    }
}

void TcpThread::cancel() {
}
