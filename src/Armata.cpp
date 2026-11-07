#include "Armata.h"
#include <fstream>
#include <algorithm>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

Armata::Armata(const std::string& nume, const std::string& fisier, int aurInitial, const Punct2D<int>& coordBaza, int idFactiune)
    : numeFactiune(nume),
      fisierSursa(fisier),
      aur(aurInitial),
      numarOrase(1),
      productieOras(25),
      baza(coordBaza),
      idFactiune(idFactiune),
      unitati(),
      arboreTech() {
    arboreTech.initializeazaArborePolytopia();
}

Armata::Armata(const Armata& alta)
    : numeFactiune(alta.numeFactiune),
      fisierSursa(alta.fisierSursa),
      aur(alta.aur),
      numarOrase(alta.numarOrase),
      productieOras(alta.productieOras),
      baza(alta.baza),
      idFactiune(alta.idFactiune),
      unitati(),
      arboreTech(alta.arboreTech) {
    unitati.reserve(alta.unitati.size());
    for (const auto& u : alta.unitati) {
        if (u) {
            unitati.push_back(u->cloneaza());
        }
    }
}

Armata& Armata::operator=(const Armata& alta) {
    if (this != &alta) {
        numeFactiune = alta.numeFactiune;
        fisierSursa = alta.fisierSursa;
        aur = alta.aur;
        numarOrase = alta.numarOrase;
        productieOras = alta.productieOras;
        baza = alta.baza;
        idFactiune = alta.idFactiune;
        arboreTech = alta.arboreTech;

        unitati.clear();
        unitati.reserve(alta.unitati.size());
        for (const auto& u : alta.unitati) {
            if (u) {
                unitati.push_back(u->cloneaza());
            }
        }
    }
    return *this;
}

Armata::~Armata() {
    unitati.clear();
}

void Armata::eliminaUnitatiInvinse() {
    std::erase_if(unitati, [](const std::unique_ptr<Unitate>& u) {
        return !u || !u->esteVie();
    });
}

bool Armata::incarcaTrupeInitiale() {
    std::ifstream f(fisierSursa);
    if (!f.is_open()) {
        return false;
    }
    json date = json::parse(f, nullptr, false);
    if (date.is_discarded() || !date.contains("unitati")) {
        return false;
    }

    int offset = 0;
    for (const auto& elem : date["unitati"]) {
        std::string n = elem.value("nume", "Soldat");
        int v = elem.value("viata", 100);
        int a = elem.value("atac", 20);
        int r = elem.value("raza", 1);
        int c = elem.value("cost", 50);
        int directie = (baza.getY() == 0) ? 1 : -1;
        Punct2D<int> poz(baza.getX(), baza.getY() + (offset * directie));
        auto unitateNoua = std::make_unique<Unitate>(n, v, a, r, c, poz, idFactiune);
        arboreTech.echipeazaUnitate(*unitateNoua);
        unitati.push_back(std::move(unitateNoua));
        offset++;
    }
    return !unitati.empty();
}

bool Armata::recruteazaUnitate(std::size_t indexSablon) {
    std::ifstream f(fisierSursa);
    if (!f.is_open()) {
        return false;
    }
    json date = json::parse(f, nullptr, false);
    if (date.is_discarded() || !date.contains("unitati")) {
        return false;
    }
    const auto& lista = date["unitati"];
    if (indexSablon >= lista.size()) {
        return false;
    }

    const auto& elem = lista[indexSablon];
    int costUnitate = elem.value("cost", 50);
    if (!cheltuieAur(costUnitate)) {
        return false;
    }

    auto unitateNoua = std::make_unique<Unitate>(
        elem.value("nume", "Soldat"),
        elem.value("viata", 100),
        elem.value("atac", 20),
        elem.value("raza", 1),
        costUnitate,
        baza,
        idFactiune
    );
    arboreTech.echipeazaUnitate(*unitateNoua);
    unitati.push_back(std::move(unitateNoua));
    return true;
}

bool Armata::recruteazaMag() {
    if (!arboreTech.esteTehnologieDeblocata("TECH_MEDITATIE") &&
        !arboreTech.esteTehnologieDeblocata("TECH_FILOZOFIE")) {
        return false;
    }
    const int costMag = 60;
    if (!cheltuieAur(costMag)) {
        return false;
    }

    auto magNou = std::make_unique<Mag>(
        "Mag " + numeFactiune,
        80,
        15,
        2,
        costMag,
        baza,
        idFactiune,
        25,
        arboreTech.areAbilitateConversie()
    );
    arboreTech.echipeazaUnitate(*magNou);
    unitati.push_back(std::move(magNou));
    return true;
}

void Armata::adaugaUnitate(std::unique_ptr<Unitate> unitateNoua) {
    if (unitateNoua) {
        unitateNoua->setIdJucator(idFactiune);
        arboreTech.echipeazaUnitate(*unitateNoua);
        unitati.push_back(std::move(unitateNoua));
    }
}

std::unique_ptr<Unitate> Armata::extrageUnitate(std::size_t index) {
    if (index >= unitati.size()) {
        return nullptr;
    }
    std::unique_ptr<Unitate> extrasa = std::move(unitati[index]);
    unitati.erase(unitati.begin() + static_cast<std::ptrdiff_t>(index));
    return extrasa;
}

void Armata::colecteazaVenit() {
    aur += numarOrase * productieOras;
}

void Armata::avanseazaCercetare(int puncteStiinta) {
    arboreTech.proceseazaTura(puncteStiinta);
    sincronizeazaTehnologiiPeTrupe();
}

void Armata::sincronizeazaTehnologiiPeTrupe() {
    for (auto& u : unitati) {
        if (u) {
            arboreTech.echipeazaUnitate(*u);
        }
    }
}

void Armata::reseteazaActiuniTrupe() {
    for (auto& u : unitati) {
        if (u) {
            u->reseteazaActiuniTura();
        }
    }
}

void Armata::cresteProductieMinerit(int bonus) {
    productieOras += bonus;
}

bool Armata::cheltuieAur(int suma) {
    if (aur >= suma) {
        aur -= suma;
        return true;
    }
    return false;
}

void Armata::curataArmata() {
    eliminaUnitatiInvinse();
}

const std::string& Armata::getNume() const {
    return numeFactiune;
}

int Armata::getAur() const {
    return aur;
}

int Armata::getNumarOrase() const {
    return numarOrase;
}

void Armata::setIdFactiune(int idNou) {
    idFactiune = idNou;
    for (auto& u : unitati) {
        if (u) {
            u->setIdJucator(idNou);
        }
    }
}

const Punct2D<int>& Armata::getBaza() const {
    return baza;
}

std::size_t Armata::getNumarUnitati() const {
    return unitati.size();
}

Unitate& Armata::getUnitate(std::size_t index) {
    return *unitati.at(index);
}

const Unitate& Armata::getUnitate(std::size_t index) const {
    return *unitati.at(index);
}

bool Armata::areUnitatiVii() const {
    for (const auto& u : unitati) {
        if (u && u->esteVie()) {
            return true;
        }
    }
    return false;
}

ArboreTehnologic& Armata::getArboreTehnologic() {
    return arboreTech;
}

const ArboreTehnologic& Armata::getArboreTehnologic() const {
    return arboreTech;
}

int Armata::calculeazaScorPutere() const {
    int scor = aur + (numarOrase * 100);
    for (const auto& u : unitati) {
        if (u) {
            scor += u->getViata() + (u->getAtac() * 2);
        }
    }
    return scor;
}

std::ostream& operator<<(std::ostream& os, const Armata& a) {
    os << "Armata " << a.numeFactiune << " (Baza: " << a.baza
       << " | Aur: " << a.aur << " | Orase: " << a.numarOrase
       << " | Scor: " << a.calculeazaScorPutere() << ")\n";
    for (std::size_t i = 0; i < a.unitati.size(); ++i) {
        if (a.unitati[i]) {
            os << "  [" << i << "] " << *a.unitati[i] << "\n";
        }
    }
    return os;
}