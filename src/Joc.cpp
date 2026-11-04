#include "Joc.h"
#include <fstream>
#include <nlohmann/json.hpp>
#include <fmt/core.h>
#include <fmt/color.h>

Joc::Joc(int liniiHarta, int coloaneHarta, TipBiom biom)
    : harta{liniiHarta, coloaneHarta, biom},
      dwarfs{"Dwarfs", 25},
      demons{"Demons", 25},
      rundaCurenta{1} {}

bool Joc::incarcaTrupe(Armata& armata, const std::string& caleFisier, int startX, int startY) {
    std::ifstream fin(caleFisier);
    if (!fin.is_open()) {
        return false;
    }
    std::string nume;
    int viata = 0;
    int atac = 0;
    int raza = 0;
    int offset = 0;
    while (fin >> nume >> viata >> atac >> raza) {
        int posY = (startY + offset) % 5;
        armata.recruteaza(Unitate(nume, viata, atac, raza, startX, posY));
        offset++;
    }
    return offset > 0;
}

bool Joc::incarcaTrupeDinJson(Armata& armata, const std::string& caleFisier) {
    std::ifstream fin(caleFisier);
    if (!fin.is_open()) {
        return false;
    }
    try {
        nlohmann::json date;
        fin >> date;
        if (!date.contains("unitati") || !date["unitati"].is_array()) {
            return false;
        }
        for (const auto& item : date["unitati"]) {
            std::string nume = item.value("nume", "Soldat");
            int viata = item.value("viata", 10);
            int atac = item.value("atac", 3);
            int raza = item.value("raza", 1);
            int x = item.value("x", 0);
            int y = item.value("y", 0);
            armata.recruteaza(Unitate(nume, viata, atac, raza, x, y));
        }
        return armata.getNumarTrupe() > 0;
    } catch (const std::exception& e) {
        fmt::print(fmt::fg(fmt::color::red), "[JSON ERROR] {}\n", e.what());
        return false;
    }
}

void Joc::start() {
    fmt::print(fmt::fg(fmt::color::light_green), "=== INITIALIZARE MAP FIGHT ===\n");

    if (!incarcaTrupeDinJson(dwarfs, "assets/dwarfs.json")) {
        if (!incarcaTrupe(dwarfs, "assets/dwarfs.txt", 0, 0)) {
            incarcaTrupe(dwarfs, "dwarfs.txt", 0, 0);
        }
    }

    if (!incarcaTrupeDinJson(demons, "assets/demons.json")) {
        if (!incarcaTrupe(demons, "assets/demons.txt", harta.getLinii() - 1, harta.getColoane() - 1)) {
            incarcaTrupe(demons, "demons.txt", harta.getLinii() - 1, harta.getColoane() - 1);
        }
    }

    Harta copieHarta = harta;
    copieHarta.plaseazaElement(1, 1, TipTeren::RESURSA);
    harta = copieHarta;
}

void Joc::executaRundaSimulata(int mutareX, int mutareY) {
    fmt::print(fmt::fg(fmt::color::magenta), "\n--- RUNDA {} ---\n", rundaCurenta);

    if (dwarfs.getNumarTrupe() > 0) {
        dwarfs.deplaseazaUnitate(0, mutareX, mutareY, harta);
    }

    executaMutareBot();

    if (dwarfs.getNumarTrupe() > 0 && demons.getNumarTrupe() > 0) {
        dwarfs.ordonaAtac(0, demons.getTrupa(0), harta);
        if (demons.getTrupa(0).esteViu()) {
            demons.ordonaAtac(0, dwarfs.getTrupa(0), harta);
        }
    }

    dwarfs.eliminaTrupeCazute();
    demons.eliminaTrupeCazute();

    dwarfs.colecteazaVenituriDinOrase();
    demons.colecteazaVenituriDinOrase();

    rundaCurenta++;
}

void Joc::executaMutareBot() {
    if (demons.getNumarTrupe() == 0) {
        return;
    }

    const Unitate& botUnit = demons.getTrupa(0);
    int curX = botUnit.getX();
    int curY = botUnit.getY();

    int tintaX = (curX > 0) ? curX - 1 : curX;
    int tintaY = (curY > 0) ? curY - 1 : curY;

    if (!demons.deplaseazaUnitate(0, tintaX, tintaY, harta)) {
        demons.deplaseazaUnitate(0, tintaX, curY, harta);
    }
}

std::string Joc::determinaLider() const {
    int scorDwarfs = dwarfs.calculeazaPutereTotala() + dwarfs.getAur() + dwarfs.getNumarOrase() * 20;
    int scorDemons = demons.calculeazaPutereTotala() + demons.getAur() + demons.getNumarOrase() * 20;
    if (scorDwarfs > scorDemons) {
        return dwarfs.getNumeJucator();
    }
    if (scorDemons > scorDwarfs) {
        return demons.getNumeJucator();
    }
    return "Egalitate";
}

const Harta& Joc::getHarta() const {
    return harta;
}

std::ostream& operator<<(std::ostream& os, const Joc& j) {
    os << "\n================ STARE JOC (Runda " << j.rundaCurenta << ") ================\n";
    os << j.harta << "\n";
    os << j.dwarfs << "\n";
    os << j.demons << "\n";
    os << "Lider curent: " << j.determinaLider() << "\n";
    os << "==========================================================\n";
    return os;
}