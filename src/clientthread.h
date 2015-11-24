#ifndef CLIENTTHREAD_H
#define CLIENTTHREAD_H

#include <p2p/Concurrency.hpp>
#include <p2p/Root.hpp>
#include <p2p/Transport.hpp>
#include <p2p/UDP.hpp>
#include <p2p/TCP.hpp>

#include "textiostream.h"

class ClientThread: public p2p::Runnable {
    public:
        ClientThread();
        virtual ~ClientThread();

        p2p::TCP::Stream& getTCPStream();

        void run();

        void cancel();

    private:
        p2p::TCP::Stream tcpStream_;

};

#endif // CLIENTTHREAD_H
