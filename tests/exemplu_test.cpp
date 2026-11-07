#include <iostream>
#include "Punct2D.h"
#include "Unitate.h"
#include "Mag.h"
#include "ArboreTehnologic.h"
#include "Armata.h"
#include "Harta.h"
#include "Vreme.h"
#include "Joc.h"

#define TEST_ASSERT(expr) \
    if (!(expr)) { \
        std::cerr << "Test esuat la linia " << __LINE__ << " -> " << #expr << "\n"; \
        return 1; \
    }

int main() {
    Punct2D<int> p1(0, 0);

    Unitate u("Soldat", 100, 20, 1, 50, p1, 1);
    u.primesteDaune(30);
    TEST_ASSERT(u.getViata() == 70);

    ArboreTehnologic arbore;
    arbore.initializeazaArborePolytopia();

    bool cercetareInceputa = arbore.incepeCercetarea("TECH_CATARARE");
    TEST_ASSERT(cercetareInceputa);

    arbore.proceseazaTura(50);
    TEST_ASSERT(arbore.esteTehnologieDeblocata("TECH_CATARARE"));

    arbore.echipeazaUnitate(u);
    int razaDupaPrimaEchipare = u.getRaza();
    int viataMaxDupaPrimaEchipare = u.getViataMaxima();

    arbore.echipeazaUnitate(u);
    TEST_ASSERT(u.getRaza() == razaDupaPrimaEchipare);
    TEST_ASSERT(u.getViataMaxima() == viataMaxDupaPrimaEchipare);

    Harta harta(8, 8, TipBiom::TEMPERAT);
    Vreme vreme(TipVreme::SENIN, 1);
    Joc joc(harta, vreme);

    Armata a1("Imperiul 1", "assets/demons.json", 200, Punct2D<int>(0, 0), 1);
    Armata a2("Imperiul 2", "assets/dwarfs.json", 200, Punct2D<int>(0, 1), 2);
    a1.adaugaUnitate(std::make_unique<Unitate>("Luptator 1", 100, 25, 1, 40, Punct2D<int>(0, 0), 1));
    a2.adaugaUnitate(std::make_unique<Unitate>("Luptator 2", 100, 20, 1, 40, Punct2D<int>(0, 1), 2));

    joc.adaugaFactiune(a1);
    joc.adaugaFactiune(a2);

    bool primulAtac = joc.executaAtac(0, 0, 1, 0);
    TEST_ASSERT(primulAtac == true);

    bool alDoileaAtac = joc.executaAtac(0, 0, 1, 0);
    TEST_ASSERT(alDoileaAtac == false);

    Punct2D<int> pozMunte(3, 4);
    joc.getFactiune(0).adaugaUnitate(std::make_unique<Unitate>("Miner", 100, 10, 1, 30, pozMunte, 1));

    bool primaMina = joc.construiesteMina(0, pozMunte);
    TEST_ASSERT(primaMina == true);

    bool aDouaMina = joc.construiesteMina(0, pozMunte);
    TEST_ASSERT(aDouaMina == false);

    std::cout << "Toate testele de regresie si reguli de joc au trecut cu succes!\n";
    return 0;
}