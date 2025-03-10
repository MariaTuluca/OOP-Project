#pragma once
#include "Angajat.h"
#include "DiscVintage.h"
#include "ArticolVestimentar.h"
#include "Disc.h"
#include "Produs.h"
#include "Comanda.h"
#include "Angajat.h"
#include "Operator.h"
#include <stdexcept>
#include <sstream>
#include <climits>
#include <set>
#include <queue>

using namespace std;

class MagazinRock {
private:
    vector<Angajat*> angajati;
    vector<Produs*> produse;
    queue<Comanda> comenzi;
    int valoare_bruta(vector<tuple<string, string, int>> &prods);
    int valoare_totala(vector<tuple<string, string, int>> &prods);
    bool validare_comanda(vector<tuple<string, string, int>> &prods);
    tm stringToTm(const string &date);
    void modificari_stocuri(const Comanda& com);
    void valori_raport(const int, const double);
    bool este_valid(int opt, int val);
    void citire_opt(int &opt, int val);
    bool compara_date(const tm data1, const tm data2){
        if(data1.tm_year < data2.tm_year) return true;
        else if(data1.tm_year == data2.tm_year && data1.tm_mon < data2.tm_mon) return true;
        else if(data1.tm_year == data2.tm_year && data1.tm_mon == data2.tm_mon && data1.tm_mday <= data2.tm_mday)
            return true;

        return false; 
    };

public:
    MagazinRock() = default;
    MagazinRock(vector<Angajat*> angaj, vector<Produs*> prods, queue<Comanda> coms);
    MagazinRock(const MagazinRock &mag);
    MagazinRock & operator=(const MagazinRock &mag);
    // constructor de mutare si operator = de mutare
    MagazinRock(MagazinRock &&mag);
    MagazinRock & operator=(MagazinRock &&mag);
    ~MagazinRock();
    void citire_angajati(const string& fisierIn);
    void citire_produse(const string& fisierIn);
    void citire_comenzi(const string &fisierIn);
    queue<Comanda> getComenzi() const;
    void procesarea_comenzilor(const tm data_first);
    void RAPORT_cel_mai_mare_nr_comenzi();
    void RAPORT_cele_mai_valoroase_comenzi();
    void RAPORT_cele_mai_mari_salarii();
    bool exists() const;
    void adaugare_angajat(const string nume, const string prenume, const string data, const string cnp, const string tip);
    void modificare_nume(string nume, const string prenume, const string nume_nou);
    void demisie(int id);
    void stergere_angajat(const string nume, const string prenume);
    void afisare_angajat(ostream &dev, const string nume, const string prenume);
    void afisare_angajati(ostream &dev);
    void modifica_stoc_dupa_cod(const int cod, const int stoc);
    void sterge_produs_dupa_cod(const int cod);
    void adaugare_produs_vest(const string denumire, const int stoc, const double pret_baza, const string culoare, const string marca);
    void adaugare_produs_disc(const string denumire, const int stoc, const double pret_baza, const string csd, const string dtl, const string tr, const string alb, const string tipDisc);
    void adaugare_produs_discVintage(const string denumire, const int stoc, const double pret_baza, const string csd, const string dtl, const string tr, const string alb, const string tipDisc, bool mint, int rar);
    void modificare_produs(const int cod, const int cantitate);
    void stergere_produs(const int cod);
    void afisare_produs(ostream &dev, const int cod);
    void afisare_produse(ostream &dev);
    void afisare_comenzi(ostream &dev);
    void menu();
};

