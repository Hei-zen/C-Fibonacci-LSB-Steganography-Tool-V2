#include "GIFImage.h"
#include <iostream>
#include <cstdio>
using namespace std;

GIFImage::GIFImage(string in, string out):Image(in,out){}

void GIFImage::hide(Payload& payload){
    FILE *in = fopen(inFile.c_str(), "rb");
    FILE *out = fopen(outFile.c_str(), "wb");
    FILE *msg = fopen(payload.getFilename().c_str(), "rb");
    if(!in || !out || !msg){
        cout<<"Error opening files.\n";
        fclose(in);
        fclose(out);
        fclose(msg);
        return;
    }

    int fileSize=getFileSize(in);
    int msgSize=getFileSize(msg);
    payload.setSize(msgSize);

    unsigned char head[13];
    fread(head, 1, 13, in);
    int header=13;
    if(head[10] & 0x80){
        int paletteSize = (head[10] & 0x07) + 1;
        header+=3*(1 << paletteSize); 
    }
    fseek(in, 0, SEEK_SET);

    if(fileSize <= header || 32 + (long)msgSize * 8 > fileSize - header){
        cout<<"Payload too large for this specific image.\n";
        fclose(in);
        fclose(out);
        fclose(msg);
        return;
    }

    for(int i=0; i<header; i++)
        fputc(fgetc(in), out);

    bitStream bs;
    bs.msgSize=bs.left=msgSize;
    unsigned char pix;

    for(int i=header; i<fileSize;i++){
        fread(&pix, 1, 1, in);
        pix= hidePixel(pix, bs, msg);
        fwrite(&pix, 1, 1, out);
    }
    fclose(in);
    fclose(out);
    fclose(msg);
}

void GIFImage::extract(Payload& payload){
    FILE* in=fopen(inFile.c_str(), "rb");
    FILE *out=fopen(payload.getFilename().c_str(), "wb");
    if(!in || !out){
        cout<<"Error opening files.\n";
        fclose(in);
        fclose(out);
        return;
    }

    int fileSize=getFileSize(in);
    unsigned char head[13];
    fread(head, 1, 13, in);
    int header=13;
    if(head[10] & 0x80){
        int paletteSize=(head[10]&0x07) + 1;
        header+=3*(1<<paletteSize);
    }
    fseek(in, header, SEEK_SET);

    bitStream bs;
    unsigned char pix;

    for(int i=header; i<fileSize; i++){
        fread(&pix, 1, 1, in);
        if(extractPixel(pix, bs, out)){
            fclose(in);
            fclose(out);
            return;
        }
    }
    fclose(in);
    fclose(out);
}