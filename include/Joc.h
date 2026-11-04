#ifndef JOC_H
#define JOC_H

#include <vector>
#include <iostream>
#include "Harta.h"
#include "Armata.h"
#include "Vreme.h"

class Joc {
private:
    Harta harta;
    Vreme vreme;
    std::vector<Armata> factiuni;
    int turaCurenta;

    bool suntIndiciValizi(std::size_t idxFactiune, std::size_t idxUnitate) const;

public:
    Joc(const Harta& hartaJoc, const Vreme& vremeInitiala);

    void adaugaFactiune(const Armata& armataNoua);
    bool deplaseazaUnitate(std::size_t idxFactiune, std::size_t idxUnitate, const Punct2D<int>& destinatie);
    bool executaAtac(std::size_t idxAtacator, std::size_t idxUnitAtac, std::size_t idxTinta, std::size_t idxUnitTinta);
    bool construiesteMina(std::size_t idxFactiune, const Punct2D<int>& pozitieMunte);
    bool recruteazaInFactiune(std::size_t idxFactiune, std::size_t idxSablon);
    void avanseazaTura();
    void afiseazaClasament() const;
    void afiseazaHartaTactica() const;

    friend std::ostream& operator<<(std::ostream& os, const Joc& j);
};

#endif