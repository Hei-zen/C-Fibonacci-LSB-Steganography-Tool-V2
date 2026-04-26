#ifndef PNGIMAGE_H
#define PNGIMAGE_H

#include "Image.h"
#include <string>

using namespace std;

class PNGImage : public Image {
public:
    PNGImage(string in, string out);
    void hide(Payload& payload) override;
    void extract(Payload& payload) override;
};

#endif
