#include "Unitate.h"

Unitate::Unitate(const std::string& nume_, int viata_, int atac_, int raza_)
    : nume{nume_}, viata{viata_}, atac{atac_}, raza{raza_} {}

Unitate::Unitate(const Unitate& other)
    : nume{other.nume}, viata{other.viata}, atac{other.atac}, raza{other.raza} {}

Unitate& Unitate::operator=(const Unitate& other) {
    if (this != &other) {
        nume = other.nume;
        viata = other.viata;
        atac = other.atac;
        raza = other.raza;
    }
    return *this;
}

Unitate::~Unitate() {}

const std::string& Unitate::getNume() const { return nume; }


void Unitate::primesteDaune(int damage) {
    viata -= damage;
    if (viata < 0) viata = 0;
    std::cout << " > " << nume << " took " << damage << " damage. HP left: " << viata << "\n";
}

void Unitate::ataca(Unitate& inamic) const {
    if (!esteViu()) {
        std::cout << " > " << nume << " is destroyed and cannot attack!\n";
        return;
    }
    if (!inamic.esteViu()) {
        std::cout << " > " << inamic.getNume() << " is already destroyed!\n";
        return;
    }
    std::cout << " > " << nume << " attacks " << inamic.getNume() << "!\n";
    inamic.primesteDaune(this->atac);
}

bool Unitate::esteViu() const {
    return viata > 0;
}

bool Unitate::operator==(const Unitate& other) const {
    return nume == other.nume && viata == other.viata &&
           atac == other.atac && raza == other.raza;
}

bool Unitate::operator!=(const Unitate& other) const {
    return !(*this == other);
}

std::ostream& operator<<(std::ostream& os, const Unitate& u) {
    os << "[" << u.nume << " | HP: " << u.viata << " | ATK: " << u.atac << " | RNG: " << u.raza << "]";
    return os;
}