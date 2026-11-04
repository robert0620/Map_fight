#ifndef UNITATE_H
#define UNITATE_H

#include <string>
#include <iostream>

class Unitate {
private:
    std::string nume;
    int viata{10};
    int viataMaxima{10};
    int atac{2};
    int raza{1};
    int x{0};
    int y{0};
    int nivel{1};

public:
    Unitate();
    explicit Unitate(const std::string& nume_, int viata_, int atac_, int raza_, int x_ = 0, int y_ = 0);

    const std::string& getNume() const;
    int getViata() const;
    int getViataMaxima() const;
    int getAtac() const;
    int getRaza() const;
    int getX() const;
    int getY() const;
    int getNivel() const;

    void seteazaPozitie(int nouX, int nouY);
    int calculeazaDistanta(const Unitate& alta) const;

    void primesteDaune(int damage);
    void vindeca(int puncte);
    void promoveaza();
    void ataca(Unitate& inamic, int bonusAparareInamic = 0) const;
    bool esteViu() const;

    bool operator==(const Unitate& other) const;
    bool operator!=(const Unitate& other) const;

    friend std::ostream& operator<<(std::ostream& os, const Unitate& u);
};

#endif