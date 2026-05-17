#include "Image.h"
#include <cstring>

Image::Image(string in, string out): inFile(in), outFile(out){}

bool Image::getbit(int pix){
    if (pix<0)
        pix+=256;
    for(int i=11;i>=0;i--)
        if(fib[i]<=pix){
            pix-=fib[i];
            if(i==0) 
                return true;
            i--;
        } 
    return false;
}

char Image::embed(int pix, int bit){
    if(pix < 0)
        pix+=256;
    for(int i=0;i<=255;i++){
        if(pix+i <= 255 && getbit(pix+i)==bit) return (char)(pix+i);
        if(pix-i >= 0 && getbit(pix-i)==bit) return (char)(pix-i);
    }
    return (char)pix;
}

int Image::getFileSize(FILE* f){
    fseek(f, 0, SEEK_END);
        int size=(unsigned int)ftell(f);
    fseek(f, 0, SEEK_SET);
    return size;
}

unsigned char Image::hidePixel(unsigned char pix, bitStream& bs, FILE* msg){
    if(bs.sizeCount < 32){
        pix=embed(pix, (bs.msgSize >> (31-bs.sizeCount))&1);
        if(++bs.sizeCount==32 && bs.left>0)
            if(fread(&bs.ch,1,1,msg)!=1) bs.ch=0;
    }
    else if(bs.left > 0){
        pix=embed(pix, (bs.ch >> (7-bs.index))&1);
        if(++bs.index==8){
            bs.index=0;
            if (--bs.left > 0 && fread(&bs.ch, 1, 1,  msg)!=1)bs.ch=0;
        }
    }
    return pix;
}

bool Image::extractPixel(unsigned char pix, bitStream& bs, FILE* out){
    if (bs.sizeCount < 32){
        bs.msgSize = (bs.msgSize << 1) | getbit(pix);
        if(++bs.sizeCount == 32) bs.left = bs.msgSize;
    }
    else if (bs.left > 0){
        bs.ch=(bs.ch<<1) | getbit(pix);
        if(++bs.index==8){
            fputc(bs.ch, out);
            bs.ch=0;
            bs.index=0;
            if(--bs.left==0)
                return true;
        }
    }
    return false;
}
