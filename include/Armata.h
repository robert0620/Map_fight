#ifndef ARMATA_H
#define ARMATA_H

#include <string>
#include <vector>
#include <memory>
#include <iostream>
#include "Unitate.h"
#include "Mag.h"
#include "ArboreTehnologic.h"
#include "Punct2D.h"

class Armata {
private:
    std::string numeFactiune;
    std::string fisierSursa;
    int aur;
    int numarOrase;
    int productieOras;
    Punct2D<int> baza;
    int idFactiune;
    std::vector<std::unique_ptr<Unitate>> unitati;
    ArboreTehnologic arboreTech;

    void eliminaUnitatiInvinse();

public:
    Armata(const std::string& nume, const std::string& fisier, int aurInitial, const Punct2D<int>& coordBaza, int idFactiune = 1);
    Armata(const Armata& alta);
    Armata& operator=(const Armata& alta);
    ~Armata();

    bool incarcaTrupeInitiale();
    bool recruteazaUnitate(std::size_t indexSablon);
    bool recruteazaMag();
    void adaugaUnitate(std::unique_ptr<Unitate> unitateNoua);
    std::unique_ptr<Unitate> extrageUnitate(std::size_t index);

    void colecteazaVenit();
    void avanseazaCercetare(int puncteStiinta);
    void sincronizeazaTehnologiiPeTrupe();
    void reseteazaActiuniTrupe();

    void cresteProductieMinerit(int bonus);
    bool cheltuieAur(int suma);
    void curataArmata();

    const std::string& getNume() const;
    int getAur() const;
    int getNumarOrase() const;
    int getIdFactiune() const;
    void setIdFactiune(int idNou);
    const Punct2D<int>& getBaza() const;

    std::size_t getNumarUnitati() const;
    Unitate& getUnitate(std::size_t index);
    const Unitate& getUnitate(std::size_t index) const;
    bool areUnitatiVii() const;

    ArboreTehnologic& getArboreTehnologic();
    const ArboreTehnologic& getArboreTehnologic() const;

    int calculeazaScorPutere() const;

    friend std::ostream& operator<<(std::ostream& os, const Armata& a);
};

#endif