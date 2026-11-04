#include <Joc.h>
#include <Harta.h>
#include <Armata.h>
#include <Unitate.h>
#include <iostream>
#include <fmt/core.h>
#include <fmt/color.h>

int main() {
    std::cout << "Introdu optiunea de biom (0=DryLand, 1=Continents, 2=WaterWorld) si numarul de runde: ";
    int optiuneBiom = 1;
    int numarRunde = 3;
    if (!(std::cin >> optiuneBiom >> numarRunde)) {
        optiuneBiom = 1;
        numarRunde = 3;
    }

    TipBiom biomAles = TipBiom::CONTINENTS;
    if (optiuneBiom == 0) {
        biomAles = TipBiom::DRY_LAND;
    } else if (optiuneBiom == 2) {
        biomAles = TipBiom::WATER_WORLD;
    }

    Joc motorJoc(6, 6, biomAles);
    motorJoc.start();

    std::cout << motorJoc;

    for (int r = 0; r < numarRunde; ++r) {
        std::cout << "\nIntrodu coordonatele (X Y) pentru mutarea din runda " << (r + 1) << ": ";
        int mutareX = 1;
        int mutareY = 1;
        if (!(std::cin >> mutareX >> mutareY)) {
            mutareX = (r + 1) % motorJoc.getHarta().getLinii();
            mutareY = (r + 1) % motorJoc.getHarta().getColoane();
        }
        motorJoc.executaRundaSimulata(mutareX, mutareY);
        std::cout << motorJoc;
    }

    fmt::print(fmt::fg(fmt::color::cyan), "\n=== VERIFICARE COMPONENTE SI METODE PUBLICE ===\n");

    const Harta& refHarta = motorJoc.getHarta();
    if (refHarta.estePozitieValida(0, 0)) {
        const Celula& celulaStart = refHarta.getCelula(0, 0);
        std::cout << "Biom: " << static_cast<int>(refHarta.getBiom())
                  << " | Celula (0,0) - Accesibila: " << celulaStart.esteAccesibila()
                  << " | Bonus Aparare: " << celulaStart.getBonusAparare()
                  << " | Proprietar: '" << celulaStart.getProprietar() << "'"
                  << " | Orase Dwarfs pe harta: " << refHarta.numaraOraseJucator("Dwarfs") << "\n";
    }

    Celula celulaCustom(TipTeren::ORAS, "Dwarfs", 3);
    celulaCustom.setBonusAparare(4);
    std::cout << "Celula custom: " << celulaCustom << " (Bonus: " << celulaCustom.getBonusAparare() << ")\n";

    Unitate gardianImplicit;
    Unitate gardian1("Gardian", 20, 5, 2, 0, 0);
    Unitate gardian2("Gardian", 20, 5, 2, 0, 1);

    if (gardian1 != gardian2 && gardianImplicit != gardian1) {
        std::cout << "Unitate: " << gardian1.getNume()
                  << " | Lvl: " << gardian1.getNivel()
                  << " | HP: " << gardian1.getViata() << "/" << gardian1.getViataMaxima()
                  << " | ATK: " << gardian1.getAtac()
                  << " | RNG: " << gardian1.getRaza()
                  << " | Dist: " << gardian1.calculeazaDistanta(gardian2) << "\n";
    }

    gardian2.seteazaPozitie(0, 0);
    if (gardian1 == gardian2) {
        std::cout << "Cele doua unitati sunt acum identice si pe aceeasi pozitie.\n";
    }

    Armata garnizoana("Garnizoana", 50);
    garnizoana.recruteaza(gardian1);
    garnizoana.getTrupa(0).primesteDaune(5);
    garnizoana.vindecaTrupaCuAur(0, 10, 5);

    const Armata& refGarnizoana = garnizoana;
    if (refGarnizoana.maiAreTrupeVii() && refGarnizoana.getNumarTrupe() > 0) {
        const Unitate& trupaConst = refGarnizoana.getTrupa(0);
        std::cout << "Garnizoana lui " << refGarnizoana.getNumeJucator()
                  << " are " << refGarnizoana.getAur() << " aur, "
                  << refGarnizoana.getNumarOrase() << " orase si unitatea: "
                  << trupaConst.getNume() << "\n";
    }

    return 0;
}