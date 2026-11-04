#include "Vreme.h"
#include <fmt/core.h>

Vreme::Vreme(TipVreme stareInitiala, int intensitateInitiala)
    : stareCurenta(stareInitiala), intensitate(intensitateInitiala) {}

double Vreme::calculeazaMultiplicatorBaza() const {
    switch (stareCurenta) {
        case TipVreme::SENIN: return 1.0;
        case TipVreme::PLOAIE: return 0.85;
        case TipVreme::CEATA: return 0.6;
        case TipVreme::VISCOL: return 0.5;
        default: return 1.0;
    }
}

void Vreme::actualizeaza(int turaCurenta) {
    int index = turaCurenta % 4;
    stareCurenta = static_cast<TipVreme>(index);
    intensitate = (turaCurenta % 3) + 1;
}

double Vreme::getModificatorAtacDistanta() const {
    return calculeazaMultiplicatorBaza();
}

int Vreme::getCostSuplimentarMiscare() const {
    if (stareCurenta == TipVreme::VISCOL) {
        return 2 * intensitate;
    }
    if (stareCurenta == TipVreme::PLOAIE) {
        return 1;
    }
    return 0;
}

std::string Vreme::getDescriere() const {
    switch (stareCurenta) {
        case TipVreme::SENIN: return fmt::format("Senin (Intensitate {})", intensitate);
        case TipVreme::PLOAIE: return fmt::format("Ploaie (Intensitate {})", intensitate);
        case TipVreme::CEATA: return fmt::format("Ceata (Intensitate {})", intensitate);
        case TipVreme::VISCOL: return fmt::format("Viscol (Intensitate {})", intensitate);
        default: return "Necunoscut";
    }
}

std::ostream& operator<<(std::ostream& os, const Vreme& v) {
    os << "[Vreme: " << v.getDescriere()
       << " | Coef. Atac Distanta: " << v.getModificatorAtacDistanta()
       << " | Penalizare Miscare: +" << v.getCostSuplimentarMiscare() << "]";
    return os;
}