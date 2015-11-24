#ifndef STDOUTSTREAM_H
#define STDOUTSTREAM_H

#include <p2p/Event.hpp>
#include <p2p/Transport.hpp>


class StdOutStream: public p2p::OStream {
    public:
        StdOutStream();

        size_t size();

        bool isValid() const;

        p2p::Event::Source eventSource() const;

        void output(const uint8_t* data, size_t size);

        size_t write(const uint8_t* data, size_t dataSize);

        std::string getString();

    private:
        std::string str;
        size_t size_;

};

#endif // STDOUTSTREAM_H
