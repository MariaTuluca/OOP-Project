#include<iostream>
#include "MagazinRock.h"

using namespace std;

int main()
{
    MagazinRock magazin;

    ofstream fout("outDate.out");
    // functie citire angajati si produse
    magazin.citire_angajati("angajati.txt");
    magazin.citire_produse("produse.txt");
    magazin.citire_comenzi("comenzi.txt");

    // afisare angajati
    fout << "Angajati: \n";
    magazin.afisare_angajati(fout);

    // afisare si modificare produse, afisare comenzi
    fout << "\nProduse: \n";
    magazin.afisare_produse(fout);
    fout << "\nComenzi: \n";
    magazin.afisare_comenzi(fout);

    // tm data_first;
    // if (!magazin.getComenzi().empty()) {
    //             Comanda primaComanda = magazin.getComenzi().front();
    //             data_first = primaComanda.getDataComanda();
    //         }
    // magazin.procesarea_comenzilor(data_first); 
    // magazin.RAPORT_cel_mai_mare_nr_comenzi();
    // magazin.RAPORT_cele_mai_mari_salarii();
    // magazin.RAPORT_cele_mai_valoroase_comenzi();

    // meniul 
    string rasp;
    cout << "Doriti sa deschideti meniul?(introduceti nu/da)\n";
    cin >> rasp;
    if(rasp == "da" || rasp == "DA" || rasp == "Da")
        magazin.menu();
    else
        cout << "Bine atunci! La revedere!\n";

     fout.close();
    
    // simulare demisie
    // int id_cautat = 3;
    // magazin.demisie(id_cautat);

    // adaugare, stergere, modificare, afisare angajat
    // magazin.adaugare_angajat("Popescu", "Ionut", "12.12.2024", "1790909419990", "Operator");
    // magazin.stergere_angajat("Popescu", "Ionut");
    // magazin.modificare_nume("Popescu", "Ion", "Radautianu");
    // magazin.afisare_angajat(fout, "Ionescu", "Răzvan");

    // adaugare, stergere, modificare, afisare produs
    // magazin.modifica_stoc_dupa_cod(2, 20);
    // magazin.modifica_stoc_dupa_cod(3, -10);
    // magazin.sterge_produs_dupa_cod(3);
    // magazin.adaugare_produs_vest("Tricou", 10, 50, "alb", "Adidas");
    // magazin.adaugare_produs_discVintage("The Wall", 5, 100, "House of Rock", "12.12.2000", "AC/DC", "Tiger", "CD", 0, 3);
    // magazin.adaugare_produs_disc("Tarararra", 5, 100, "House of Rock", "12.12.2020", "AC/DC", "Tiger", "Vinil");
    // magazin.stergere_produs(11);
    // magazin.modificare_produs(11, -5);
    // magazin.afisare_produs(fout, 10);

    // eliberare memorie automata cu destructor

    return 0;
}