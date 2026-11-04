#include "Joc.h"
#include <algorithm>
#include <fmt/core.h>

Joc::Joc(const Harta& hartaJoc, const Vreme& vremeInitiala)
    : harta(hartaJoc), vreme(vremeInitiala), factiuni(), turaCurenta(1) {}

bool Joc::suntIndiciValizi(std::size_t idxFactiune, std::size_t idxUnitate) const {
    if (idxFactiune >= factiuni.size()) {
        return false;
    }
    return idxUnitate < factiuni[idxFactiune].getUnitati().size();
}

void Joc::adaugaFactiune(const Armata& armataNoua) {
    factiuni.push_back(armataNoua);
}

bool Joc::deplaseazaUnitate(std::size_t idxFactiune, std::size_t idxUnitate, const Punct2D<int>& destinatie) {
    if (!suntIndiciValizi(idxFactiune, idxUnitate) || !harta.esteAccesibil(destinatie)) {
        return false;
    }
    Unitate& u = factiuni[idxFactiune].getUnitati()[idxUnitate];
    int distanta = u.getPozitie().distantaManhattan(destinatie);
    int costTotal = distanta + harta.getCostMiscare(destinatie) + vreme.getCostSuplimentarMiscare();

    if (costTotal <= 6) {
        u.setPozitie(destinatie);
        std::cout << fmt::format(">> {} s-a deplasat la ({}, {}).\n", u.getNume(), destinatie.getX(), destinatie.getY());
        return true;
    }
    return false;
}

bool Joc::executaAtac(std::size_t idxAtacator, std::size_t idxUnitAtac, std::size_t idxTinta, std::size_t idxUnitTinta) {
    if (idxAtacator == idxTinta || !suntIndiciValizi(idxAtacator, idxUnitAtac) || !suntIndiciValizi(idxTinta, idxUnitTinta)) {
        return false;
    }

    Unitate& atacator = factiuni[idxAtacator].getUnitati()[idxUnitAtac];
    Unitate& tinta = factiuni[idxTinta].getUnitati()[idxUnitTinta];

    int distanta = atacator.getPozitie().distantaManhattan(tinta.getPozitie());
    if (distanta > atacator.getRaza()) {
        return false;
    }

    int putere = atacator.calculeazaPutereAtac(vreme.getModificatorAtacDistanta());
    int aparare = harta.getBonusAparare(tinta.getPozitie());
    int dauneFinale = std::max(5, putere - aparare);

    tinta.primesteDaune(dauneFinale);
    atacator.castigaXP(30);
    std::cout << fmt::format(">> {} a atacat {} provocand {} daune!\n", atacator.getNume(), tinta.getNume(), dauneFinale);
    factiuni[idxTinta].curataArmata();
    return true;
}

bool Joc::construiesteMina(std::size_t idxFactiune, const Punct2D<int>& pozitieMunte) {
    if (idxFactiune >= factiuni.size()) {
        return false;
    }
    if (harta.getTeren(pozitieMunte) != TipTeren::MUNTE) {
        return false;
    }

    Armata& factiune = factiuni[idxFactiune];
    if (!factiune.cheltuieAur(40)) {
        return false;
    }

    int distantaFataDeBaza = factiune.getBaza().distantaManhattan(pozitieMunte);
    int bonus = (distantaFataDeBaza <= 5) ? 4 : 2;
    factiune.cresteProductieMinerit(bonus);
    return true;
}

bool Joc::recruteazaInFactiune(std::size_t idxFactiune, std::size_t idxSablon) {
    if (idxFactiune >= factiuni.size()) {
        return false;
    }
    if (factiuni[idxFactiune].recruteazaUnitate(idxSablon)) {
        const auto& u = factiuni[idxFactiune].getUnitati().back();
        std::cout << fmt::format(">> Unitate recrutata: {} (Cost: {} aur)\n", u.getNume(), u.getCost());
        return true;
    }
    return false;
}

void Joc::avanseazaTura() {
    turaCurenta++;
    vreme.actualizeaza(turaCurenta);
    for (auto& f : factiuni) {
        f.colecteazaVenit();
    }
}

void Joc::afiseazaClasament() const {
    std::vector<Armata> copieClasament = factiuni;
    std::sort(copieClasament.begin(), copieClasament.end(), [](const Armata& a, const Armata& b) {
        return a.calculeazaScorPutere() > b.calculeazaScorPutere();
    });

    std::cout << "\n=== CLASAMENT PUTERE TRIBURI (Tura " << turaCurenta << ") ===\n";
    for (std::size_t i = 0; i < copieClasament.size(); ++i) {
        std::cout << fmt::format("{}. {} - Scor: {} (Aur: {}, Orase: {})\n",
                                 i + 1,
                                 copieClasament[i].getNume(),
                                 copieClasament[i].calculeazaScorPutere(),
                                 copieClasament[i].getAur(),
                                 copieClasament[i].getNumarOrase());
    }
}

void Joc::afiseazaHartaTactica() const {
    std::cout << "\n--- HARTA TACTICA (1 = Jucator 1, 2 = Jucator 2, . = Campie, P = Padure, M = Munte, ~ = Apa) ---\n  ";
    for (int x = 0; x < harta.getLatime(); ++x) {
        std::cout << x << ' ';
    }
    std::cout << '\n';

    for (int y = 0; y < harta.getInaltime(); ++y) {
        std::cout << y << ' ';
        for (int x = 0; x < harta.getLatime(); ++x) {
            Punct2D<int> p(x, y);
            char simbol = harta.getSimbolTeren(harta.getTeren(p));
            for (std::size_t f = 0; f < factiuni.size(); ++f) {
                for (const auto& u : factiuni[f].getUnitati()) {
                    if (u.getPozitie() == p) {
                        simbol = static_cast<char>('1' + f);
                    }
                }
            }
            std::cout << simbol << ' ';
        }
        std::cout << '\n';
    }
}

std::ostream& operator<<(std::ostream& os, const Joc& j) {
    os << "\n=== STARE JOC (Tura " << j.turaCurenta << ") ===\n";
    os << j.vreme << "\n";
    os << j.harta;
    for (const auto& f : j.factiuni) {
        os << f;
    }
    return os;
}