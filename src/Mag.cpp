#include "Mag.h"
#include <iostream>
#include <cmath>

Mag::Mag(const std::string& nume, int viata, int atac, int raza, int cost,
         const Punct2D<int>& startPoz, int id_jucator, int putereVindecare, bool conversieActiva)
    : Unitate(nume, viata, atac, raza, cost, startPoz, id_jucator),
      putereVindecareBaza(putereVindecare),
      bonusVindecareAplicat(0),
      conversieActiva(conversieActiva) {}

std::unique_ptr<Unitate> Mag::cloneaza() const {
    return std::make_unique<Mag>(*this);
}

int Mag::getPutereVindecare() const {
    return putereVindecareBaza + bonusVindecareAplicat;
}

void Mag::sincronizeazaBonusVindecare(int totalBonusVindecare) {
    if (totalBonusVindecare >= 0) {
        bonusVindecareAplicat = totalBonusVindecare;
    }
}

void Mag::setConversieActiva(bool activa) {
    conversieActiva = activa;
}

bool Mag::vindeca(Unitate& aliat) {
    if (!this->poateActiona() || !aliat.esteVie()) {
        return false;
    }

    if (this->getIdJucator() != aliat.getIdJucator()) {
        return false;
    }

    int dx = std::abs(this->getPozitie().getX() - aliat.getPozitie().getX());
    int dy = std::abs(this->getPozitie().getY() - aliat.getPozitie().getY());

    if (dx > this->getRaza() || dy > this->getRaza()) {
        return false;
    }

    if (aliat.getViata() >= aliat.getViataMaxima()) {
        return false;
    }

    int viataInainte = aliat.getViata();
    aliat.setViata(viataInainte + getPutereVindecare());
    this->marcheazaActionat();
    return true;
}

bool Mag::converteste(Unitate& inamic) {
    if (!conversieActiva || !this->poateActiona() || !inamic.esteVie()) {
        return false;
    }

    if (this->getIdJucator() == inamic.getIdJucator()) {
        return false;
    }

    int dx = std::abs(this->getPozitie().getX() - inamic.getPozitie().getX());
    int dy = std::abs(this->getPozitie().getY() - inamic.getPozitie().getY());

    if (dx > 1 || dy > 1) {
        return false;
    }

    inamic.setIdJucator(this->getIdJucator());
    this->marcheazaActionat();
    return true;
}

std::string Mag::getSimbolVisual() const {
    return "🧙";
}

bool Mag::esteMag() const {
    return true;
}

bool Mag::vindecaAliat(Unitate& aliat) {
    return vindeca(aliat);
}

bool Mag::convertesteInamic(Unitate& inamic) {
    return converteste(inamic);
}