#pragma once
#include "Disc.h"

class DiscVintage: public Disc{
protected:
    int coeficientRaritate; // coeficientul de raritate al discului
    bool mint;

public: 
    DiscVintage(const string tip,const string den, const int st, const double pr, const string cd, const string dl, const string tr, const string alb, const string tipDisc, bool mnt, int coef_rar):
        Disc(tip, den, st, pr, cd, dl, tr, alb, tipDisc), mint(mnt), coeficientRaritate(coef_rar){};

    double pret_final() const override{
        return pret_baza + 15 * coeficientRaritate; //cost împachetare și livrare
    };
    
    void afisare_produs(ostream& dev) const override{
        Disc::afisare_produs(dev);
        dev<<"Coeficient de raritate: "<<coeficientRaritate<<endl;
        dev<<"Mint: "<<(mint ? "Da" : "Nu") <<endl;
    }
};