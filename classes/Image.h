#ifndef IMAGE_H
#define IMAGE_H
#include "Payload.h"
#include <cstdio>

class Image{
    protected:
        const string inFile;
        const string outFile;
        int fib[11]={1,2,3,5,8,13,21,34,55,89,144};

        struct bitStream{
            int msgSize=0;
            int left=0;
            int sizeCount=0;
            char ch=0;
            int index=0;
        };

        bool getbit(int);
        char embed(int,int);
        int getFileSize(FILE*);
        unsigned char hidePixel(unsigned char, bitStream&, FILE*);
        bool extractPixel(unsigned char, bitStream&, FILE*);

    public:
        Image(string, string);
        virtual void hide(Payload&) = 0;
        virtual void extract(Payload&) = 0;
};

#endif