#ifndef LINE_READER_H
#define LINE_READER_H
using namespace std;

#include <string>

class LineReader {
public:
    virtual ~LineReader() = default;
    virtual string read(const string& prompt) = 0;
};

class StdinReader : public LineReader {
public:
    string read(const string& prompt) override;
};

#ifdef USE_READLINE
class ReadlineReader : public LineReader {
public:
    string read(const string& prompt) override;
};
#endif

#endif