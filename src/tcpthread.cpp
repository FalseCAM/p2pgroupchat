#include "tcpthread.h"
#include "clientthread.h"
#include <p2p/Concurrency.hpp>

TcpThread::TcpThread(p2p::TCP::Acceptor* acceptor){
    acceptor_ = acceptor;
}

void TcpThread::run() {
    while (true) {
        ClientThread* c = clientThreads.add(new ClientThread());

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
