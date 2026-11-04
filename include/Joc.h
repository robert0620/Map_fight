#ifndef JOC_H
#define JOC_H

#include "Armata.h"
#include "Harta.h"
#include <string>
#include <iostream>

class Joc {
private:
    Harta harta;
    Armata dwarfs;
    Armata demons;
    int rundaCurenta{1};

    static bool incarcaTrupe(Armata& armata, const std::string& caleFisier, int startX, int startY);
    static bool incarcaTrupeDinJson(Armata& armata, const std::string& caleFisier);

public:
    Joc(int liniiHarta, int coloaneHarta, TipBiom biom);

    void start();
    void executaRundaSimulata(int mutareX, int mutareY);
    void executaMutareBot();
    std::string determinaLider() const;

    const Harta& getHarta() const;

    friend std::ostream& operator<<(std::ostream& os, const Joc& j);
};

#endif