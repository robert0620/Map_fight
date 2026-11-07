#ifndef ARBORE_TEHNOLOGIC_H
#define ARBORE_TEHNOLOGIC_H

#include <string>
#include <map>
#include <vector>
#include "Tehnologie.h"
#include "Unitate.h"

class ArboreTehnologic {
private:
    std::map<std::string, Tehnologie> tehnologii;
    std::string tehnologieInCercetare;

public:
    ArboreTehnologic();

    bool incarcaDinFisier(const std::string& caleFisier);
    void initializeazaArborePolytopia();
    void adaugaTehnologie(const Tehnologie& tech);
    void seteazaDependenta(const std::string& idTech, const std::string& idCerinta);

    int calculeazaCostEfectiv(const std::string& idTech) const;
    bool esteTehnologieDeblocata(const std::string& idTech) const;
    bool poateFiCercetata(const std::string& idTech) const;
    bool incepeCercetarea(const std::string& idTech);
    void proceseazaTura(int puncteStiintaGenerate);

    bool areAbilitateConversie() const;
    int getBonusViataTotal() const;
    int getBonusAtacTotal() const;
    int getBonusRazaTotal() const;
    int getBonusVindecareTotal() const;
    int getReducereCostTotala() const;

    void echipeazaUnitate(Unitate& unitate) const;

    void afiseazaTehnologiiDisponibile() const;
    void afiseazaStatusCercetare() const;
};

#endif