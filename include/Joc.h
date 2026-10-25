#ifndef JOC_H
#define JOC_H
#include "Armata.h"
#include <string>

class Joc {
private:
    Armata dwarfs;
    Armata demons;
    bool incarcaTrupe(Armata& armata, const std::string& caleFisier);

public:
    Joc();
    void start(); 
};

#endif