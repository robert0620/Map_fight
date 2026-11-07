#include "Joc.h"
#include <algorithm>
#include <fmt/core.h>

Joc::Joc(const Harta& hartaJoc, const Vreme& vremeInitiala)
    : harta(hartaJoc), vreme(vremeInitiala), factiuni(), turaCurenta(1) {}

bool Joc::suntIndiciValizi(std::size_t idxFactiune, std::size_t idxUnitate) const {
    if (idxFactiune >= factiuni.size()) {
        return false;
    }
    return idxUnitate < factiuni[idxFactiune].getNumarUnitati();
}

bool Joc::areUnitateInApropiere(std::size_t idxFactiune, const Punct2D<int>& p, int distantaMax) const {
    if (idxFactiune >= factiuni.size()) {
        return false;
    }
    const Armata& armata = factiuni[idxFactiune];
    for (std::size_t i = 0; i < armata.getNumarUnitati(); ++i) {
        const Unitate& u = armata.getUnitate(i);
        if (u.esteVie() && u.getPozitie().distantaManhattan(p) <= distantaMax) {
            return true;
        }
    }
    return false;
}

void Joc::adaugaFactiune(const Armata& armataNoua) {
    factiuni.push_back(armataNoua);
    factiuni.back().setIdFactiune(static_cast<int>(factiuni.size()));
    factiuni.back().sincronizeazaTehnologiiPeTrupe();
}

bool Joc::deplaseazaUnitate(std::size_t idxFactiune, std::size_t idxUnitate, const Punct2D<int>& destinatie) {
    if (!suntIndiciValizi(idxFactiune, idxUnitate) || !harta.esteAccesibil(destinatie)) {
        return false;
    }

    Unitate& u = factiuni[idxFactiune].getUnitate(idxUnitate);
    if (!u.sePoateDeplasa()) {
        return false;
    }

    int distanta = u.getPozitie().distantaManhattan(destinatie);
    int costTotal = distanta + harta.getCostMiscare(destinatie) + vreme.getCostSuplimentarMiscare();
    int pasiMaximiPermisi = 6 - harta.getCostMiscare(destinatie) - vreme.getCostSuplimentarMiscare();

    if (costTotal <= 6 && harta.existaTraseuAccesibil(u.getPozitie(), destinatie, pasiMaximiPermisi)) {
        u.setPozitie(destinatie);
        u.marcheazaDeplasat();
        std::cout << fmt::format(">> {} s-a deplasat la ({}, {}).\n", u.getNume(), destinatie.getX(), destinatie.getY());
        return true;
    }
    return false;
}

bool Joc::executaAtac(std::size_t idxAtacator, std::size_t idxUnitAtac, std::size_t idxTinta, std::size_t idxUnitTinta) {
    if (idxAtacator == idxTinta || !suntIndiciValizi(idxAtacator, idxUnitAtac) || !suntIndiciValizi(idxTinta, idxUnitTinta)) {
        return false;
    }

    Unitate& atacator = factiuni[idxAtacator].getUnitate(idxUnitAtac);
    Unitate& tinta = factiuni[idxTinta].getUnitate(idxUnitTinta);

    if (!atacator.poateActiona() || !tinta.esteVie()) {
        return false;
    }

    int distanta = atacator.getPozitie().distantaManhattan(tinta.getPozitie());
    if (distanta > atacator.getRaza()) {
        return false;
    }

    int putere = atacator.calculeazaPutereAtac(vreme.getModificatorAtacDistanta());
    int aparare = harta.getBonusAparare(tinta.getPozitie());
    int dauneFinale = std::max(5, putere - aparare);

    tinta.primesteDaune(dauneFinale);
    atacator.castigaXP(30);
    atacator.marcheazaActionat();
    std::cout << fmt::format(">> {} a atacat {} provocand {} daune!\n", atacator.getNume(), tinta.getNume(), dauneFinale);
    factiuni[idxTinta].curataArmata();
    return true;
}

bool Joc::construiesteMina(std::size_t idxFactiune, const Punct2D<int>& pozitieMunte) {
    if (idxFactiune >= factiuni.size()) {
        return false;
    }
    if (harta.getTeren(pozitieMunte) != TipTeren::MUNTE || harta.areMina(pozitieMunte)) {
        return false;
    }
    if (!areUnitateInApropiere(idxFactiune, pozitieMunte, 1)) {
        return false;
    }

    Armata& factiune = factiuni[idxFactiune];
    if (!factiune.cheltuieAur(40)) {
        return false;
    }

    if (!harta.plaseazaMina(pozitieMunte)) {
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
        std::size_t ultimIdx = factiuni[idxFactiune].getNumarUnitati() - 1;
        const Unitate& u = factiuni[idxFactiune].getUnitate(ultimIdx);
        std::cout << fmt::format(">> Unitate recrutata: {} (Cost: {} aur)\n", u.getNume(), u.getCost());
        return true;
    }
    return false;
}

bool Joc::recruteazaMagInFactiune(std::size_t idxFactiune) {
    if (idxFactiune >= factiuni.size()) {
        return false;
    }
    if (factiuni[idxFactiune].recruteazaMag()) {
        std::size_t ultimIdx = factiuni[idxFactiune].getNumarUnitati() - 1;
        const Unitate& u = factiuni[idxFactiune].getUnitate(ultimIdx);
        std::cout << fmt::format(">> Mag recrutat in armata: {} (Simbol: {})\n", u.getNume(), u.getSimbolVisual());
        return true;
    }
    return false;
}

bool Joc::incepeCercetareInFactiune(std::size_t idxFactiune, const std::string& idTech) {
    if (idxFactiune >= factiuni.size()) {
        return false;
    }
    return factiuni[idxFactiune].getArboreTehnologic().incepeCercetarea(idTech);
}

bool Joc::executaVindecareMag(std::size_t idxFactiune, std::size_t idxMag, std::size_t idxAliat) {
    if (!suntIndiciValizi(idxFactiune, idxMag) || !suntIndiciValizi(idxFactiune, idxAliat) || idxMag == idxAliat) {
        return false;
    }
    Unitate& mag = factiuni[idxFactiune].getUnitate(idxMag);
    Unitate& aliat = factiuni[idxFactiune].getUnitate(idxAliat);
    return mag.vindecaAliat(aliat);
}

bool Joc::executaConversieMag(std::size_t idxFactiune, std::size_t idxMag, std::size_t idxInamic, std::size_t idxUnitInamica) {
    if (idxFactiune == idxInamic || !suntIndiciValizi(idxFactiune, idxMag) || !suntIndiciValizi(idxInamic, idxUnitInamica)) {
        return false;
    }
    Unitate& mag = factiuni[idxFactiune].getUnitate(idxMag);
    Unitate& tinta = factiuni[idxInamic].getUnitate(idxUnitInamica);

    if (!mag.convertesteInamic(tinta)) {
        return false;
    }

    std::unique_ptr<Unitate> unitateConvertita = factiuni[idxInamic].extrageUnitate(idxUnitInamica);
    if (unitateConvertita) {
        factiuni[idxFactiune].adaugaUnitate(std::move(unitateConvertita));
        return true;
    }
    return false;
}

void Joc::avanseazaTura() {
    turaCurenta++;
    vreme.actualizeaza(turaCurenta);
    for (auto& f : factiuni) {
        f.colecteazaVenit();
        f.avanseazaCercetare(40);
        f.reseteazaActiuniTrupe();
    }
}

bool Joc::esteJocTerminat() const {
    if (factiuni.size() < 2) {
        return false;
    }
    int factiuniActive = 0;
    for (const auto& f : factiuni) {
        if (f.areUnitatiVii()) {
            factiuniActive++;
        }
    }
    return factiuniActive <= 1;
}

int Joc::getCastigator() const {
    for (std::size_t i = 0; i < factiuni.size(); ++i) {
        if (factiuni[i].areUnitatiVii()) {
            return static_cast<int>(i);
        }
    }
    return -1;
}

const Armata& Joc::getFactiune(std::size_t idxFactiune) const {
    return factiuni.at(idxFactiune);
}

Armata& Joc::getFactiune(std::size_t idxFactiune) {
    return factiuni.at(idxFactiune);
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
    std::cout << "\n--- HARTA TACTICA (1/M = Jucator 1, 2/W = Jucator 2, $ = Mina, . = Campie, P = Padure, M = Munte, ~ = Apa) ---\n  ";
    for (int x = 0; x < harta.getLatime(); ++x) {
        std::cout << x << ' ';
    }
    std::cout << '\n';

    for (int y = 0; y < harta.getInaltime(); ++y) {
        std::cout << y << ' ';
        for (int x = 0; x < harta.getLatime(); ++x) {
            Punct2D<int> p(x, y);
            char simbol = harta.areMina(p) ? '$' : Harta::getSimbolTeren(harta.getTeren(p));
            for (std::size_t f = 0; f < factiuni.size(); ++f) {
                for (std::size_t uIdx = 0; uIdx < factiuni[f].getNumarUnitati(); ++uIdx) {
                    const Unitate& u = factiuni[f].getUnitate(uIdx);
                    if (u.esteVie() && u.getPozitie() == p) {
                        if (u.esteMag()) {
                            simbol = (f == 0) ? 'M' : 'W';
                        } else {
                            simbol = static_cast<char>('1' + f);
                        }
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