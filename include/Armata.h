#ifndef ARMATA_H
#define ARMATA_H

#include <string>
#include <vector>
#include <iostream>
#include "Unitate.h"
#include "Punct2D.h"

class Armata {
private:
    std::string numeFactiune;
    std::string fisierSursa;
    int aur;
    int numarOrase;
    int productieOras;
    Punct2D<int> baza;
    std::vector<Unitate> unitati;

    void eliminaUnitatiInvinse();

public:
    Armata(const std::string& nume, const std::string& fisier, int aurInitial, const Punct2D<int>& coordBaza);
    Armata(const Armata& alta);
    Armata& operator=(const Armata& alta);
    ~Armata();

    bool incarcaTrupeInitiale();
    bool recruteazaUnitate(std::size_t indexSablon);
    void colecteazaVenit();
    void cresteProductieMinerit(int bonus);
    bool cheltuieAur(int suma);
    void curataArmata();

    const std::string& getNume() const;
    int getAur() const;
    int getNumarOrase() const;
    const Punct2D<int>& getBaza() const;
    std::vector<Unitate>& getUnitati();
    const std::vector<Unitate>& getUnitati() const;
    int calculeazaScorPutere() const;

    friend std::ostream& operator<<(std::ostream& os, const Armata& a);
};

#endif