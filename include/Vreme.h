#ifndef VREME_H
#define VREME_H

#include <string>
#include <iostream>

enum class TipVreme { SENIN, PLOAIE, CEATA, VISCOL };

class Vreme {
private:
    TipVreme stareCurenta;
    int intensitate;

    double calculeazaMultiplicatorBaza() const;

public:
    explicit Vreme(TipVreme stareInitiala = TipVreme::SENIN, int intensitateInitiala = 1);

    void actualizeaza(int turaCurenta);
    double getModificatorAtacDistanta() const;
    int getCostSuplimentarMiscare() const;
    std::string getDescriere() const;

    friend std::ostream& operator<<(std::ostream& os, const Vreme& v);
};

#endif