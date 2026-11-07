#include <cassert>
#include <iostream>
#include "Punct2D.h"
#include "Unitate.h"
#include "Mag.h"
#include "ArboreTehnologic.h"
#include "Armata.h"
#include "Harta.h"
#include "Vreme.h"
#include "Joc.h"

int main() {
    Punct2D<int> p1(0, 0);
    Punct2D<int> p2(0, 1);

    Unitate u("Soldat", 100, 20, 1, 50, p1, 1);
    u.primesteDaune(30);
    assert(u.getViata() == 70);

    ArboreTehnologic arbore;
    arbore.initializeazaArborePolytopia();
    assert(arbore.incepeCercetarea("TECH_CATARARE"));
    arbore.proceseazaTura(50);
    assert(arbore.esteTehnologieDeblocata("TECH_CATARARE"));

    arbore.echipeazaUnitate(u);
    int razaDupaPrimaEchipare = u.getRaza();
    int viataMaxDupaPrimaEchipare = u.getViataMaxima();

    arbore.echipeazaUnitate(u);
    assert(u.getRaza() == razaDupaPrimaEchipare);
    assert(u.getViataMaxima() == viataMaxDupaPrimaEchipare);

    Harta harta(8, 8, TipBiom::TEMPERAT);
    Vreme vreme(TipVreme::SENIN, 1);
    Joc joc(harta, vreme);

    Armata a1("Imperiul 1", "assets/demons.json", 200, Punct2D<int>(0, 0), 1);
    Armata a2("Imperiul 2", "assets/dwarfs.json", 200, Punct2D<int>(0, 1), 2);
    a1.adaugaUnitate(std::make_unique<Unitate>("Luptator 1", 100, 25, 1, 40, Punct2D<int>(0, 0), 1));
    a2.adaugaUnitate(std::make_unique<Unitate>("Luptator 2", 100, 20, 1, 40, Punct2D<int>(0, 1), 2));

    joc.adaugaFactiune(a1);
    joc.adaugaFactiune(a2);

    assert(joc.executaAtac(0, 0, 1, 0) == true);
    assert(joc.executaAtac(0, 0, 1, 0) == false);

    Punct2D<int> pozMunte(3, 4);
    joc.getFactiune(0).adaugaUnitate(std::make_unique<Unitate>("Miner", 100, 10, 1, 30, pozMunte, 1));
    assert(joc.construiesteMina(0, pozMunte) == true);
    assert(joc.construiesteMina(0, pozMunte) == false);

    std::cout << "Toate testele de regresie si reguli de joc au trecut cu succes!\n";
    return 0;
}