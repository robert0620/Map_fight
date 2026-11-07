#include "Unitate.h"
#include <algorithm>
#include <cmath>

Unitate::Unitate(const std::string& nume, int viata, int atac, int raza, int cost,
                 const Punct2D<int>& startPoz, int id_jucator)
    : nume(nume),
      viata(viata),
      viataMaxima(viata),
      atac(atac),
      raza(raza),
      cost(cost),
      pozitie(startPoz),
      xp(0),
      rang(Rang::RECRUT),
      id_jucator(id_jucator),
      bonusViataAplicat(0),
      bonusAtacAplicat(0),
      bonusRazaAplicat(0),
      deplasatInTura(false),
      actionatInTura(false) {}

std::unique_ptr<Unitate> Unitate::cloneaza() const {
    return std::make_unique<Unitate>(*this);
}

const std::string& Unitate::getNume() const {
    return nume;
}

int Unitate::getViata() const {
    return viata;
}

int Unitate::getViataMaxima() const {
    return viataMaxima;
}

void Unitate::setViata(int viataNoua) {
    if (viataNoua > viataMaxima) {
        this->viata = viataMaxima;
    } else if (viataNoua < 0) {
        this->viata = 0;
    } else {
        this->viata = viataNoua;
    }
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

int Unitate::getIdJucator() const {
    return id_jucator;
}

void Unitate::setIdJucator(int idNou) {
    this->id_jucator = idNou;
}

bool Unitate::sePoateDeplasa() const {
    return esteVie() && !deplasatInTura;
}

bool Unitate::poateActiona() const {
    return esteVie() && !actionatInTura;
}

void Unitate::marcheazaDeplasat() {
    deplasatInTura = true;
}

void Unitate::marcheazaActionat() {
    actionatInTura = true;
}

void Unitate::reseteazaActiuniTura() {
    deplasatInTura = false;
    actionatInTura = false;
}

void Unitate::sincronizeazaBonusuriTech(int totalBonusViata, int totalBonusAtac, int totalBonusRaza) {
    int difViata = totalBonusViata - bonusViataAplicat;
    if (difViata != 0) {
        viataMaxima += difViata;
        viata = std::min(viataMaxima, std::max(1, viata + difViata));
        bonusViataAplicat = totalBonusViata;
    }

    int difAtac = totalBonusAtac - bonusAtacAplicat;
    if (difAtac != 0) {
        atac += difAtac;
        bonusAtacAplicat = totalBonusAtac;
    }

    int difRaza = totalBonusRaza - bonusRazaAplicat;
    if (difRaza != 0) {
        raza += difRaza;
        bonusRazaAplicat = totalBonusRaza;
    }
}

std::string Unitate::getSimbolVisual() const {
    return "⚔️";
}

bool Unitate::esteMag() const {
    return false;
}

bool Unitate::vindecaAliat(Unitate&) {
    return false;
}

bool Unitate::convertesteInamic(Unitate&) {
    return false;
}

void Unitate::primesteDaune(int daune) {
    viata = std::max(0, viata - daune);
}

void Unitate::verificaPromovare() {
    if (xp >= 50 && rang == Rang::RECRUT) {
        rang = Rang::VETERAN;
        atac += 5;
        viataMaxima += 20;
        viata += 20;
    } else if (xp >= 120 && rang == Rang::VETERAN) {
        rang = Rang::ELITA;
        atac += 10;
        viataMaxima += 30;
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
    os << u.getSimbolVisual() << " " << u.nume << " [" << u.getNumeRang() << ", XP:" << u.xp
       << ", HP:" << u.viata << "/" << u.viataMaxima << ", ATK:" << u.atac
       << ", Raza:" << u.raza << ", Poz:" << u.pozitie << "]";
    return os;
}