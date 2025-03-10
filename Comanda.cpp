#include "Comanda.h"
#include <algorithm>
int Comanda::id_counter = 0;

Comanda::Comanda(vector<tuple<string, string, int>> produse, tm data, tm final, int durata, double val_brut, double val_total): produse(produse), data_comanda(data), durata_solutionare(durata), data_finalizare(final), valoare_bruta(val_brut), valoare_totala(val_total)
{
    ID = ++id_counter;
}

tm Comanda::getDataFinalizare() const{
    tm data_finalizare = data_comanda;

    data_finalizare.tm_mday += durata_solutionare;
    mktime(&data_finalizare);

    return data_finalizare;
}

ostream &operator<<(ostream &dev, const Comanda &comanda)
{
    dev << "Comanda cu ID-ul: " << comanda.ID << endl;
    dev << "Produse comandate: " << endl;
    for (auto produs : comanda.produse)
    {
        dev << " - " << get<0>(produs) << " cu caracteristica " << get<1>(produs) << " si cantitatea "<< get<2>(produs)<<".\n";
    }
    dev << "Data primire comanda: " << comanda.data_comanda.tm_mday << "."
        << comanda.data_comanda.tm_mon + 1 << "." << comanda.data_comanda.tm_year + 1900 << endl;
    dev << "Durata solutionare: " << comanda.durata_solutionare << " zile." << endl;
    dev << "Valoare comanda (fara livrare si impachetare): " << comanda.valoare_bruta<< " RON." << endl;
    dev << "Valoare totala comanda: " << comanda.valoare_totala << " RON." << endl;
    dev << endl;

    return dev;
}
