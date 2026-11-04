#ifndef HARTA_H
#define HARTA_H

#include <vector>
#include <string>
#include <iostream>
#include "Punct2D.h"

enum class TipTeren { CAMPIE, PADURE, MUNTE, APA };
enum class TipBiom { TEMPERAT, VULCANIC, ARCTIC, ARHIPELAG };

class Harta {
private:
    int latime;
    int inaltime;
    TipBiom biom;
    std::vector<std::vector<TipTeren>> grila;

    void genereazaTerenDupaBiom();
    bool esteInInterior(const Punct2D<int>& p) const;

public:
    Harta(int latime, int inaltime, TipBiom biomAles = TipBiom::TEMPERAT);

    TipTeren getTeren(const Punct2D<int>& p) const;
    char getSimbolTeren(TipTeren t) const;
    std::string getNumeBiom() const;
    int getBonusAparare(const Punct2D<int>& p) const;
    int getCostMiscare(const Punct2D<int>& p) const;
    bool esteAccesibil(const Punct2D<int>& p) const;
    int getLatime() const;
    int getInaltime() const;

    friend std::ostream& operator<<(std::ostream& os, const Harta& h);
};

#endif