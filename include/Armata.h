#pragma once
#include <vector>
#include <string>
#include <cstddef>
#include "Unitate.h"

class Armata {
    std::string numeJucator;
    std::vector<Unitate> trupe;

public:
    explicit Armata(const std::string& numeJucator_ = "Jucator");

    void recruteaza(const Unitate& trupaNoua);
    void ordonaAtac(size_t indexTrupa, Unitate& inamic) const;
    Unitate& getTrupa(size_t index);

    friend std::ostream& operator<<(std::ostream& os, const Armata& a);
};