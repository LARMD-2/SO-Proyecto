#ifndef LINE_READER_H
#define LINE_READER_H

#include <string>

class LineReader {
public:
    virtual ~LineReader() = default;
    virtual std::string read(const std::string& prompt) = 0;
};

class StdinReader : public LineReader {
public:
    std::string read(const std::string& prompt) override;
};

#ifdef USE_READLINE
class ReadlineReader : public LineReader {
public:
    std::string read(const std::string& prompt) override;
};
#endif

#endif