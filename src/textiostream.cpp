#include "textiostream.h"

TextIOStream::TextIOStream(p2p::IOStream& stream) : stream_(stream), binaryStream_(stream) { }

TextIOStream& TextIOStream::operator<<(const std::string& string) {
    binaryStream_.output().writeAll((const uint8_t*) string.c_str(), string.size());
    return *this;
}

TextIOStream& TextIOStream::operator>>(p2p::OStream& stream) {
    p2p::Binary::MoveData(stream_, stream);
    return *this;
}
