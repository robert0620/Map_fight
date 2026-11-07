#include "ArboreTehnologic.h"
#include "Mag.h"
#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>

ArboreTehnologic::ArboreTehnologic() : tehnologieInCercetare("") {}

bool ArboreTehnologic::incarcaDinFisier(const std::string& caleFisier) {
    std::ifstream fisier(caleFisier);
    if (!fisier.is_open()) {
        return false;
    }

    nlohmann::json date = nlohmann::json::parse(fisier, nullptr, false);
    if (date.is_discarded() || !date.is_array()) {
        return false;
    }

    tehnologii.clear();
    for (const auto& item : date) {
        Tehnologie tech(
            item.value("id", ""),
            item.value("nume", ""),
            item.value("cost", 50)
        );
        tech.seteazaBonusuriLupta(
            item.value("bonusViata", 0),
            item.value("bonusAtac", 0),
            item.value("bonusRaza", 0)
        );
        tech.seteazaEfecteMagice(
            item.value("bonusVindecare", 0),
            item.value("conversie", false),
            item.value("reducereCost", 0)
        );
        adaugaTehnologie(tech);
    }

    for (const auto& item : date) {
        std::string idTech = item.value("id", "");
        if (item.contains("cerinte") && item["cerinte"].is_array()) {
            for (const auto& cerinta : item["cerinte"]) {
                seteazaDependenta(idTech, cerinta.get<std::string>());
            }
        }
    }

    return !tehnologii.empty();
}

void ArboreTehnologic::initializeazaArborePolytopia() {
    if (incarcaDinFisier("assets/tehnologii.json")) {
        return;
    }

    Tehnologie catarare("TECH_CATARARE", "Catarare", 50);
    catarare.seteazaBonusuriLupta(0, 0, 1);

    Tehnologie meditatie("TECH_MEDITATIE", "Meditatie", 80);
    meditatie.seteazaBonusuriLupta(10, 0, 0);
    meditatie.seteazaEfecteMagice(15, false, 0);

    Tehnologie filozofie("TECH_FILOZOFIE", "Filozofie", 130);
    filozofie.seteazaEfecteMagice(20, true, 20);

    Tehnologie minerit("TECH_MINERIT", "Minerit", 80);
    minerit.seteazaBonusuriLupta(5, 5, 0);

    Tehnologie fierarie("TECH_FIERARIE", "Fierarie", 130);
    fierarie.seteazaBonusuriLupta(15, 12, 0);

    adaugaTehnologie(catarare);
    adaugaTehnologie(meditatie);
    adaugaTehnologie(filozofie);
    adaugaTehnologie(minerit);
    adaugaTehnologie(fierarie);

    seteazaDependenta("TECH_MEDITATIE", "TECH_CATARARE");
    seteazaDependenta("TECH_FILOZOFIE", "TECH_MEDITATIE");
    seteazaDependenta("TECH_MINERIT", "TECH_CATARARE");
    seteazaDependenta("TECH_FIERARIE", "TECH_MINERIT");
}

void ArboreTehnologic::adaugaTehnologie(const Tehnologie& tech) {
    tehnologii.insert({tech.getId(), tech});
}

void ArboreTehnologic::seteazaDependenta(const std::string& idTech, const std::string& idCerinta) {
    auto it = tehnologii.find(idTech);
    if (it != tehnologii.end()) {
        it->second.adaugaCerinta(idCerinta);
    }
}

int ArboreTehnologic::getReducereCostTotala() const {
    int reducere = 0;
    for (const auto& pereche : tehnologii) {
        if (pereche.second.esteDeblocata()) {
            reducere += pereche.second.getReducereCostProcent();
        }
    }
    return (reducere > 50) ? 50 : reducere;
}

int ArboreTehnologic::calculeazaCostEfectiv(const std::string& idTech) const {
    auto it = tehnologii.find(idTech);
    if (it == tehnologii.end()) {
        return 0;
    }
    int costBaza = it->second.getCost();
    int reducere = getReducereCostTotala();
    return costBaza - (costBaza * reducere / 100);
}

bool ArboreTehnologic::esteTehnologieDeblocata(const std::string& idTech) const {
    auto it = tehnologii.find(idTech);
    if (it == tehnologii.end()) {
        return false;
    }
    return it->second.esteDeblocata();
}

bool ArboreTehnologic::poateFiCercetata(const std::string& idTech) const {
    auto it = tehnologii.find(idTech);
    if (it == tehnologii.end()) {
        return false;
    }

    const Tehnologie& tech = it->second;
    if (tech.esteDeblocata()) {
        return false;
    }

    for (const std::string& idCerinta : tech.getCerinte()) {
        auto itCerinta = tehnologii.find(idCerinta);
        if (itCerinta == tehnologii.end() || !itCerinta->second.esteDeblocata()) {
            return false;
        }
    }

    return true;
}

bool ArboreTehnologic::incepeCercetarea(const std::string& idTech) {
    if (poateFiCercetata(idTech)) {
        tehnologieInCercetare = idTech;
        return true;
    }
    return false;
}

void ArboreTehnologic::proceseazaTura(int puncteStiintaGenerate) {
    if (tehnologieInCercetare.empty()) {
        return;
    }

    Tehnologie& tech = tehnologii.at(tehnologieInCercetare);
    int costEfectiv = calculeazaCostEfectiv(tehnologieInCercetare);

    tech.adaugaProgres(puncteStiintaGenerate);
    if (tech.getProgres() >= costEfectiv) {
        tech.deblocheaza();
        tehnologieInCercetare = "";
    }
}

bool ArboreTehnologic::areAbilitateConversie() const {
    for (const auto& pereche : tehnologii) {
        if (pereche.second.esteDeblocata() && pereche.second.permiteConversia()) {
            return true;
        }
    }
    return false;
}

int ArboreTehnologic::getBonusViataTotal() const {
    int total = 0;
    for (const auto& pereche : tehnologii) {
        if (pereche.second.esteDeblocata()) {
            total += pereche.second.getBonusViata();
        }
    }
    return total;
}

int ArboreTehnologic::getBonusAtacTotal() const {
    int total = 0;
    for (const auto& pereche : tehnologii) {
        if (pereche.second.esteDeblocata()) {
            total += pereche.second.getBonusAtac();
        }
    }
    return total;
}

int ArboreTehnologic::getBonusRazaTotal() const {
    int total = 0;
    for (const auto& pereche : tehnologii) {
        if (pereche.second.esteDeblocata()) {
            total += pereche.second.getBonusRaza();
        }
    }
    return total;
}

int ArboreTehnologic::getBonusVindecareTotal() const {
    int total = 0;
    for (const auto& pereche : tehnologii) {
        if (pereche.second.esteDeblocata()) {
            total += pereche.second.getBonusVindecare();
        }
    }
    return total;
}

void ArboreTehnologic::echipeazaUnitate(Unitate& unitate) const {
    unitate.sincronizeazaBonusuriTech(getBonusViataTotal(), getBonusAtacTotal(), getBonusRazaTotal());
    if (auto* magPtr = dynamic_cast<Mag*>(&unitate)) {
        magPtr->sincronizeazaBonusVindecare(getBonusVindecareTotal());
        magPtr->setConversieActiva(areAbilitateConversie());
    }
}

void ArboreTehnologic::afiseazaTehnologiiDisponibile() const {
    std::cout << "\n=== TEHNOLOGII DISPONIBILE PENTRU CERCETARE ===\n";
    for (const auto& pereche : tehnologii) {
        if (poateFiCercetata(pereche.first)) {
            const Tehnologie& t = pereche.second;
            std::cout << " -> [" << t.getId() << "] " << t.getNume()
                      << " | Cost: " << calculeazaCostEfectiv(t.getId()) << " stiinta\n";
        }
    }
}

void ArboreTehnologic::afiseazaStatusCercetare() const {
    std::cout << "Bonusuri Tech Active -> HP: +" << getBonusViataTotal()
              << ", ATK: +" << getBonusAtacTotal()
              << ", Raza: +" << getBonusRazaTotal()
              << ", Vindecare Mag: +" << getBonusVindecareTotal()
              << ", Conversie: " << (areAbilitateConversie() ? "DA" : "NU") << "\n";
}