#ifndef TEXTIOSTREAM_H
#define TEXTIOSTREAM_H

#include <stdint.h>
#include <stdio.h>
#include <fstream>

#include <p2p/Root.hpp>

class TextIOStream {
    public:
        TextIOStream(p2p::IOStream& stream);

        TextIOStream& operator<<(const std::string& string);

        TextIOStream& operator>>(p2p::OStream& stream);

    private:
        p2p::IOStream& stream_;
        p2p::BinaryIOStream binaryStream_;

};

#endif // TEXTIOSTREAM_H
