#pragma once
#include "Produs.h"

class Disc: public Produs{
protected:
    string casa_discuri;
    string data_lansare;    // nu am nevoie de ea in format tm
    string trupa;
    string album;
    string tipDisc; //tipul discurilor

public: 
    Disc(const string tip, const string den, const int st, const double pr, const string cd, const string dl, const string tr, const string alb, const string tipDisc):
        Produs(tip, den, st, pr), casa_discuri(cd), data_lansare(dl), trupa(tr), album(alb), tipDisc(tipDisc){};

    double pret_final() const override{
        return pret_baza + 5; //cost împachetare și livrare
    }

    string getTipSpec(){
        return tipDisc;
    }

    void afisare_produs(ostream& dev) const override{
        Produs::afisare_produs(dev);
        dev<<"Tipul discului: "<<tipDisc<<endl;
        dev<<"Casa discuri: "<<casa_discuri<<endl;
        dev<<"Data lansare: "<<data_lansare<<endl;
        dev<<"Trupa: "<<trupa<<endl;
        dev<<"Album: "<<album<<endl;
        dev<<"Pret final: "<<pret_final()<<" RON."<<endl;
    }
};