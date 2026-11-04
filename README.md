# Map Fight 4X

Un joc de strategie turn-based (tip 4X) dezvoltat in C++, cu un puternic accent pe arhitectura orientata pe obiecte (OOP). Proiectul combina gestionarea resurselor, pozitionarea tactica pe o harta generata procedural si sisteme dinamice de lupta si vreme.

## Logica si Mecanicile Jocului

Jocul se desfasoara in runde, unde doi jucatori se infrunta pe o harta tip grila, controland factiuni complet diferite.

* Teren si Biomuri: Harta este generata in functie de biomul ales (Temperat, Vulcanic, Arctic, Arhipelag). Fiecare celula influenteaza lupta: Muntele ofera +10 Aparare dar costa 3 Puncte de Miscare (PM), Padurea ofera +5 Aparare, iar Apa este un obstacol natural.
* Economie si Extindere: Jucatorii pornesc cu un buget de aur si un venit pe tura. Aurul este folosit pentru recrutarea de trupe noi sau pentru construirea de Mine pe munti. Minele aflate aproape de baza genereaza mai mult aur.
* Sistem de Lupta & Experienta (XP): Atacurile sunt calculate folosind Distanta Manhattan. Orice unitate care ataca primeste puncte XP. La anumite praguri, unitatile sunt promovate (Recrut -> Veteran -> Elita), vindecandu-se si primind bonusuri permanente la daune.
* Vreme Dinamica: La fiecare cateva ture, vremea se schimba. Ploaia incetineste miscarile, Ceata scade eficienta atacurilor la distanta (60%), iar Viscolul dubleaza costurile de deplasare.
* Date Externe (JSON): Trupele factiunilor nu sunt hardcodate in cod. Statisticile lor (Viata, Atac, Raza, Cost) sunt incarcate dinamic din fisiere .json aflate in folderul "assets/".

## Exemplu de Rulare in Terminal

Interfata grafica ruleaza direct in consola (CLI), redand starea vremii, statisticile armatelor si o harta tactica unde caracterele '1' si '2' reprezinta trupele celor doi jucatori.

```text
=== STARE JOC (Tura 4) ===
[Vreme: Ploaie (Intensitate 1) | Coef. Atac Distanta: 0.85 | Penalizare Miscare: +1]

Armata Legiunea Demonica (Baza: (0, 0) | Aur: 170 | Orase: 1 | Scor: 530)
  - Demon Mic [Recrut, XP:30, HP:90, ATK:25, Raza:1, Poz:(0, 0)]
  - Succubus [Veteran, XP:60, HP:70, ATK:40, Raza:2, Poz:(1, 0)]

Armata Clanul Piticilor (Baza: (7, 7) | Aur: 110 | Orase: 1 | Scor: 490)
  - Tun Pitic [Recrut, XP:0, HP:80, ATK:50, Raza:4, Poz:(7, 6)]

--- HARTA TACTICA (1 = Jucator 1, 2 = Jucator 2, . = Campie, P = Padure, M = Munte, ~ = Apa) ---
  0 1 2 3 4 5 6 7 
0 1 1 P P . . . . 
1 . . . M . . . . 
2 . . . . P . . . 
3 . . . . . M . . 
4 P P . . ~ . . . 
5 . M . . ~ . . . 
6 . . . . . P P 2 
7 . . M . . . . 2 

-----------------------------------------
TURA JUCATORULUI 1 - MENIU COMENZI:
1. Afiseaza starea completa si harta tactica
2. Plimba / Deplaseaza o unitate
3. Ataca o unitate inamica
4. Recruteaza o unitate noua in baza
5. Construieste o mina pe munte (40 aur)
6. Afiseaza clasamentul triburilor
7. Incheie tura
0. Iesire din joc
Alege comanda:ows