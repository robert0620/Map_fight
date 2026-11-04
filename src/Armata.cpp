#include "Armata.h"
#include <fstream>
#include <algorithm>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

Armata::Armata(const std::string& nume, const std::string& fisier, int aurInitial, const Punct2D<int>& coordBaza)
    : numeFactiune(nume), fisierSursa(fisier), aur(aurInitial), numarOrase(1), productieOras(25), baza(coordBaza), unitati() {}

Armata::Armata(const Armata& alta)
    : numeFactiune(alta.numeFactiune),
      fisierSursa(alta.fisierSursa),
      aur(alta.aur),
      numarOrase(alta.numarOrase),
      productieOras(alta.productieOras),
      baza(alta.baza),
      unitati(alta.unitati) {}

Armata& Armata::operator=(const Armata& alta) {
    if (this != &alta) {
        numeFactiune = alta.numeFactiune;
        fisierSursa = alta.fisierSursa;
        aur = alta.aur;
        numarOrase = alta.numarOrase;
        productieOras = alta.productieOras;
        baza = alta.baza;
        unitati = alta.unitati;
    }
    return *this;
}

Armata::~Armata() {
    unitati.clear();
}

void Armata::eliminaUnitatiInvinse() {
    std::erase_if(unitati, [](const Unitate& u) { return !u.esteVie(); });
}

bool Armata::incarcaTrupeInitiale() {
    std::ifstream f(fisierSursa);
    if (!f.is_open()) {
        return false;
    }
    json date = json::parse(f, nullptr, false);
    if (date.is_discarded() || !date.contains("unitati")) {
        return false;
    }

    int offset = 0;
    for (const auto& elem : date["unitati"]) {
        std::string n = elem.value("nume", "Soldat");
        int v = elem.value("viata", 100);
        int a = elem.value("atac", 20);
        int r = elem.value("raza", 1);
        int c = elem.value("cost", 50);
        int directie = (baza.getY() == 0) ? 1 : -1;
        Punct2D<int> poz(baza.getX(), baza.getY() + (offset * directie));
        unitati.emplace_back(n, v, a, r, c, poz);
        offset++;
    }
    return !unitati.empty();
}

bool Armata::recruteazaUnitate(std::size_t indexSablon) {
    std::ifstream f(fisierSursa);
    if (!f.is_open()) {
        return false;
    }
    json date = json::parse(f, nullptr, false);
    if (date.is_discarded() || !date.contains("unitati")) {
        return false;
    }
    const auto& lista = date["unitati"];
    if (indexSablon >= lista.size()) {
        return false;
    }

    const auto& elem = lista[indexSablon];
    int costUnitate = elem.value("cost", 50);
    if (!cheltuieAur(costUnitate)) {
        return false;
    }

    unitati.emplace_back(
        elem.value("nume", "Soldat"),
        elem.value("viata", 100),
        elem.value("atac", 20),
        elem.value("raza", 1),
        costUnitate,
        baza
    );
    return true;
}

void Armata::colecteazaVenit() {
    aur += numarOrase * productieOras;
}

void Armata::cresteProductieMinerit(int bonus) {
    productieOras += bonus;
}

bool Armata::cheltuieAur(int suma) {
    if (aur >= suma) {
        aur -= suma;
        return true;
    }
    return false;
}

void Armata::curataArmata() {
    eliminaUnitatiInvinse();
}

const std::string& Armata::getNume() const {
    return numeFactiune;
}

int Armata::getAur() const {
    return aur;
}

int Armata::getNumarOrase() const {
    return numarOrase;
}

const Punct2D<int>& Armata::getBaza() const {
    return baza;
}

std::vector<Unitate>& Armata::getUnitati() {
    return unitati;
}

const std::vector<Unitate>& Armata::getUnitati() const {
    return unitati;
}

int Armata::calculeazaScorPutere() const {
    int scor = aur + (numarOrase * 100);
    for (const auto& u : unitati) {
        scor += u.getViata() + (u.getAtac() * 2);
    }
    return scor;
}

std::ostream& operator<<(std::ostream& os, const Armata& a) {
    os << "Armata " << a.numeFactiune << " (Baza: " << a.baza
       << " | Aur: " << a.aur << " | Orase: " << a.numarOrase
       << " |Scor: " << a.calculeazaScorPutere() << ")\n";
    for (const auto& u : a.unitati) {
        os << "  - " << u << "\n";
    }
    return os;
}