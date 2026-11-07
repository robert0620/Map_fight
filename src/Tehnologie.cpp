#include "Tehnologie.h"

Tehnologie::Tehnologie(const std::string& id, const std::string& nume, const std::string& ramura, int nivel, int cost)
    : id(id),
      nume(nume),
      ramura(ramura),
      nivel(nivel),
      costStiinta(cost),
      progresCurent(0),
      deblocata(false),
      unitateDeblocata(""),
      bonusViata(0),
      bonusAtac(0),
      bonusRaza(0),
      bonusVindecare(0),
      deblocheazaConversia(false),
      reducereCostProcent(0) {}

const std::string& Tehnologie::getId() const { return id; }
const std::string& Tehnologie::getNume() const { return nume; }
const std::string& Tehnologie::getRamura() const { return ramura; }
int Tehnologie::getNivel() const { return nivel; }
int Tehnologie::getCost() const { return costStiinta; }
int Tehnologie::getProgres() const { return progresCurent; }
bool Tehnologie::esteDeblocata() const { return deblocata; }
const std::vector<std::string>& Tehnologie::getCerinte() const { return cerinte; }

const std::string& Tehnologie::getUnitateDeblocata() const { return unitateDeblocata; }
int Tehnologie::getBonusViata() const { return bonusViata; }
int Tehnologie::getBonusAtac() const { return bonusAtac; }
int Tehnologie::getBonusRaza() const { return bonusRaza; }
int Tehnologie::getBonusVindecare() const { return bonusVindecare; }
bool Tehnologie::permiteConversia() const { return deblocheazaConversia; }
int Tehnologie::getReducereCostProcent() const { return reducereCostProcent; }

void Tehnologie::seteazaUnitateDeblocata(const std::string& unitate) {
    this->unitateDeblocata = unitate;
}

void Tehnologie::seteazaBonusuriLupta(int viata, int atac, int raza) {
    this->bonusViata = viata;
    this->bonusAtac = atac;
    this->bonusRaza = raza;
}

void Tehnologie::seteazaEfecteMagice(int vindecare, bool conversie, int reducereCost) {
    this->bonusVindecare = vindecare;
    this->deblocheazaConversia = conversie;
    this->reducereCostProcent = reducereCost;
}

void Tehnologie::adaugaCerinta(const std::string& idCerinta) {
    cerinte.push_back(idCerinta);
}

void Tehnologie::adaugaProgres(int puncte) {
    if (!deblocata && puncte > 0) {
        progresCurent += puncte;
    }
}

void Tehnologie::deblocheaza() {
    deblocata = true;
    if (progresCurent < costStiinta) {
        progresCurent = costStiinta;
    }
}