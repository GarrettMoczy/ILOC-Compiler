//buffer.h
#ifndef BUFFER_H
#define BUFFER_H

#include <fstream>

constexpr int BUF_SIZE = 1024;

class Buffer
{
private:
    std::ifstream& file;
    char buf[BUF_SIZE];
    int pos;
    int bufEnd;
public:
    Buffer(std::ifstream& _file);
    ~Buffer() = default;
    char get();
    void unget();
};

#endif
