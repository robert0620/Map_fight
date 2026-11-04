#ifndef UNITATE_H
#define UNITATE_H

#include <string>
#include <iostream>
#include "Punct2D.h"

enum class Rang { RECRUT, VETERAN, ELITA };

class Unitate {
private:
    std::string nume;
    int viata;
    int atac;
    int raza;
    int cost;
    Punct2D<int> pozitie;
    int xp;
    Rang rang;

    void verificaPromovare();

public:
    Unitate(const std::string& nume, int viata, int atac, int raza, int cost, const Punct2D<int>& startPoz = Punct2D<int>(0, 0));

    const std::string& getNume() const;
    int getViata() const;
    int getAtac() const;
    int getRaza() const;
    int getCost() const;
    const Punct2D<int>& getPozitie() const;
    void setPozitie(const Punct2D<int>& nouaPoz);
    bool esteVie() const;

    void primesteDaune(int daune);
    void castigaXP(int cantitate);
    int calculeazaPutereAtac(double modificatorVreme) const;
    std::string getNumeRang() const;

    friend std::ostream& operator<<(std::ostream& os, const Unitate& u);
};

#endif