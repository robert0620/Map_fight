#include "Armata.h"
#include <iostream>

Armata::Armata(const std::string& numeJucator_) : numeJucator{numeJucator_} {}

void Armata::recruteaza(const Unitate& trupaNoua) {
    trupe.push_back(trupaNoua);
    std::cout << "[RECRUIT] " << numeJucator << " enlisted: " << trupaNoua.getNume() << "\n";
}

void Armata::ordonaAtac(size_t indexTrupa, Unitate& inamic) const {
    if (indexTrupa < trupe.size()) {
        std::cout << "[ACTION] " << numeJucator << " commander issues an attack order!\n";
        trupe[indexTrupa].ataca(inamic);
    } else {
        std::cout << "[ERROR] Unit does not exist in " << numeJucator << "'s army!\n";
    }
}

Unitate& Armata::getTrupa(size_t index) {
    return trupe.at(index);
}

std::ostream& operator<<(std::ostream& os, const Armata& a) {
    os << "=== Faction: " << a.numeJucator << " ===\n";
    for (const auto& trupa : a.trupe) {
        os << " - " << trupa << "\n";
    }
    return os;
}
