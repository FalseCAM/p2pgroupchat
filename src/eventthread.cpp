#include "eventthread.h"

EventThread::EventThread(p2p::Root::Core::Service& coreService, p2p::Root::DHT::Service& dhtService)
    : coreService_(coreService), dhtService_(dhtService) { }

void EventThread::run() {
    while (!signal_.isActive()) {
        while (coreService_.processMessage() || dhtService_.processMessage()) { }
        p2p::Event::Wait({ coreService_.eventSource(), dhtService_.eventSource(), signal_.eventSource() });
    }
}

void EventThread::cancel() {
    signal_.activate();
}
