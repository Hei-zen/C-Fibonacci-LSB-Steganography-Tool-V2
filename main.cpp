#include <iostream>
#include <cstring>

#include "classes/Payload.h"
#include "classes/Image.h"
#include "classes/BMPImage.h"
#include "classes/PNGImage.h"
#include "classes/GIFImage.h"
using namespace std;

Image* createImageHandler(string inFile, string outFile){
    if(inFile.length() >= 4){
        string ext = inFile.substr(inFile.length() - 4);
        if(ext == ".bmp" || ext == ".BMP") return new BMPImage(inFile, outFile);
        if(ext == ".png" || ext == ".PNG") return new PNGImage(inFile, outFile);
        if(ext == ".gif" || ext == ".GIF") return new GIFImage(inFile, outFile);
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