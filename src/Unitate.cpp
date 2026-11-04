#include "Unitate.h"
#include <cmath>
#include <algorithm>
#include <fmt/core.h>
#include <fmt/color.h>

Unitate::Unitate()
    : nume{"Necunoscut"}, viata{10}, viataMaxima{10}, atac{2}, raza{1}, x{0}, y{0}, nivel{1} {}

Unitate::Unitate(const std::string& nume_, int viata_, int atac_, int raza_, int x_, int y_)
    : nume{nume_},
      viata{viata_ > 0 ? viata_ : 10},
      viataMaxima{viata_ > 0 ? viata_ : 10},
      atac{atac_ > 0 ? atac_ : 1},
      raza{raza_ > 0 ? raza_ : 1},
      x{x_},
      y{y_},
      nivel{1} {}

const std::string& Unitate::getNume() const {
    return nume;
}

int Unitate::getViata() const {
    return viata;
}

int Unitate::getViataMaxima() const {
    return viataMaxima;
}

int Unitate::getAtac() const {
    return atac;
}

int Unitate::getRaza() const {
    return raza;
}

int Unitate::getX() const {
    return x;
}

int Unitate::getY() const {
    return y;
}

int Unitate::getNivel() const {
    return nivel;
}

void Unitate::seteazaPozitie(int nouX, int nouY) {
    x = nouX;
    y = nouY;
}

int Unitate::calculeazaDistanta(const Unitate& alta) const {
    return std::abs(x - alta.x) + std::abs(y - alta.y);
}

void Unitate::primesteDaune(int damage) {
    int dauneReale = std::max(0, damage);
    viata -= dauneReale;
    if (viata < 0) {
        viata = 0;
    }
}

void Unitate::vindeca(int puncte) {
    if (esteViu() && puncte > 0) {
        viata = std::min(viataMaxima, viata + puncte);
    }
}

void Unitate::promoveaza() {
    if (esteViu()) {
        nivel++;
        viataMaxima += 5;
        viata = viataMaxima;
        atac += 2;
        fmt::print(fmt::fg(fmt::color::gold), "[LEVEL UP] {} a avansat la nivelul {} (HP: {}, ATK: {})!\n",
                   nume, nivel, viataMaxima, atac);
    }
}

void Unitate::ataca(Unitate& inamic, int bonusAparareInamic) const {
    if (!esteViu()) {
        fmt::print(fmt::fg(fmt::color::red), "[COMBAT] {} este eliminata si nu poate ataca!\n", nume);
        return;
    }
    int dist = calculeazaDistanta(inamic);
    if (dist <= raza) {
        int dauneCalculate = std::max(1, atac - bonusAparareInamic);
        fmt::print(fmt::fg(fmt::color::orange),
                   "[COMBAT] {} ataca {} de la distanta {} (Damage: {}, Reducere teren: {})!\n",
                   nume, inamic.nume, dist, dauneCalculate, bonusAparareInamic);
        inamic.primesteDaune(dauneCalculate);
    } else {
        fmt::print(fmt::fg(fmt::color::yellow),
                   "[COMBAT] {} este prea departe de {} (Distanta: {}, Raza: {})!\n",
                   inamic.nume, nume, dist, raza);
    }
}

bool Unitate::esteViu() const {
    return viata > 0;
}

bool Unitate::operator==(const Unitate& other) const {
    return nume == other.nume &&
           viata == other.viata &&
           viataMaxima == other.viataMaxima &&
           atac == other.atac &&
           raza == other.raza &&
           x == other.x &&
           y == other.y &&
           nivel == other.nivel;
}

bool Unitate::operator!=(const Unitate& other) const {
    return !(*this == other);
}

std::ostream& operator<<(std::ostream& os, const Unitate& u) {
    os << u.nume << " [Lvl: " << u.nivel
       << " | HP: " << u.viata << "/" << u.viataMaxima
       << " | ATK: " << u.atac
       << " | RNG: " << u.raza
       << " | Pos: (" << u.x << "," << u.y << ")]";
    return os;
}