#ifndef BMPIMAGE_H
#define BMPIMAGE_H

#include "Image.h"

class BMPImage:public Image{
    public:
        BMPImage(string,string);
        void hide(Payload&) override;
        void extract(Payload&) override;
};

#endif