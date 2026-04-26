#ifndef PAYLOAD_H
#define PAYLOAD_H
#include <string>

using namespace std;

class Payload{
    private:
    string filename;
    int size;
    public:
    Payload(string);

    string getFilename() const;
    void setSize(int);
    int getSize() const;
};
#endif