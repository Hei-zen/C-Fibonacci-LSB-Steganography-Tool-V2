#include "Payload.h"

Payload::Payload(string file) : filename(file), size(0){}

string Payload::getFilename() const{return filename;}
void Payload::setSize(int s){size=s;}
int Payload::getSize() const{return size;}