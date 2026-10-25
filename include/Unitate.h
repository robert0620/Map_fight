#pragma once
#include <string>
#include <iostream>

class Unitate {
    std::string nume;
    int viata{0};
    int atac{0};
    int raza{0};

public:
    explicit Unitate(const std::string& nume_ = "Necunoscut", int viata_ = 10, int atac_ = 2, int raza_ = 1);

    Unitate(const Unitate& other);
    Unitate& operator=(const Unitate& other);
    Unitate(Unitate&&) = default;
    Unitate& operator=(Unitate&&) = default;
    ~Unitate();

    const std::string& getNume() const;
    int getViata() const;
    int getAtac() const;
    int getRaza() const;

    void primesteDaune(int damage);
    void ataca(Unitate& inamic) const;
    bool esteViu() const;

    bool operator==(const Unitate& other) const;
    bool operator!=(const Unitate& other) const;

    friend std::ostream& operator<<(std::ostream& os, const Unitate& u);
};