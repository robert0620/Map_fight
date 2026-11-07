#ifndef MAG_H
#define MAG_H

#include <string>
#include <memory>
#include "Unitate.h"
#include "Punct2D.h"

class Mag : public Unitate {
private:
    int putereVindecareBaza;
    int bonusVindecareAplicat;
    bool conversieActiva;

public:
    Mag(const std::string& nume, int viata, int atac, int raza, int cost,
        const Punct2D<int>& startPoz = Punct2D<int>(0, 0),
        int id_jucator = 0, int putereVindecare = 25, bool conversieActiva = false);

    std::unique_ptr<Unitate> cloneaza() const override;

    int getPutereVindecare() const;
    void sincronizeazaBonusVindecare(int totalBonusVindecare);

    bool areConversieActiva() const;
    void setConversieActiva(bool activa);

    bool vindeca(Unitate& aliat);
    bool converteste(Unitate& inamic);

    std::string getSimbolVisual() const override;
    bool esteMag() const override;
    bool vindecaAliat(Unitate& aliat) override;
    bool convertesteInamic(Unitate& inamic) override;
};

#endif