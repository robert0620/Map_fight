#ifndef HARTA_H
#define HARTA_H

#include <iostream>
#include <string>

enum class TipTeren {
    PAMANT,
    APA,
    MUNTE,
    RESURSA,
    TRIB,
    ORAS
};

enum class TipBiom {
    DRY_LAND,
    CONTINENTS,
    WATER_WORLD
};

class Celula {
private:
    TipTeren teren{TipTeren::PAMANT};
    std::string proprietar{};
    int bonusAparare{0};

public:
    Celula();
    explicit Celula(TipTeren teren_, const std::string& proprietar_, int bonusAparare_ = 0);

    TipTeren getTeren() const;
    void setTeren(TipTeren terenNou);

    const std::string& getProprietar() const;
    void setProprietar(const std::string& proprietarNou);

    int getBonusAparare() const;
    void setBonusAparare(int bonusNou);

    bool esteAccesibila() const;

    friend std::ostream& operator<<(std::ostream& os, const Celula& c);
};

class Harta {
private:
    int linii{6};
    int coloane{6};
    TipBiom biom{TipBiom::CONTINENTS};
    Celula** grid{nullptr};

    void alocaMemorie();
    void elibereazaMemorie();
    void populeazaBiom();

public:
    Harta(int linii_, int coloane_, TipBiom biom_);

    Harta(const Harta& other);
    Harta& operator=(const Harta& other);
    ~Harta();

    int getLinii() const;
    int getColoane() const;
    TipBiom getBiom() const;
    bool estePozitieValida(int x, int y) const;
    bool estePozitieAccesibila(int x, int y) const;
    const Celula& getCelula(int x, int y) const;
    int getBonusAparareLa(int x, int y) const;
    int numaraOraseJucator(const std::string& numeJucator) const;

    int colecteazaResursa(int x, int y);
    bool convertesteTribInOras(int x, int y, const std::string& numeJucator);
    void plaseazaElement(int x, int y, TipTeren terenNou);

    friend std::ostream& operator<<(std::ostream& os, const Harta& h);
};

#endif