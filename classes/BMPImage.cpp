#include "BMPImage.h"
#include <iostream>
#include <cstdio>
using namespace std;

BMPImage::BMPImage(string in, string out): Image(in, out){}

void BMPImage::hide(Payload& payload){
    FILE* in=fopen(inFile.c_str(), "rb");
    FILE* out=fopen(outFile.c_str(), "wb");
    FILE* msg=fopen(payload.getFilename().c_str(), "rb");
    if(!in || !out || !msg){
        cout<<"Error opening files.\n";
        if(in)fclose(in);
        if(out)fclose(out);
        if(msg)fclose(msg);
        return;
    }

    char head[54];
    if(fread(head, 1, 54, in) != 54){
        cout<<"Error reading BMP header.\n";
        fclose(in);
        fclose(out);
        fclose(msg);
        return;
    }
    fwrite(head, 1 , 54, out);
    int offset=*(int*)&head[10];
    int width=*(int*)&head[18];
    int height=*(int*)&head[22];
    int pad = (4-(width*3)%4)%4;
    height = abs(height);
    int msgSize = getFileSize(msg);
    payload.setSize(msgSize);

    if(32+(long)msgSize*8>(long)height*width*3){
        cout<<"Payload too large for this specific image.\n";
        fclose(in);
        fclose(out);
        fclose(msg);
        return;
    }

    for(int i=54; i<offset; i++)
        fputc(fgetc(in), out);

    bitStream bs;
    bs.msgSize=bs.left=msgSize;
    unsigned char pix;

    for(int y=0; y<height; y++){
        for(int x=0; x<width*3; x++){
            fread(&pix, 1, 1, in);
            pix=hidePixel(pix, bs, msg);
            fwrite(&pix, 1, 1, out);
        }
        for(int p=0; p<pad;p++)
            fputc(fgetc(in), out);
    }
    int temp;
    while((temp=fgetc(in))!=EOF) fputc(temp,out);
    fclose(in);
    fclose(out);
    fclose(msg);
}

void BMPImage::extract(Payload& payload){
    FILE* in=fopen(inFile.c_str(), "rb");
    FILE* out=fopen(payload.getFilename().c_str(),"wb");
    if(!in || !out){
        cout<<"Error opening files.\n";
        if (in) fclose(in);
        if (out) fclose(out);
        return;
    }

    char head[54];
    fread(head, 1, 54, in);
    int offset=*(int*)&head[10];
    int width=*(int*)&head[18];
    int height=*(int*)&head[22];
    height = abs(height);
    int pad = (4-(width*3)%4)%4;
    fseek(in, offset, SEEK_SET);

    bitStream bs;
    unsigned char pix;

    for(int y=0; y<height; y++){
        for(int x=0; x<width * 3; x++){
            fread(&pix, 1, 1, in);
            if(extractPixel(pix, bs, out)){
                fclose(in);
                fclose(out);
                return;
            }
        }
        fseek(in, pad, SEEK_CUR);
    }

    fclose(in);
    fclose(out);
}
