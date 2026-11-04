#include "Harta.h"
#include <fmt/core.h>
#include <fmt/color.h>

Celula::Celula()
    : teren{TipTeren::PAMANT}, proprietar{""}, bonusAparare{0} {}

Celula::Celula(TipTeren teren_, const std::string& proprietar_, int bonusAparare_)
    : teren{teren_}, proprietar{proprietar_}, bonusAparare{bonusAparare_} {}

TipTeren Celula::getTeren() const {
    return teren;
}

void Celula::setTeren(TipTeren terenNou) {
    teren = terenNou;
    if (terenNou == TipTeren::ORAS) {
        bonusAparare = 2;
    } else if (terenNou == TipTeren::TRIB) {
        bonusAparare = 1;
    } else {
        bonusAparare = 0;
    }
}

const std::string& Celula::getProprietar() const {
    return proprietar;
}

void Celula::setProprietar(const std::string& proprietarNou) {
    proprietar = proprietarNou;
}

int Celula::getBonusAparare() const {
    return bonusAparare;
}

void Celula::setBonusAparare(int bonusNou) {
    bonusAparare = bonusNou;
}

bool Celula::esteAccesibila() const {
    return teren != TipTeren::APA && teren != TipTeren::MUNTE;
}

std::ostream& operator<<(std::ostream& os, const Celula& c) {
    switch (c.teren) {
        case TipTeren::PAMANT:
            os << "[.]";
            break;
        case TipTeren::APA:
            os << "[~]";
            break;
        case TipTeren::MUNTE:
            os << "[^]";
            break;
        case TipTeren::RESURSA:
            os << "[$]";
            break;
        case TipTeren::TRIB:
            os << "[T]";
            break;
        case TipTeren::ORAS:
            os << "[C]";
            break;
    }
    return os;
}

void Harta::alocaMemorie() {
    grid = new Celula*[static_cast<size_t>(linii)];
    for (int i = 0; i < linii; ++i) {
        grid[i] = new Celula[static_cast<size_t>(coloane)];
    }
}

void Harta::elibereazaMemorie() {
    if (grid != nullptr) {
        for (int i = 0; i < linii; ++i) {
            delete[] grid[i];
        }
        delete[] grid;
        grid = nullptr;
    }
}

void Harta::populeazaBiom() {
    for (int i = 0; i < linii; ++i) {
        for (int j = 0; j < coloane; ++j) {
            if (biom == TipBiom::DRY_LAND) {
                if (i == linii / 2 && j == coloane / 2) {
                    grid[i][j].setTeren(TipTeren::APA);
                } else if ((i + j) % 7 == 0 && i > 0 && j > 0) {
                    grid[i][j].setTeren(TipTeren::MUNTE);
                } else {
                    grid[i][j].setTeren(TipTeren::PAMANT);
                }
            } else if (biom == TipBiom::WATER_WORLD) {
                if (i == 0 || i == linii - 1 || i == j || (i + j) % 3 == 0) {
                    grid[i][j].setTeren(TipTeren::PAMANT);
                } else {
                    grid[i][j].setTeren(TipTeren::APA);
                }
            } else {
                if (j == coloane / 2 && i != linii / 2) {
                    grid[i][j].setTeren(TipTeren::APA);
                } else {
                    grid[i][j].setTeren(TipTeren::PAMANT);
                }
            }

            if (grid[i][j].getTeren() == TipTeren::PAMANT &&
                !(i == 0 && j == 0) &&
                !(i == linii - 1 && j == coloane - 1)) {
                if ((i * 3 + j) % 8 == 0) {
                    grid[i][j].setTeren(TipTeren::RESURSA);
                } else if ((i + j * 2) % 11 == 0) {
                    grid[i][j].setTeren(TipTeren::TRIB);
                }
            }
        }
    }
}

Harta::Harta(int linii_, int coloane_, TipBiom biom_)
    : linii{linii_ > 2 ? linii_ : 5},
      coloane{coloane_ > 2 ? coloane_ : 5},
      biom{biom_},
      grid{nullptr} {
    alocaMemorie();
    populeazaBiom();
}

Harta::Harta(const Harta& other)
    : linii{other.linii}, coloane{other.coloane}, biom{other.biom}, grid{nullptr} {
    alocaMemorie();
    for (int i = 0; i < linii; ++i) {
        for (int j = 0; j < coloane; ++j) {
            grid[i][j] = other.grid[i][j];
        }
    }
}

Harta& Harta::operator=(const Harta& other) {
    if (this != &other) {
        elibereazaMemorie();
        linii = other.linii;
        coloane = other.coloane;
        biom = other.biom;
        alocaMemorie();
        for (int i = 0; i < linii; ++i) {
            for (int j = 0; j < coloane; ++j) {
                grid[i][j] = other.grid[i][j];
            }
        }
    }
    return *this;
}

Harta::~Harta() {
    elibereazaMemorie();
}

int Harta::getLinii() const {
    return linii;
}

int Harta::getColoane() const {
    return coloane;
}

TipBiom Harta::getBiom() const {
    return biom;
}

bool Harta::estePozitieValida(int x, int y) const {
    return x >= 0 && x < linii && y >= 0 && y < coloane;
}

bool Harta::estePozitieAccesibila(int x, int y) const {
    if (!estePozitieValida(x, y)) {
        return false;
    }
    return grid[x][y].esteAccesibila();
}

const Celula& Harta::getCelula(int x, int y) const {
    return grid[x][y];
}

int Harta::getBonusAparareLa(int x, int y) const {
    if (!estePozitieValida(x, y)) {
        return 0;
    }
    return grid[x][y].getBonusAparare();
}

int Harta::numaraOraseJucator(const std::string& numeJucator) const {
    int total = 0;
    for (int i = 0; i < linii; ++i) {
        for (int j = 0; j < coloane; ++j) {
            if (grid[i][j].getTeren() == TipTeren::ORAS && grid[i][j].getProprietar() == numeJucator) {
                total++;
            }
        }
    }
    return total;
}

int Harta::colecteazaResursa(int x, int y) {
    if (estePozitieValida(x, y) && grid[x][y].getTeren() == TipTeren::RESURSA) {
        grid[x][y].setTeren(TipTeren::PAMANT);
        fmt::print(fmt::fg(fmt::color::gold), "[MAP] Resursa colectata la ({}, {})! (+10 Gold)\n", x, y);
        return 10;
    }
    return 0;
}

bool Harta::convertesteTribInOras(int x, int y, const std::string& numeJucator) {
    if (estePozitieValida(x, y) && grid[x][y].getTeren() == TipTeren::TRIB) {
        grid[x][y].setTeren(TipTeren::ORAS);
        grid[x][y].setProprietar(numeJucator);
        fmt::print(fmt::fg(fmt::color::cyan), "[MAP] Tribul de la ({}, {}) a fost convertit in Oras pentru {}!\n", x, y, numeJucator);
        return true;
    }
    return false;
}

void Harta::plaseazaElement(int x, int y, TipTeren terenNou) {
    if (estePozitieValida(x, y)) {
        grid[x][y].setTeren(terenNou);
    }
}

std::ostream& operator<<(std::ostream& os, const Harta& h) {
    std::string numeBiom = "Dry Land";
    if (h.biom == TipBiom::CONTINENTS) {
        numeBiom = "Continents";
    } else if (h.biom == TipBiom::WATER_WORLD) {
        numeBiom = "Water World";
    }

    os << "=== Harta (" << h.linii << "x" << h.coloane << " | Biom: " << numeBiom << ") ===\n";
    os << "Legenda: [.] Pamant  [~] Apa  [^] Munte  [$] Resursa  [T] Trib  [C] Oras\n   ";
    for (int j = 0; j < h.coloane; ++j) {
        os << " " << j << " ";
    }
    os << "\n";

    for (int i = 0; i < h.linii; ++i) {
        os << " " << i << " ";
        for (int j = 0; j < h.coloane; ++j) {
            const Celula& celulaCurenta = h.grid[i][j];
            os << celulaCurenta;
        }
        os << "\n";
    }
    return os;
}