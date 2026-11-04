#ifndef ARMATA_H
#define ARMATA_H

#include <vector>
#include <string>
#include <cstddef>
#include <iostream>
#include "Unitate.h"
#include "Harta.h"

class Armata {
private:
    std::string numeJucator;
    std::vector<Unitate> trupe;
    int aur{20};
    int numarOrase{1};

public:
    Armata(const std::string& numeJucator_, int aurInitial_);

    void recruteaza(const Unitate& trupaNoua);
    void ordonaAtac(size_t indexTrupa, Unitate& inamic, const Harta& harta);

    bool deplaseazaUnitate(size_t indexTrupa, int nouX, int nouY, Harta& harta);
    void colecteazaVenituriDinOrase();
    bool vindecaTrupaCuAur(size_t indexTrupa, int costAur, int puncteViata);
    void eliminaTrupeCazute();
    int calculeazaPutereTotala() const;

    Unitate& getTrupa(size_t index);
    const Unitate& getTrupa(size_t index) const;
    size_t getNumarTrupe() const;
    int getAur() const;
    int getNumarOrase() const;
    const std::string& getNumeJucator() const;
    bool maiAreTrupeVii() const;

    friend std::ostream& operator<<(std::ostream& os, const Armata& a);
};

#endif