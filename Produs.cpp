#include "Produs.h"
#include <algorithm>
#include <vector>

int Produs::cod_counter = 0;

Produs::Produs(const string tip, const string den, const int st, const double pret):tip(tip), denumire(den), stoc(st), pret_baza(pret){
        cod = ++cod_counter;
    }

void Produs::afisare_produs(ostream &dev) const{
    dev<<"Cod produs: "<<cod<<endl;
    dev<<"Tipul produsului: "<<tip<<endl;
    dev<<"Denumire produs: "<<denumire<<endl;
    dev<<"Stoc: "<<stoc<<endl;
    dev<<"Pret baza: "<<pret_baza<<" RON."<<endl;
}

void Produs::modifica_stoc(const int cantitate){
    stoc += cantitate;
}

void Produs::modifica_pret_baza(const int alt_pret){
    pret_baza = alt_pret;
}

int Produs::getStoc() const{
    return stoc;
}

int Produs::getCod() const {
    return cod;
}

int Produs::getPret() const{
    return pret_baza;
}

string Produs::getTip() const {
    return tip;
}

string Produs::getDenumire() const {
    return denumire;
}

void Produs::modifica_stoc_dupa_cod(vector<Produs*>& produse, int cod_produs, const int cantitate) {
    // functie lambda pentru cautarea in vector cu iterator
    vector<Produs*>::const_iterator it = find_if(produse.begin(), produse.end(), [cod_produs](Produs* produs) {
        return produs->getCod() == cod_produs; 
    });
    // daca am gasit produsul, il actualizez
    if(it != produse.end()){
        if((cantitate < 0 && (*it)->stoc > abs(cantitate)) || (cantitate > 0)){
            (*it)->modifica_stoc(cantitate);
            cout<<"Stocul produsului cu codul "<< cod_produs << " a fost modificat la "<< (*it)->getStoc() << endl;
        }
        else cout<<"Stocul produsului cu codul "<< cod_produs << " are un stoc prea mic pentru a fi modificat cu "<< cantitate << endl;
        }

    else cout <<"Produsul cu codul "<<cod_produs<<" nu a fost gasit!" << endl;
}

// functie care sterge un produs dupa codul sau
void Produs::sterge_produs_dupa_cod(vector<Produs*>& produse, int cod_produs){
    auto it = find_if(produse.begin(), produse.end(), [cod_produs](Produs* produs){
        return produs->getCod() == cod_produs; 
    });

    if(it != produse.end()){
        delete *it; // sterge obiectul din memorie
        produse.erase(it);  // sterge pointerul din memorie
        cout <<"Produsul cu codul " << cod_produs << " a fost sters.\n";
    }
    else cout << "Produsul cu codul " << cod_produs << " nu a fost gasit.\n";

}

Produs::~Produs(){
}

