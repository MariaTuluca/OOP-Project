#pragma once
#include "Produs.h"

class ArticolVestimentar: public Produs{
protected: 
    string culoare;
    string marca;

public:
    ArticolVestimentar(const string tip, const string den, const int st, const double pr, const string cul, const string marc):
        Produs(tip, den, st, pr), culoare(cul), marca(marc){};

    double pret_final() const override{
        return pret_baza + 20; //cost împachetare și livrare
    };

    string getTipSpec(){
        return culoare;
    }
    void afisare_produs(ostream& dev) const override{
        Produs::afisare_produs(dev);
        dev<<"Culoare:"<<culoare<<endl;
        dev<<"Marca: "<<marca<<endl;
        dev<<"Pret final: "<<pret_final()<<" RON."<<endl;
    };
};