#ifndef TCPTHREAD_H
#define TCPTHREAD_H

#include <p2p/Root.hpp>
#include <p2p/TCP.hpp>
#include <p2p/Concurrency.hpp>
#include <p2p/Event.hpp>

#include "clientthread.h"
#include "container.h"

class TcpThread: public p2p::Runnable {
public:
    TcpThread(p2p::TCP::Acceptor* acceptor);

    void run();

    void cancel();

private:
    p2p::TCP::Acceptor *acceptor_;
    Container<ClientThread> clientThreads;
    Container<p2p::Thread> threads;
};

#endif // TCPTHREAD_H
