#include "PNGImage.h"
#include <iostream>
#include <cstdio>

#define STB_IMAGE_IMPLEMENTATION
#include "../stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../stb_image_write.h"

using namespace std;

PNGImage::PNGImage(string in, string out):Image(in, out){}

void PNGImage::hide(Payload& payload){
    FILE *msg=fopen(payload.getFilename().c_str(), "rb");
    if(!msg){
        cout<<"Error opening files.";return;
    }

    int msgSize=getFileSize(msg);
    payload.setSize(msgSize);

    int w, h, ch;
    unsigned char* img= stbi_load(inFile.c_str(), &w, &h, &ch, 0);
    if(!img){
        cout<<"Error loading image.\n"; 
        fclose(msg);
        return;
    }
    unsigned int total = (unsigned int)(w*h*ch);
    if(32+(unsigned long)msgSize*8>total){
        cout<<"Payload is too large for this specific image.\n";
        fclose(msg);stbi_image_free(img);
        return;
    }

    bitStream bs;
    bs.msgSize=bs.left=msgSize;
    for(unsigned int i=0; i<total;i++){
        img[i]=hidePixel(img[i], bs, msg);
        if (bs.left == 0 && bs.sizeCount==32) break;
    }
    fclose(msg);
    stbi_write_png(outFile.c_str(), w, h, ch, img, w*ch);
    stbi_image_free(img);
}

void PNGImage::extract(Payload& payload){
    int w, h, ch;
    unsigned char* img=stbi_load(inFile.c_str(), &w, &h, &ch, 0);
    if(!img){
        cout<<"Error loading image.\n";
        return;
    }
    FILE *out=fopen(payload.getFilename().c_str(),"wb");
    if(!out){
        cout<<"Error opening output file.\n";
        stbi_image_free(img); 
        return;
    }

    bitStream bs;
    unsigned int total=(unsigned int)(w * h * ch);

    for(unsigned int i=0; i<total; i++){
        if(extractPixel(img[i], bs, out)) break;
    }

    fclose(out);
    stbi_image_free(img);
}
