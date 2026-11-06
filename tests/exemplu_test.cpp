#include <iostream>
#include <cassert>
#include "Unitate.h"
#include "Punct2D.h"

int main() {
    Unitate test1("Dummy_Test", 10, 1, 1, 50, Punct2D<int>(0, 0));
    Unitate test2 = test1;
    assert(test1.getNume() == test2.getNume() && "Constructorul de copiere a esuat la nume!");
    assert(test1.esteVie() == test2.esteVie() && "Constructorul de copiere a esuat la status!");

    test2.primesteDaune(15);

    assert(test1.esteVie() == true && "Memoria nu este separata corect! Originalul a murit odata cu copia.");
    assert(test2.esteVie() == false && "Copia ar fi trebuit sa moara dupa 15 daune.");

    std::cout << "Toate testele pentru clasa Unitate au trecut cu succes!\n";
    return 0;
}