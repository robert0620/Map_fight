#include "Unitate.h"
#include <algorithm>
#include <cmath>

Unitate::Unitate(const std::string& nume, int viata, int atac, int raza, int cost, const Punct2D<int>& startPoz)
    : nume(nume), viata(viata), atac(atac), raza(raza), cost(cost), pozitie(startPoz), xp(0), rang(Rang::RECRUT) {}

const std::string& Unitate::getNume() const {
    return nume;
}

int Unitate::getViata() const {
    return viata;
}

int Unitate::getAtac() const {
    return atac;
}

int Unitate::getRaza() const {
    return raza;
}

int Unitate::getCost() const {
    return cost;
}

const Punct2D<int>& Unitate::getPozitie() const {
    return pozitie;
}

void Unitate::setPozitie(const Punct2D<int>& nouaPoz) {
    pozitie = nouaPoz;
}

bool Unitate::esteVie() const {
    return viata > 0;
}

void Unitate::primesteDaune(int daune) {
    viata = std::max(0, viata - daune);
}

void Unitate::verificaPromovare() {
    if (xp >= 50 && rang == Rang::RECRUT) {
        rang = Rang::VETERAN;
        atac += 5;
        viata += 20;
    } else if (xp >= 120 && rang == Rang::VETERAN) {
        rang = Rang::ELITA;
        atac += 10;
        viata += 30;
    }
}

void Unitate::castigaXP(int cantitate) {
    xp += cantitate;
    verificaPromovare();
}

int Unitate::calculeazaPutereAtac(double modificatorVreme) const {
    if (raza > 1) {
        return static_cast<int>(std::round(atac * modificatorVreme));
    }
    return atac;
}

std::string Unitate::getNumeRang() const {
    switch (rang) {
        case Rang::RECRUT: return "Recrut";
        case Rang::VETERAN: return "Veteran";
        case Rang::ELITA: return "Elita";
        default: return "Necunoscut";
    }
}

std::ostream& operator<<(std::ostream& os, const Unitate& u) {
    os << u.nume << " [" << u.getNumeRang() << ", XP:" << u.xp
       << ", HP:" << u.viata << ", ATK:" << u.atac
       << ", Raza:" << u.raza << ", Poz:" << u.pozitie << "]";
    return os;
}