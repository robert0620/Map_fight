#ifndef UNITATE_H
#define UNITATE_H

#include <string>
#include <iostream>
#include <memory>
#include "Punct2D.h"

enum class Rang { RECRUT, VETERAN, ELITA };

class Unitate {
private:
    std::string nume;
    int viata;
    int viataMaxima;
    int atac;
    int raza;
    int cost;
    Punct2D<int> pozitie;
    int xp;
    Rang rang;
    int id_jucator;

    int bonusViataAplicat;
    int bonusAtacAplicat;
    int bonusRazaAplicat;

    bool deplasatInTura;
    bool actionatInTura;

    void verificaPromovare();

public:
    Unitate(const std::string& nume, int viata, int atac, int raza, int cost,
            const Punct2D<int>& startPoz = Punct2D<int>(0, 0), int id_jucator = 0);

    virtual ~Unitate() = default;

    virtual std::unique_ptr<Unitate> cloneaza() const;

    const std::string& getNume() const;
    int getViata() const;
    int getViataMaxima() const;
    void setViata(int viataNoua);

    int getAtac() const;
    int getRaza() const;
    int getCost() const;
    const Punct2D<int>& getPozitie() const;
    void setPozitie(const Punct2D<int>& nouaPoz);
    bool esteVie() const;

    int getIdJucator() const;
    void setIdJucator(int idNou);

    bool sePoateDeplasa() const;
    bool poateActiona() const;
    void marcheazaDeplasat();
    void marcheazaActionat();
    void reseteazaActiuniTura();

    void sincronizeazaBonusuriTech(int totalBonusViata, int totalBonusAtac, int totalBonusRaza);

    virtual std::string getSimbolVisual() const;
    virtual bool esteMag() const;
    virtual bool vindecaAliat(Unitate& aliat);
    virtual bool convertesteInamic(Unitate& inamic);

    void primesteDaune(int daune);
    void castigaXP(int cantitate);
    int calculeazaPutereAtac(double modificatorVreme) const;
    std::string getNumeRang() const;

    friend std::ostream& operator<<(std::ostream& os, const Unitate& u);
};

#endif