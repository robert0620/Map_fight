#undef NDEBUG
#include <cassert>
#include <iostream>
#include "Unitate.h"

int main() {

    Unitate test1("Dummy_Test", 10, 1, 1);
    Unitate test2 = test1;

    assert(test1 == test2 && "Constructorul de copiere a esuat!");

    test2.primesteDaune(5);

    assert(test1 != test2 && "Memoria nu este separata corect (deep copy esuat)!");

    std::cout << "[TEST] Toate testele pentru clasa Unitate au trecut cu succes!\n";
    return 0;
}