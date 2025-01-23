#include "buffer.h"

Buffer::Buffer(std::ifstream& _file) : file(_file) {
    file.read(buf, sizeof(buf));
    pos = 0;
    bufEnd = file.gcount();
}

char Buffer::get() {
    if (bufEnd == 0) {
        return EOF;
    }
    if (pos == bufEnd) {
        file.read(buf, sizeof(buf));
        bufEnd = file.gcount();

        if (bufEnd == 0) {
            return EOF;
        }
        pos = 0;
    }
    return buf[pos++];
}

void Buffer::unget() {
    if (pos > 0) pos--;
}