# Map Fight

Un joc de strategie tactic *turn-based* (pe ture) rulat direct din consolă, scris în C++23. Proiectul simulează o confruntare între două facțiuni (Dwarfs și Demons) pe o hartă bidimensională, îmbinând concepte fundamentale de Programare Orientată pe Obiecte (OOP) cu elemente de joc 4X.

## Logica Jocului și Mecanici

Jocul se desfășoară pe o hartă generată dinamic, unde jucătorul controlează facțiunea Dwarfs, iar un bot controlează facțiunea Demons.

* **Sistemul de Hartă și Biomuri:**
  La începutul jocului se alege unul dintre cele 3 biomuri (0 = Dry Land, 1 = Continents, 2 = Water World). Harta conține elemente de teren cu impact tactic:
  * `[.]` Pământ (teren accesibil).
  * `[~]` Apă și `[^]` Munte (teren inaccesibil trupelor terestre).
  * `[$]` Resursă (oferă +10 Aur la colectare).
  * `[T]` Trib neutru (poate fi convertit în Oraș).
  * `[C]` Oraș (generează venit pasiv de +5 Aur pe rundă și oferă bonus de apărare).

* **Sistemul de Luptă și Economie:**
  * **Mișcarea:** Jucătorul își poate deplasa eroul cu tastele direcționale (W, A, S, D) sau introducând coordonate exacte (X Y).
  * **Atacul:** Se bazează pe distanța Manhattan. O unitate poate ataca doar dacă inamicul este în raza ei de acțiune. Daunele sunt influențate de armura oferită de relieful pe care stă ținta.
  * **Level Up:** O unitate care elimină un inamic este promovată, primind viață maximă și atac crescut.
  * **Aurul:** Este folosit pentru a menține economia și a vindeca unitățile avariate între lupte.

* **Arhitectura OOP:**
  * **Compunere:** Clasa `Joc` integrează `Harta` și instanțele de `Armata`. La rândul ei, `Harta` compune `Celula`, iar `Armata` compune `Unitate`.
  * **Gestionarea Memoriei:** Clasa `Harta` respectă strict *Regula celor 3* prin gestionarea unei matrici alocate dinamic pentru celulele grilei de joc.
  * **Înlănțuire:** Afișarea stării curente se face prin supraîncărcarea în cascadă a `operator<<` dinspre `Joc` până la `Celula` și `Unitate`.

## Cum se rulează

Proiectul este împărțit într-o bibliotecă statică (logica din `src/` și `include/`) și un executabil (`app/main.cpp`).

### 1. Din CLion (Mod Interactiv)
1. Deschide proiectul în CLion și lasă CMake să descarce automat bibliotecile externe (`fmt` și `nlohmann_json`).
2. Selectează target-ul `oop_main` din colțul din dreapta-sus.
3. Apasă **Run** (sau Shift+F10).
4. Interacționează cu jocul citind mesajele din consolă (introdu biomul, apoi alege W/A/S/D la fiecare rundă).

### 2. Din Terminal / GitHub Actions (Mod Automatizat)
Pentru testele automate de tip CI (Continuous Integration), jocul suportă citirea input-urilor dintr-un fișier prin pipe (redirecționare), astfel încât să nu aștepte input manual de la tastatură:
```bash
# Generarea și compilarea proiectului
cmake -B build
cmake --build build

# Rularea automată folosind fișierul de testare
./build/oop_main < tastatura.txt