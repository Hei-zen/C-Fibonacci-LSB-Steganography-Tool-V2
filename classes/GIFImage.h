#ifndef GIFIMAGE_H
#define GIFIMAGE_H

#include "Image.h"

class GIFImage:public Image{
    public:
        GIFImage(string,string);
        void hide(Payload&) override;
        void extract(Payload&) override;
};

#endif