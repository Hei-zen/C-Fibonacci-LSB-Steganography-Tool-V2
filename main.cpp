#include <iostream>
#include <cstring>

#include "classes/Payload.h"
#include "classes/Image.h"
#include "classes/BMPImage.h"
#include "classes/PNGImage.h"
#include "classes/GIFImage.h"
using namespace std;

Image* createImageHandler(string inFile, string outFile){
    FILE* f = fopen(inFile.c_str(), "rb");
    if (!f) return nullptr;
    unsigned char header[8];
    size_t bytesRead = fread(header, 1, 8, f);
    fclose(f);

    if (bytesRead >= 2 && header[0] == 'B' && header[1] == 'M') {
        return new BMPImage(inFile, outFile);
    }
    if (bytesRead >= 8 && header[0] == 0x89 && header[1] == 'P' && header[2] == 'N' && header[3] == 'G' &&
        header[4] == 0x0D && header[5] == 0x0A && header[6] == 0x1A && header[7] == 0x0A) {
        return new PNGImage(inFile, outFile);
    }
    if (bytesRead >= 6 && header[0] == 'G' && header[1] == 'I' && header[2] == 'F' && header[3] == '8' &&
        (header[4] == '7' || header[4] == '9') && header[5] == 'a') {
        return new GIFImage(inFile, outFile);
    }

    return nullptr;
}

int main(int a, char* f[]){
    if(a<2){
        cout<<"Invalid number of arguments.\n";
        return 1;
    }

    if(strcmp(f[1], "-e") == 0 && a == 5){
        Image* img = createImageHandler(f[2], f[3]);
        if(!img){
            cout<<"Unsupported image format.\n";
            return 1;
        }
        Payload payload(f[4]);
        img->hide(payload);
        delete img;
    }

    else if(strcmp(f[1], "-d") == 0 && a==4){
        Image* img = createImageHandler(f[2], "null" );
        if(!img){
            cout<<"Unsupported image format.\n";
            return 1;
        }
        Payload payload(f[3]);
        img->extract(payload);
        delete img;
    }
    else if(strcmp(f[1], "-h") == 0){
        cout << "-e:\tEncrypt\t./project.exe -e <input_img> <output_img> <payload>\n";
        cout << "-d:\tDecrypt\t./project.exe -d <input_img> <output_payload>\n";
    }
    else{
        cout<<"Invalid Arguments.\n";
    }
    return 0;
}
