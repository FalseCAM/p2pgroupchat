#ifndef EVENTTHREAD_H
#define EVENTTHREAD_H

#include <p2p/Event.hpp>
#include <p2p/Concurrency.hpp>
#include <p2p/Root.hpp>

class EventThread: public p2p::Runnable {
    public:
        EventThread(p2p::Root::Core::Service& coreService, p2p::Root::DHT::Service& dhtService);

        void run();

        void cancel();

    private:
        p2p::Root::Core::Service& coreService_;
        p2p::Root::DHT::Service& dhtService_;
        p2p::Event::Signal signal_;
};

#endif // EVENTTHREAD_H
