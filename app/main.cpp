#include <iostream>
#include <string>
#include "Punct2D.h"
#include "Vreme.h"
#include "Unitate.h"
#include "Mag.h"
#include "ArboreTehnologic.h"
#include "Armata.h"
#include "Harta.h"
#include "Joc.h"

static void selecteazaFactiune(int optiune, std::string& numeOut, std::string& fisierOut) {
    switch (optiune) {
        case 1:
            numeOut = "Legiunea Demonica";
            fisierOut = "assets/demons.json";
            break;
        case 2:
            numeOut = "Clanul Piticilor";
            fisierOut = "assets/dwarfs.json";
            break;
        case 3:
            numeOut = "Alianta Elfica";
            fisierOut = "assets/elves.json";
            break;
        default:
            numeOut = "Hoarda Orcilor";
            fisierOut = "assets/orcs.json";
            break;
    }
}

static TipBiom selecteazaBiom(int optiune) {
    switch (optiune) {
        case 1: return TipBiom::TEMPERAT;
        case 2: return TipBiom::VULCANIC;
        case 3: return TipBiom::ARCTIC;
        case 4: return TipBiom::ARHIPELAG;
        default: return TipBiom::TEMPERAT;
    }
}

int main() {
    std::cout << "=========================================\n";
    std::cout << "       BINE AI VENIT IN MAP FIGHT 4X     \n";
    std::cout << "=========================================\n";

    std::cout << "Alege tipul de biom:\n";
    std::cout << "1. Temperat\n2. Vulcanic\n3. Arctic\n4. Arhipelag\nOptiune biom: ";
    int optBiom = 1;
    if (!(std::cin >> optBiom)) {
        optBiom = 1;
    }

    std::cout << "Introdu dimensiunea hartii (intre 5 si 20): ";
    int dimensiune = 8;
    if (!(std::cin >> dimensiune) || dimensiune < 5 || dimensiune > 20) {
        dimensiune = 8;
    }

    std::cout << "\nFactiuni disponibile:\n";
    std::cout << "1. Legiunea Demonica\n2. Clanul Piticilor\n3. Alianta Elfica\n4. Hoarda Orcilor\n";
    std::cout << "Alege factiunea pentru Jucatorul 1 (1-4): ";
    int optJ1 = 1;
    if (!(std::cin >> optJ1)) {
        optJ1 = 1;
    }

    std::cout << "Alege factiunea pentru Jucatorul 2 (1-4): ";
    int optJ2 = 2;
    if (!(std::cin >> optJ2)) {
        optJ2 = 2;
    }

    std::string nume1, fisier1, nume2, fisier2;
    selecteazaFactiune(optJ1, nume1, fisier1);
    selecteazaFactiune(optJ2, nume2, fisier2);

    Harta hartaJoc(dimensiune, dimensiune, selecteazaBiom(optBiom));
    Vreme vremeInitiala(TipVreme::SENIN, 1);

    Punct2D<int> bazaJ1(0, 0);
    Punct2D<int> bazaJ2(hartaJoc.getLatime() - 1, hartaJoc.getInaltime() - 1);

    Armata armata1(nume1, fisier1, 150, bazaJ1, 1);
    Armata armata2(nume2, fisier2, 150, bazaJ2, 2);

    if (!armata1.incarcaTrupeInitiale()) {
        std::cout << "Eroare la incarcarea fisierului " << fisier1 << "\n";
        return 1;
    }
    if (!armata2.incarcaTrupeInitiale()) {
        std::cout << "Eroare la incarcarea fisierului " << fisier2 << "\n";
        return 1;
    }

    Joc sesiune(hartaJoc, vremeInitiala);
    sesiune.adaugaFactiune(armata1);
    sesiune.adaugaFactiune(armata2);

    sesiune.afiseazaHartaTactica();

    std::size_t jucatorActiv = 0;
    bool jocActiv = true;

    while (jocActiv) {
        if (sesiune.esteJocTerminat()) {
            int castigator = sesiune.getCastigator();
            if (castigator >= 0) {
                std::cout << "\n=========================================\n";
                std::cout << "VICTORIE! Jucatorul " << (castigator + 1) << " ("
                          << sesiune.getFactiune(static_cast<std::size_t>(castigator)).getNume()
                          << ") a cucerit harta!\n";
                std::cout << "=========================================\n";
            }
            break;
        }

        std::cout << "\n-----------------------------------------\n";
        std::cout << "TURA JUCATORULUI " << (jucatorActiv + 1) << " ("
                  << sesiune.getFactiune(jucatorActiv).getNume() << ") - MENIU COMENZI:\n";
        std::cout << "1. Afiseaza starea completa si harta tactica\n";
        std::cout << "2. Deplaseaza o unitate (1 data / tura)\n";
        std::cout << "3. Ataca o unitate inamica (1 data / tura)\n";
        std::cout << "4. Recruteaza o unitate sau un Mag in baza\n";
        std::cout << "5. Construieste o mina pe munte controlat (40 aur)\n";
        std::cout << "6. Afiseaza clasamentul triburilor\n";
        std::cout << "7. Incheie tura\n";
        std::cout << "8. Arbore Tehnologic (Cercetare)\n";
        std::cout << "9. Abilitati Mag din armata (Vindecare / Conversie)\n";
        std::cout << "0. Iesire din joc\n";
        std::cout << "Alege comanda: ";

        int comanda = 0;
        if (!(std::cin >> comanda)) {
            break;
        }

        switch (comanda) {
            case 1: {
                std::cout << sesiune;
                sesiune.afiseazaHartaTactica();
                sesiune.getFactiune(jucatorActiv).getArboreTehnologic().afiseazaStatusCercetare();
                break;
            }
            case 2: {
                std::size_t idxUnitate = 0;
                int x = 0;
                int y = 0;
                std::cout << "Indexul unitatii tale (incepand de la 0): ";
                std::cin >> idxUnitate;
                std::cout << "Coordonata destinatie X si Y (ex: 1 2): ";
                std::cin >> x >> y;
                if (!sesiune.deplaseazaUnitate(jucatorActiv, idxUnitate, Punct2D<int>(x, y))) {
                    std::cout << ">> Deplasare invalida (deja mutata in aceasta tura, traseu blocat de apa sau prea departe)!\n";
                }
                sesiune.afiseazaHartaTactica();
                break;
            }
            case 3: {
                std::size_t idxUnitProprie = 0;
                std::size_t idxUnitInamica = 0;
                std::size_t inamic = (jucatorActiv == 0) ? 1 : 0;
                std::cout << "Indexul unitatii tale care ataca: ";
                std::cin >> idxUnitProprie;
                std::cout << "Indexul unitatii inamice tinta: ";
                std::cin >> idxUnitInamica;
                if (!sesiune.executaAtac(jucatorActiv, idxUnitProprie, inamic, idxUnitInamica)) {
                    std::cout << ">> Atacul a esuat (unitatea a actionat deja in aceasta tura sau tinta nu este in raza)!\n";
                }
                break;
            }
            case 4: {
                std::size_t tipSablon = 0;
                std::cout << "Alege tipul de unitate (0, 1, 2 pentru sabloane sau 3 pentru Mag - 60 aur, necesita Meditatie/Filozofie): ";
                std::cin >> tipSablon;
                if (tipSablon == 3) {
                    if (!sesiune.recruteazaMagInFactiune(jucatorActiv)) {
                        std::cout << ">> Recrutare Mag esuata (cerceteaza TECH_MEDITATIE si asigura-te ca ai 60 aur)!\n";
                    }
                } else {
                    if (!sesiune.recruteazaInFactiune(jucatorActiv, tipSablon)) {
                        std::cout << ">> Recrutare esuata (aur insuficient sau index invalid)!\n";
                    }
                }
                break;
            }
            case 5: {
                int mx = 0;
                int my = 0;
                std::cout << "Coordonatele muntelui X si Y (ex: 3 4): ";
                std::cin >> mx >> my;
                if (sesiune.construiesteMina(jucatorActiv, Punct2D<int>(mx, my))) {
                    std::cout << ">> Mina a fost construita cu succes! Productia de aur a crescut.\n";
                } else {
                    std::cout << ">> Construire esuata (nu este munte, are deja mina, nu ai o unitate langa munte sau nu ai 40 aur)!\n";
                }
                break;
            }
            case 6: {
                sesiune.afiseazaClasament();
                break;
            }
            case 7: {
                jucatorActiv = (jucatorActiv == 0) ? 1 : 0;
                if (jucatorActiv == 0) {
                    sesiune.avanseazaTura();
                    std::cout << ">> O noua runda a inceput! Vremea, cercetarea, actiunile si veniturile au fost actualizate.\n";
                } else {
                    std::cout << ">> Tura a trecut la Jucatorul 2.\n";
                }
                break;
            }
            case 8: {
                ArboreTehnologic& arbore = sesiune.getFactiune(jucatorActiv).getArboreTehnologic();
                arbore.afiseazaStatusCercetare();
                arbore.afiseazaTehnologiiDisponibile();
                std::cout << "Introdu ID-ul tehnologiei de cercetat (ex: TECH_CATARARE sau 0 pentru anulare): ";
                std::string idTech;
                std::cin >> idTech;
                if (idTech != "0") {
                    if (sesiune.incepeCercetareInFactiune(jucatorActiv, idTech)) {
                        std::cout << ">> Cercetarea pentru " << idTech << " a inceput si va avansa la fiecare runda!\n";
                    } else {
                        std::cout << ">> Nu poti cerceta aceasta tehnologie acum.\n";
                    }
                }
                break;
            }
            case 9: {
                std::size_t inamicIdx = (jucatorActiv == 0) ? 1 : 0;
                std::size_t idxMag = 0;
                std::cout << "Indexul Magului din armata ta: ";
                std::cin >> idxMag;
                std::cout << "1. Vindeca o unitate aliata ranita\n2. Converteste o unitate inamica adiacenta (necesita TECH_FILOZOFIE)\nOptiune: ";
                int actiuneMag = 0;
                std::cin >> actiuneMag;

                if (actiuneMag == 1) {
                    std::size_t idxAliat = 0;
                    std::cout << "Indexul unitatii aliate pe care vrei sa o vindeci: ";
                    std::cin >> idxAliat;
                    if (sesiune.executaVindecareMag(jucatorActiv, idxMag, idxAliat)) {
                        std::cout << ">> Unitatea aliata a fost vindecata!\n";
                    } else {
                        std::cout << ">> Vindecare esuata (unitatea nu este Mag, a actionat deja, aliatul are HP maxim sau este prea departe)!\n";
                    }
                } else if (actiuneMag == 2) {
                    std::size_t idxInamic = 0;
                    std::cout << "Indexul unitatii inamice adiacente: ";
                    std::cin >> idxInamic;
                    if (sesiune.executaConversieMag(jucatorActiv, idxMag, inamicIdx, idxInamic)) {
                        std::cout << ">> Unitatea inamica a fost convertita si mutata in armata ta!\n";
                    } else {
                        std::cout << ">> Conversie esuata (necesita un Mag activ, TECH_FILOZOFIE si distanta <= 1)!\n";
                    }
                }
                break;
            }
            case 0: {
                std::cout << "Jocul s-a incheiat.\n";
                jocActiv = false;
                break;
            }
            default: {
                std::cout << "Comanda necunoscuta!\n";
                break;
            }
        }
    }

    return 0;
}