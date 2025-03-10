#pragma once
#include <iostream>
#include <vector>
#include <ctime>
#include <tuple>
#include <string.h>

using namespace std;

class Comanda {
private: 
    static int id_counter; // counter pentru id unic
    int ID;
    vector<tuple<string, string, int>> produse;    // produse adaugate in comanda
    tm data_comanda;
    int durata_solutionare;     //  durata de solutionare (in zile)
    tm data_finalizare; // data finalizarii comenzii
    double valoare_bruta;  // valoarea bruta a comenzii, fara costuri de livrare 
    double valoare_totala; // valoarea finala a comenzii

public: 
    Comanda(vector<tuple<string, string, int>> produse, tm data, tm final, int durata, double val_brut, double val_total);

    int getID() const { return ID;}

    vector<tuple<string, string, int>> getProduse() const { return produse; }

    tm getDataComanda() const {return data_comanda;}

    double getValoareBruta() const {return valoare_bruta;}

    double getValoareTotala() const {return valoare_totala;}

    int getDurataSolutionare() const {return durata_solutionare; }

    void setDataComanda(const tm& dataNoua) { data_comanda = dataNoua; }

    tm getDataFinalizare() const;

    friend ostream& operator<<(ostream& dev, const Comanda& comanda);

    ~Comanda(){}
};