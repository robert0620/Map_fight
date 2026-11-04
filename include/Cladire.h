#ifndef CLADIRE_H
#define CLADIRE_H

#include <string>
#include "Armata.h"

class Cladire {
protected:
    int x, y;
    int costConstruire;
    std::string proprietar;

public:
    Cladire(int x, int y, int cost, std::string prop)
        : x(x), y(y), costConstruire(cost), proprietar(prop) {}

    virtual ~Cladire() = default;

    int getX() const { return x; }
    int getY() const { return y; }
    int getCost() const { return costConstruire; }
    std::string getProprietar() const { return proprietar; }

    virtual void ruleazaEfect(Armata& jucator) = 0;
};

class Oras : public Cladire {
private:
    int productieAur;
public:
    Oras(int x, int y, std::string prop)
        : Cladire(x, y, 0, prop), productieAur(10) {}

    void cresteProductie(int bonus) { productieAur += bonus; }

    void ruleazaEfect(Armata& jucator) override {
        jucator.adaugaAur(productieAur);
    }
};

class Mina : public Cladire {
public:
    Mina(int x, int y, std::string prop)
        : Cladire(x, y, 40, prop) {}

    void ruleazaEfect(Armata& jucator) override {
    }
};

class Port : public Cladire {
public:
    Port(int x, int y, std::string prop)
        : Cladire(x, y, 60, prop) {} // Portul costa 60 aur

    void ruleazaEfect(Armata& jucator) override {
        // Portul permite calatoria, mentenanta ar putea costa 1 aur/tura
        jucator.cheltuieAur(1);
    }
};

#endif