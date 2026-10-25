#include "Joc.h"
#include <iostream>
#include <fstream>

Joc::Joc() : dwarfs("Dwarfs"), demons("Demons") {}

bool Joc::incarcaTrupe(Armata& armata, const std::string& caleFisier) {
    std::ifstream fin(caleFisier);
    if (!fin.is_open()) fin.open("../" + caleFisier);
    if (!fin.is_open()) fin.open("../../" + caleFisier);

    if (!fin.is_open()) {
        std::cerr << "[ERROR] Missing data file: " << caleFisier << "!\n";
        return false;
    }

    std::string nume;
    int hp, atk, raza;
    while (fin >> nume >> hp >> atk >> raza) {
        armata.recruteaza(Unitate(nume, hp, atk, raza));
    }
    fin.close();
    return true;
}

void Joc::start() {
    std::cout << "======================================\n";
    std::cout << "         MAP FIGHT - TERMINAL         \n";
    std::cout << "======================================\n\n";

    if (!incarcaTrupe(dwarfs, "assets/dwarfs.txt") || !incarcaTrupe(demons, "assets/demons.txt")) {
        std::cout << "[SYSTEM] Skipping battle simulation due to missing assets.\n";
        return;
    }

    int optiune = -1;
    while (optiune != 0) {
        std::cout << "\n=== MENIU DE LUPTA ===\n";
        std::cout << "1. Ataca cu Dwarf_Shield in Demon_Guard\n";
        std::cout << "2. Ataca cu Demon_King in Dwarf_Shield\n";
        std::cout << "3. Afiseaza starea armatelor\n";
        std::cout << "0. Iesire din simulare\n";
        std::cout << "Alege o actiune: ";
        
        if (!(std::cin >> optiune)) break;

        if (optiune == 1) {
            dwarfs.ordonaAtac(0, demons.getTrupa(0));
        } else if (optiune == 2) {
            demons.ordonaAtac(1, dwarfs.getTrupa(0));
        } else if (optiune == 3) {
            std::cout << "\n" << dwarfs << "\n" << demons << "\n";
        }
    }
    std::cout << "\n[SYSTEM] Simulare incheiata cu succes.\n";
}