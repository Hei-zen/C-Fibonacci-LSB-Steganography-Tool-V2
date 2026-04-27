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

    int msgSize=getFileSize(msg);
    payload.setSize(msgSize);

    unsigned char head[13];
    if(fread(head, 1, 13, in) != 13){
        cout<<"Error reading GIF header.\n";
        fclose(in); fclose(out); fclose(msg); return;
    }

    int paletteBytes = 0;
    if(head[10] & 0x80){
        int paletteSize = (head[10] & 0x07) + 1;
        paletteBytes = 3 * (1 << paletteSize); 
    }

    if(paletteBytes == 0 || 32 + (long)msgSize * 8 > paletteBytes){
        cout<<"Payload too large for the Global Color Table (max capacity: "<< (paletteBytes > 32 ? (paletteBytes-32)/8 : 0) << " bytes).\n";
        fclose(in); fclose(out); fclose(msg); return;
    }

    fwrite(head, 1, 13, out);

    bitStream bs;
    bs.msgSize=bs.left=msgSize;
    unsigned char pix;

    // Embed in palette
    for(int i=0; i<paletteBytes; i++){
        fread(&pix, 1, 1, in);
        pix = hidePixel(pix, bs, msg);
        fwrite(&pix, 1, 1, out);
    }

    // Copy the rest of the file (Image blocks, extensions, trailer)
    int ch;
    while((ch = fgetc(in)) != EOF){
        fputc(ch, out);
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

    unsigned char head[13];
    if(fread(head, 1, 13, in) != 13){
        cout<<"Error reading GIF header.\n";
        fclose(in); fclose(out); return;
    }

    int paletteBytes = 0;
    if(head[10] & 0x80){
        int paletteSize = (head[10] & 0x07) + 1;
        paletteBytes = 3 * (1 << paletteSize); 
    }

    if(paletteBytes == 0){
        cout<<"No Global Color Table found in GIF.\n";
        fclose(in); fclose(out); return;
    }

    bitStream bs;
    unsigned char pix;

    for(int i=0; i<paletteBytes; i++){
        fread(&pix, 1, 1, in);
        if(extractPixel(pix, bs, out)){
            break;
        }
    }

    fclose(in);
    fclose(out);
}
