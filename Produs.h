#pragma once
#include<iostream>
#include<fstream>
#include<string.h>
#include<vector>

using namespace std;

class Produs{
protected: 
    string tip;
    static int cod_counter; // counter pentru codul unic al produsului
    int cod; // codul unic al produsului
    string denumire; // numele produsului
    int stoc;
    double pret_baza;

public:
    Produs() = default;
    Produs(const string tip, const string den, const int st, const double pret);    
    virtual double pret_final() const = 0;
    virtual void afisare_produs(ostream &dev) const;
    void modifica_stoc(const int cantitate);
    void modifica_pret_baza(const int alt_pret);
    virtual string getTipSpec() = 0;
    int getStoc() const;
    int getCod() const;
    int getPret() const;
    string getTip() const;
    string getDenumire() const;
    static void modifica_stoc_dupa_cod(vector<Produs*>& produse, int cod_produs, const int cantitate);
    static void sterge_produs_dupa_cod(vector<Produs*>& produse, int cod_produs);

    virtual ~Produs();
};