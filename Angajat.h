#pragma once
#include<iostream>
#include<string.h>
#include<vector>
#include<fstream>
#include<ctime>

using namespace std;

class Angajat{
protected:
    static int counter; // counter pentru numarul de angajati
    string tip_angajat; // tipul angajatului
    static int id_counter;  // counter pentru id-ul unic
    int ID;
    string nume;
    string prenume;
    string CNP;
    tm data_angajare; // data angajarii
    double salariu; // salariul prelucrat in functie de tipul angajatului
    // variabile interne pentru rapoarte
    int nr_comenzi = 0;        // număr de comenzi procesate
    double val_comenzi = 0.0; // valoarea totală a comenzilor

public: 
    Angajat(); 
    Angajat(const string nm, const string prenm, tm data_ang, const string cnp, const string tip_ang);
    bool validare_CNP();
    double calcul_salariu() const;
    void set_nume(const string& nm){
        nume = nm;
    };
    int getID() const { return ID; }
    string getTip() const { return tip_angajat; }
    string getNume() const { return nume; }
    string getPrenume() const { return prenume; }
    void set_tip(const string& tip_nou){
        tip_angajat = tip_nou;
    };
    void modifica_nr_comenzi(const int nr){
        nr_comenzi += nr;
    }
    void modifica_val_comenzi(const double val){
        val_comenzi += val;
    }
    int getNrComenzi() const { return nr_comenzi; }
    double getValComenzi() const { return val_comenzi; }
    void resetCounter();
    friend ostream& operator<<(ostream& dev, const Angajat& angajat);
    friend istream& operator>>(istream& dev, Angajat& angajat); 
};