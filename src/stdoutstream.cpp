#include "stdoutstream.h"

StdOutStream::StdOutStream() : size_(0) { }

size_t StdOutStream::size() {
    return size_;
}

bool StdOutStream::isValid() const {
    return true;
}

p2p::Event::Source StdOutStream::eventSource() const {
    return p2p::Event::Source();
}

void StdOutStream::output(const uint8_t* data, size_t size) {
    for (size_t i = 0; i < size; i++) {
        str += (char) data[i];
    }
}

size_t StdOutStream::write(const uint8_t* data, size_t dataSize) {
    // std::cout << "Write of size " << dataSize << ": ";
    output(data, dataSize);
    size_ += dataSize;
    return dataSize;
}

std::string StdOutStream::getString(){
    return str;
}
