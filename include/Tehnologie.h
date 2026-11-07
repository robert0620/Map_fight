#ifndef TEHNOLOGIE_H
#define TEHNOLOGIE_H

#include <string>
#include <vector>

class Tehnologie {
private:
    std::string id;
    std::string nume;
    std::string ramura;
    int nivel;
    int costStiinta;
    int progresCurent;
    bool deblocata;
    std::vector<std::string> cerinte;

    std::string unitateDeblocata;
    int bonusViata;
    int bonusAtac;
    int bonusRaza;
    int bonusVindecare;
    bool deblocheazaConversia;
    int reducereCostProcent;

public:
    Tehnologie(const std::string& id, const std::string& nume, const std::string& ramura, int nivel, int cost);

    const std::string& getId() const;
    const std::string& getNume() const;
    int getCost() const;
    int getProgres() const;
    bool esteDeblocata() const;
    const std::vector<std::string>& getCerinte() const;

    const std::string& getUnitateDeblocata() const;
    int getBonusViata() const;
    int getBonusAtac() const;
    int getBonusRaza() const;
    int getBonusVindecare() const;
    bool permiteConversia() const;
    int getReducereCostProcent() const;

    void seteazaUnitateDeblocata(const std::string& unitate);
    void seteazaBonusuriLupta(int viata, int atac, int raza);
    void seteazaEfecteMagice(int vindecare, bool conversie, int reducereCost);

    void adaugaCerinta(const std::string& idCerinta);
    void adaugaProgres(int puncte);
    void deblocheaza();
};

#endif