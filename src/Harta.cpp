#include "Harta.h"
#include <queue>
#include <cmath>

Harta::Harta(int latime, int inaltime, TipBiom biomAles)
    : latime(latime),
      inaltime(inaltime),
      biom(biomAles),
      grila(inaltime, std::vector<TipTeren>(latime, TipTeren::CAMPIE)),
      mineConstruite(inaltime, std::vector<bool>(latime, false)) {
    genereazaTerenDupaBiom();
}

void Harta::genereazaTerenDupaBiom() {
    for (int y = 0; y < inaltime; ++y) {
        for (int x = 0; x < latime; ++x) {
            if ((x == 0 && y < 3) || (x == latime - 1 && y >= inaltime - 3)) {
                grila[y][x] = TipTeren::CAMPIE;
                continue;
            }

            if (biom == TipBiom::VULCANIC) {
                if ((x + y) % 3 == 0) {
                    grila[y][x] = TipTeren::MUNTE;
                } else if ((x * y) % 7 == 0) {
                    grila[y][x] = TipTeren::PADURE;
                }
            } else if (biom == TipBiom::ARCTIC) {
                if ((x + y) % 4 == 0) {
                    grila[y][x] = TipTeren::PADURE;
                } else if ((x + y) % 6 == 0) {
                    grila[y][x] = TipTeren::MUNTE;
                }
            } else if (biom == TipBiom::ARHIPELAG) {
                if ((x + y) % 4 == 0 && x > 1 && x < latime - 2) {
                    grila[y][x] = TipTeren::APA;
                } else if ((x * y) % 5 == 0) {
                    grila[y][x] = TipTeren::PADURE;
                } else if ((x + y) % 7 == 0) {
                    grila[y][x] = TipTeren::MUNTE;
                }
            } else {
                if ((x + y) % 7 == 0) {
                    grila[y][x] = TipTeren::MUNTE;
                } else if ((x * y) % 5 == 0 && x > 1) {
                    grila[y][x] = TipTeren::PADURE;
                } else if (x == latime / 2 && y == inaltime / 2) {
                    grila[y][x] = TipTeren::APA;
                }
            }
        }
    }
}

bool Harta::esteInInterior(const Punct2D<int>& p) const {
    return p.getX() >= 0 && p.getX() < latime && p.getY() >= 0 && p.getY() < inaltime;
}

char Harta::getSimbolTeren(TipTeren t) {
    switch (t) {
        case TipTeren::CAMPIE: return '.';
        case TipTeren::PADURE: return 'P';
        case TipTeren::MUNTE: return 'M';
        case TipTeren::APA: return '~';
        default: return '?';
    }
}

std::string Harta::getNumeBiom() const {
    switch (biom) {
        case TipBiom::TEMPERAT: return "Temperat";
        case TipBiom::VULCANIC: return "Vulcanic";
        case TipBiom::ARCTIC: return "Arctic";
        case TipBiom::ARHIPELAG: return "Arhipelag";
        default: return "Necunoscut";
    }
}

TipTeren Harta::getTeren(const Punct2D<int>& p) const {
    if (!esteInInterior(p)) {
        return TipTeren::APA;
    }
    return grila[p.getY()][p.getX()];
}

int Harta::getBonusAparare(const Punct2D<int>& p) const {
    TipTeren t = getTeren(p);
    if (t == TipTeren::PADURE) {
        return 5;
    }
    if (t == TipTeren::MUNTE) {
        return 10;
    }
    return 0;
}

int Harta::getCostMiscare(const Punct2D<int>& p) const {
    TipTeren t = getTeren(p);
    if (t == TipTeren::PADURE) {
        return 2;
    }
    if (t == TipTeren::MUNTE) {
        return 3;
    }
    return 1;
}

bool Harta::esteAccesibil(const Punct2D<int>& p) const {
    return esteInInterior(p) && getTeren(p) != TipTeren::APA;
}

bool Harta::existaTraseuAccesibil(const Punct2D<int>& start, const Punct2D<int>& destinatie, int pasiMaximi) const {
    if (!esteAccesibil(start) || !esteAccesibil(destinatie) || pasiMaximi <= 0) {
        return false;
    }
    if (start == destinatie) {
        return true;
    }

    std::vector<std::vector<int>> dist(inaltime, std::vector<int>(latime, -1));
    std::queue<Punct2D<int>> coada;

    coada.push(start);
    dist[start.getY()][start.getX()] = 0;

    const int dx[4] = {1, -1, 0, 0};
    const int dy[4] = {0, 0, 1, -1};

    while (!coada.empty()) {
        Punct2D<int> curent = coada.front();
        coada.pop();

        int dCurent = dist[curent.getY()][curent.getX()];
        if (curent == destinatie) {
            return dCurent <= pasiMaximi;
        }
        if (dCurent >= pasiMaximi) {
            continue;
        }

        for (int dir = 0; dir < 4; ++dir) {
            Punct2D<int> vecin(curent.getX() + dx[dir], curent.getY() + dy[dir]);
            if (esteAccesibil(vecin) && dist[vecin.getY()][vecin.getX()] == -1) {
                dist[vecin.getY()][vecin.getX()] = dCurent + 1;
                coada.push(vecin);
            }
        }
    }

    return false;
}

bool Harta::areMina(const Punct2D<int>& p) const {
    if (!esteInInterior(p)) {
        return false;
    }
    return mineConstruite[p.getY()][p.getX()];
}

bool Harta::plaseazaMina(const Punct2D<int>& p) {
    if (!esteInInterior(p) || getTeren(p) != TipTeren::MUNTE || mineConstruite[p.getY()][p.getX()]) {
        return false;
    }
    mineConstruite[p.getY()][p.getX()] = true;
    return true;
}

int Harta::getLatime() const {
    return latime;
}

int Harta::getInaltime() const {
    return inaltime;
}

std::ostream& operator<<(std::ostream& os, const Harta& h) {
    os << "Harta [" << h.getNumeBiom() << "] (" << h.latime << "x" << h.inaltime << "):\n";
    for (int y = 0; y < h.inaltime; ++y) {
        for (int x = 0; x < h.latime; ++x) {
            if (h.mineConstruite[y][x]) {
                os << "$ ";
            } else {
                os << Harta::getSimbolTeren(h.grila[y][x]) << ' ';
            }
        }
        os << '\n';
    }
    return os;
}