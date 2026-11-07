#ifndef JOC_H
#define JOC_H

#include <vector>
#include <string>
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
    bool areUnitateInApropiere(std::size_t idxFactiune, const Punct2D<int>& p, int distantaMax) const;

public:
    Joc(const Harta& hartaJoc, const Vreme& vremeInitiala);

    void adaugaFactiune(const Armata& armataNoua);
    bool deplaseazaUnitate(std::size_t idxFactiune, std::size_t idxUnitate, const Punct2D<int>& destinatie);
    bool executaAtac(std::size_t idxAtacator, std::size_t idxUnitAtac, std::size_t idxTinta, std::size_t idxUnitTinta);
    bool construiesteMina(std::size_t idxFactiune, const Punct2D<int>& pozitieMunte);
    bool recruteazaInFactiune(std::size_t idxFactiune, std::size_t idxSablon);
    bool recruteazaMagInFactiune(std::size_t idxFactiune);

    bool incepeCercetareInFactiune(std::size_t idxFactiune, const std::string& idTech);
    bool executaVindecareMag(std::size_t idxFactiune, std::size_t idxMag, std::size_t idxAliat);
    bool executaConversieMag(std::size_t idxFactiune, std::size_t idxMag, std::size_t idxInamic, std::size_t idxUnitInamica);

    void avanseazaTura();
    bool esteJocTerminat() const;
    int getCastigator() const;

    const Armata& getFactiune(std::size_t idxFactiune) const;
    Armata& getFactiune(std::size_t idxFactiune);

    void afiseazaClasament() const;
    void afiseazaHartaTactica() const;

    friend std::ostream& operator<<(std::ostream& os, const Joc& j);
};

#endif