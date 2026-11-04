#include "Armata.h"
#include <algorithm>
#include <numeric>
#include <fmt/core.h>
#include <fmt/color.h>

Armata::Armata(const std::string& numeJucator_, int aurInitial_)
    : numeJucator{numeJucator_}, trupe{}, aur{aurInitial_}, numarOrase{1} {}

void Armata::recruteaza(const Unitate& trupaNoua) {
    trupe.push_back(trupaNoua);
    fmt::print(fmt::fg(fmt::color::green), "[RECRUIT] {} a recrutat: {} la pozitia ({}, {})\n",
               numeJucator, trupaNoua.getNume(), trupaNoua.getX(), trupaNoua.getY());
}

void Armata::ordonaAtac(size_t indexTrupa, Unitate& inamic, const Harta& harta) {
    if (indexTrupa < trupe.size()) {
        std::cout << "[ACTION] Comandantul " << numeJucator << " ordona atacul!\n";
        int bonusTeren = harta.getBonusAparareLa(inamic.getX(), inamic.getY());
        trupe[indexTrupa].ataca(inamic, bonusTeren);
        if (!inamic.esteViu()) {
            trupe[indexTrupa].promoveaza();
        }
    } else {
        std::cout << "[ERROR] Unitatea cu indexul " << indexTrupa << " nu exista in armata " << numeJucator << "!\n";
    }
}

bool Armata::deplaseazaUnitate(size_t indexTrupa, int nouX, int nouY, Harta& harta) {
    if (indexTrupa >= trupe.size() || !trupe[indexTrupa].esteViu()) {
        return false;
    }
    if (!harta.estePozitieAccesibila(nouX, nouY)) {
        fmt::print(fmt::fg(fmt::color::red),
                   "[MOVE] Pozitia ({}, {}) este inaccesibila pentru {}!\n",
                   nouX, nouY, trupe[indexTrupa].getNume());
        return false;
    }

    trupe[indexTrupa].seteazaPozitie(nouX, nouY);
    fmt::print(fmt::fg(fmt::color::light_blue), "[MOVE] {} ({}) s-a deplasat la ({}, {}).\n",
               trupe[indexTrupa].getNume(), numeJucator, nouX, nouY);

    int aurCastigat = harta.colecteazaResursa(nouX, nouY);
    if (aurCastigat > 0) {
        aur += aurCastigat;
    }

    if (harta.convertesteTribInOras(nouX, nouY, numeJucator)) {
        numarOrase++;
        aur += 15;
    }

    return true;
}

void Armata::colecteazaVenituriDinOrase() {
    int venit = numarOrase * 5;
    aur += venit;
    fmt::print(fmt::fg(fmt::color::gold), "[ECONOMY] {} colecteaza {} aur din {} orase (Total Aur: {}).\n",
               numeJucator, venit, numarOrase, aur);
}

bool Armata::vindecaTrupaCuAur(size_t indexTrupa, int costAur, int puncteViata) {
    if (indexTrupa >= trupe.size() || aur < costAur || costAur <= 0) {
        return false;
    }
    aur -= costAur;
    trupe[indexTrupa].vindeca(puncteViata);
    return true;
}

void Armata::eliminaTrupeCazute() {
    auto it = std::remove_if(trupe.begin(), trupe.end(), [](const Unitate& u) {
        return !u.esteViu();
    });
    if (it != trupe.end()) {
        trupe.erase(it, trupe.end());
    }
}

int Armata::calculeazaPutereTotala() const {
    return std::accumulate(trupe.begin(), trupe.end(), 0, [](int suma, const Unitate& u) {
        return suma + (u.esteViu() ? (u.getViata() + u.getAtac() * 2) : 0);
    });
}

Unitate& Armata::getTrupa(size_t index) {
    return trupe.at(index);
}

const Unitate& Armata::getTrupa(size_t index) const {
    return trupe.at(index);
}

size_t Armata::getNumarTrupe() const {
    return trupe.size();
}

int Armata::getAur() const {
    return aur;
}

int Armata::getNumarOrase() const {
    return numarOrase;
}

const std::string& Armata::getNumeJucator() const {
    return numeJucator;
}

bool Armata::maiAreTrupeVii() const {
    return std::any_of(trupe.begin(), trupe.end(), [](const Unitate& u) {
        return u.esteViu();
    });
}

std::ostream& operator<<(std::ostream& os, const Armata& a) {
    os << "=== Factiune: " << a.numeJucator
       << " (Aur: " << a.aur
       << " | Orase: " << a.numarOrase
       << " | Putere: " << a.calculeazaPutereTotala() << ") ===\n";
    for (const auto& trupa : a.trupe) {
        os << " - " << trupa << "\n";
    }
    return os;
}