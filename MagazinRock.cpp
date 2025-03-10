#include "MagazinRock.h"
#include <fstream>
#include <unordered_map>
#include <map>
#include <limits>
#include <algorithm>
#include <set>
#include <iomanip> // pentru std::setw si std::setfill
#include <stdexcept>

MagazinRock::MagazinRock(vector<Angajat *> angaj, vector<Produs *> prods, queue<Comanda> coms)
{
    angajati = angaj;
    produse = prods;
    comenzi = coms;
}

MagazinRock::MagazinRock(const MagazinRock &mag)
{
    angajati = mag.angajati;
    produse = mag.produse;
    comenzi = mag.comenzi;
}

MagazinRock &MagazinRock::operator=(const MagazinRock &mag)
{
    if (this != &mag)
    {
        angajati.clear();
        for (auto it : mag.angajati)
            angajati.push_back(it);

        produse.clear();
        produse = mag.produse;
        for (auto it : mag.produse)
            produse.push_back(it);

        comenzi = mag.comenzi;
    }
    return *this;
}

MagazinRock::MagazinRock(MagazinRock &&mag)
{
    angajati = move(mag.angajati);
    produse = move(mag.produse);
    comenzi = move(comenzi);
    // eliberare memorie a magazinului
    mag.angajati.clear();
    mag.produse.clear();
    while (!mag.comenzi.empty())
    {
        mag.comenzi.pop();
    }
}

MagazinRock &MagazinRock::operator=(MagazinRock &&mag)
{
    if (this != &mag)
    {
        angajati.clear();
        angajati = move(mag.angajati);
        produse.clear();
        produse = move(mag.produse);
        while (!comenzi.empty())
        {
            comenzi.pop();
        }
        comenzi = move(mag.comenzi);
        mag.angajati.clear();
        mag.produse.clear();
        while (!mag.comenzi.empty())
        {
            mag.comenzi.pop();
        }
    }
    return *this;
}

MagazinRock::~MagazinRock()
{
    angajati.clear();
    produse.clear();
}

// operator == pentru date de tip tm
bool operator==(const tm &data1, const tm &data2)
{
    return (data1.tm_year == data2.tm_year &&
            data1.tm_mon == data2.tm_mon &&
            data1.tm_mday == data2.tm_mday);
}

// functie de convertire string in tm
tm MagazinRock::stringToTm(const string &date)
{
    tm data = {0};
    sscanf(date.c_str(), "%d.%d.%d", &data.tm_mday, &data.tm_mon, &data.tm_year);
    data.tm_mon -= 1;     // ajustare luna
    data.tm_year -= 1900; // ajustare an
    return data;
}

// operator << pentru date tm
ostream &operator<<(ostream &out, const tm &data)
{
    out << setw(2) << setfill('0') << data.tm_mday << "."    // Ziua
        << setw(2) << setfill('0') << data.tm_mon + 1 << "." // Luna (tm_mon începe de la 0)
        << data.tm_year + 1900;                              // Anul (tm_year este numărat din 1900)

    return out;
}

int MagazinRock::valoare_bruta(vector<tuple<string, string, int>> &prods)
{
    int valoare = 0;
    for (auto it : prods)
    {
        string denumire = get<0>(it);
        string tip = get<1>(it);
        int cantitate = get<2>(it);
        for (auto it1 : produse)
        {
            if (it1->getDenumire() == denumire && it1->getTipSpec() == tip)
            {
                valoare += it1->getPret() * cantitate;
                break;
            }
        }
    }
    return valoare;
}

int MagazinRock::valoare_totala(vector<tuple<string, string, int>> &prods)
{
    int valoare = 0;
    for (auto it : prods)
    {
        string denumire = get<0>(it);
        string tip = get<1>(it);
        int cantitate = get<2>(it);
        for (auto it1 : produse)
        {
            if (it1->getDenumire() == denumire && it1->getTipSpec() == tip)
            {
                valoare += it1->pret_final() * cantitate;
                break;
            }
        }
    }
    return valoare;
}
// funcție pentru adăugarea unui număr de zile la o dată
tm calcDataFinalizare(const tm &time_start, int days)
{
    time_t time_sec = mktime(const_cast<tm *>(&time_start)); // convertire în timp brut (secunde)
    time_sec += days * 86400;                               // (1 zi = 86400 secunde)

    tm *time_end = localtime(&time_sec); // înapoi în struct tm
    return *time_end;
}

bool MagazinRock::validare_comanda(vector<tuple<string, string, int>> &prods)
{
    int nr_disc = 0;
    int nr_artVest = 0;
    double sumaBrut_comanda = valoare_bruta(prods);
    // verificare numar produse
    for (auto item : prods){
        bool produsGasit = false;
        for (auto produs : produse)
        {
            if (produs->getDenumire() == get<0>(item) && produs->getTipSpec() == get<1>(item))
            {   produsGasit = true;
                if (produs->getStoc() < get<2>(item))
                {
                    cout << "Stoc insuficient pentru produsul: " << produs->getTip() << ", cu codul: " << produs->getCod() << ".\n";
                    return false;
                }else{
                    produs->modifica_stoc(-get<2>(item));
                    if (dynamic_cast<Disc*>(produs) || dynamic_cast<DiscVintage*>(produs)) {
                        nr_disc += get<2>(item);
                    } else if (dynamic_cast<ArticolVestimentar*>(produs)) {
                        nr_artVest += get<2>(item);
                    }
                }
                break;
            }
        }
    }

    if (nr_disc > 5 || nr_artVest > 3)
    {
        cout << "Comanda trebuie să conțină maxim 3 articole vestimentare și 5 discuri!\n";
        return false;
    }

    if (sumaBrut_comanda < 100)
    {
        cout << "Valoarea minimă a comenzii brute trebuie să fie 100RON!\n";
        return false;
    }

    return true;
}

void MagazinRock::modificari_stocuri(const Comanda &com)
{
    for (const auto &item : com.getProduse())
    {
        for (auto &produs : produse)
            if (produs->getDenumire() == get<0>(item) && produs->getTipSpec() == get<1>(item))
                produs->modifica_stoc(-get<2>(item));
        
    }
}

void MagazinRock::valori_raport(const int id, const double val)
{
    for (auto &angajat : angajati)
    {
        if (angajat->getID() == id)
        {
            angajat->modifica_nr_comenzi(1);
            angajat->modifica_val_comenzi(val);
        }
    }
}

// functie de citire date angajati din fisier
void MagazinRock::citire_angajati(const string &fisierIn)
{
    ifstream fis(fisierIn);
    if (!fis.is_open())
    {
        cout << "Fisierul de citire angajati nu poate fi deschis!" << endl;
        return;
    }

    string nume, prenume, cnp, tip, dataAng; // citesc data angajarii ca pe un string

    while (fis >> nume >> prenume >> cnp >> dataAng >> tip)
    {
        // verifică dacă CNP-ul este valid
        Angajat angajat_temp(nume, prenume, stringToTm(dataAng), cnp, tip); // crează un obiect temporar pentru validare
        if (!angajat_temp.validare_CNP())
        {
            cout << "CNP invalid pentru angajatul: " << nume << " " << prenume << endl;
            continue; // dacă CNP-ul nu este valid, sarim la următorul angajat
        }
        angajat_temp.resetCounter();
        // transform data din string in tm
        tm dataAng_noua = {};
        stringstream ss(dataAng); // lucrez pe stream de string
        string an, luna, zi;
        getline(ss, zi, '.');
        getline(ss, luna, '.');
        getline(ss, an);

        dataAng_noua.tm_year = stoi(an) - 1900;
        dataAng_noua.tm_mon = stoi(luna) - 1;
        dataAng_noua.tm_mday = stoi(zi);
        // pun angajatul în vectorul de angajati
        int luna_nasterii = std::stoi(cnp.substr(3, 2));
        int ziua_nasterii = std::stoi(cnp.substr(5, 2));
        int anul_nasterii = std::stoi(cnp.substr(1, 2));

        if (anul_nasterii < 24)
            anul_nasterii += 2000; // mileniul 3
        else
            anul_nasterii += 1900; // mileniul 2
        //  verificarea varstei la data angajarii
        int varsta = dataAng_noua.tm_year + 1900 - anul_nasterii;
        // am verificat functia de citire cu mai multe tipuri de cnp-uri invalide
        if (varsta > 18 || (varsta == 18 && dataAng_noua.tm_mon > luna_nasterii) ||
            ((varsta == 18 && dataAng_noua.tm_mon == luna_nasterii && dataAng_noua.tm_mday >= ziua_nasterii)))
            angajati.push_back(new Angajat(nume, prenume, dataAng_noua, cnp, tip));
        else cout << "Nu este major la data angajării!: " << nume << " " << prenume << "\n";
        
    }
    fis.close();
}

// functie de citire produse din fisier
void MagazinRock::citire_produse(const string &fisierIn)
{
    ifstream fis(fisierIn);
    if (!fis.is_open())
    {
        cout << "Fisierul de citire produse nu poate fi deschis!" << endl;
        return;
    }

    string buf;
    while (getline(fis, buf))
    {
        stringstream ss(buf);
        string tip, denumire, culoare, marca, casaDiscuri, dataLansare, trupa, album, tipDisc;
        int stoc, coefRar;
        double pret_baza;
        bool mint;

        ss >> tip >> denumire >> stoc >> pret_baza;

        if (tip == "ArticolVestimentar")
        {
            ss >> culoare >> marca;
            produse.push_back(new ArticolVestimentar(tip, denumire, stoc, pret_baza, culoare, marca));
        }
        else if (tip == "Disc")
        {
            ss >> casaDiscuri >> dataLansare >> trupa >> album >> tipDisc;
            produse.push_back(new Disc(tip, denumire, stoc, pret_baza, casaDiscuri, dataLansare, trupa, album, tipDisc));
        }
        else if (tip == "DiscVintage")
        {
            ss >> casaDiscuri >> dataLansare >> trupa >> album >> tipDisc >> coefRar >> mint;
            produse.push_back(new DiscVintage(tip, denumire, stoc, pret_baza, casaDiscuri, dataLansare, trupa, album, tipDisc, mint, coefRar));
        }
    }

    fis.close();
}

void MagazinRock::citire_comenzi(const string &fisierIn)
{
    ifstream fin(fisierIn);
    if (!fin.is_open())
    {
        cout << "Fisierul de citire comenzi nu poate fi deschis!" << endl;
        return;
    }

    int nr_comenzi;
    fin >> nr_comenzi;

    for (int i = 0; i < nr_comenzi; i++)
    {
        int nr_produse;
        fin >> nr_produse;
        vector<tuple<string, string, int>> prods;
        for (int j = 0; j < nr_produse; j++)
        {
            string denumire;
            int cantitate;
            string tip;
            fin >> denumire >> cantitate >> tip;
            prods.push_back(make_tuple(denumire, tip, cantitate));
        }
        string data;
        fin >> data;
        tm data_comanda = stringToTm(data);
        int durata;
        fin >> durata;

        if (validare_comanda(prods))
        {
            cout << "Comanda " << i + 1 << ": " << "este inregistrata! \n";
            comenzi.push(Comanda(prods, data_comanda, calcDataFinalizare(data_comanda, durata), durata, valoare_bruta(prods), valoare_totala(prods)));
        }
        else
            cout << "Comanda " << i + 1 << ": " << "nu este inregistrata! \n";
    }

    fin.close();
}

queue<Comanda> MagazinRock::getComenzi() const
{
    return comenzi;
}

void MagazinRock::procesarea_comenzilor(tm data_first)
{
    set<Operator> simulationSet;
    cout << "Operatori: ";
    for (auto angajat : angajati)
        if (angajat->getTip() == "Operator")
        {
            auto it = simulationSet.insert(Operator(angajat->getID(), new vector<Comanda>));
            cout << angajat->getID() << " " << angajat->getTip() << "   ";
        }

    cout << simulationSet.size() << endl;

    ofstream fout("outDateProcesate.out");

    fout << "Procesare la momentul: " << data_first.tm_mday << "." << data_first.tm_mon + 1 << "." << data_first.tm_year + 1900 << "\n";
    while (!comenzi.empty() || (comenzi.empty() && !simulationSet.empty()))
    {
        // finalizez comenzile operatorilor
        for (auto &op : simulationSet)
        {
            auto &comenzi_op = *op.getComenzi();
            comenzi_op.erase(remove_if(comenzi_op.begin(), comenzi_op.end(), [&](const Comanda &comanda){
                tm data_com = comanda.getDataFinalizare();
                mktime(&data_com);
                if (data_com == data_first) {
                    fout << "- finalizarea procesarii comenzii: " << comanda.getID() << "\n";
                    return true;
                }
                return false; }),comenzi_op.end());   
        }

        // procesarea următoarei comenzi din coadă
        if (!comenzi.empty())
        {
            Comanda comanda_procesata = comenzi.front();
            comenzi.pop();
            fout << "- procesarea comenzii " << comanda_procesata.getID() << "\n";

            auto op_it = min_element(simulationSet.begin(), simulationSet.end(), [](const Operator &a, const Operator &b)
                { return a.getComenzi()->size() < b.getComenzi()->size(); });

            if (op_it->getComenzi()->size() < 3)
            {
                // adaug comanda operatorului găsit, il modific in set
                Operator op = *op_it;
                auto it = simulationSet.erase(op_it);
                op.adauga_comanda(comanda_procesata);
                simulationSet.insert(op);

                // modific stocurile si datele pentru rapoarte
                modificari_stocuri(comanda_procesata);
                valori_raport(op.getID(), comanda_procesata.getValoareTotala());
            }
            else
            {  
                fout << "Operatori ocupati! Comanda " << comanda_procesata.getID() << " amanata!\n";
                tm data_comanda = comanda_procesata.getDataComanda();
                data_comanda.tm_mday++;
                mktime(&data_comanda);
                comanda_procesata.setDataComanda(data_comanda);

                queue<Comanda> temp_queue;
                temp_queue.push(comanda_procesata); // adaug comanda la început

                while (!comenzi.empty())
                {   
                    Comanda com = comenzi.front();
                    tm data_comanda = com.getDataComanda();
                    data_comanda.tm_mday++;
                    mktime(&data_comanda);
                    com.setDataComanda(data_comanda);
                    temp_queue.push(com);
                    comenzi.pop();
                }
                comenzi = temp_queue; // înlocuim coada originală
            }
        }
        else {// un operator a terminat toate comenzile, chiar si pe cele nefinalizate după golirea cozii 
            for (auto it = simulationSet.begin(); it != simulationSet.end(); ){
                if( it->getComenzi()->empty() ) 
                    it = simulationSet.erase(it);
                else it++;
            }
        }

        // afișez statusul comenzilor si operatorilor
        for (const auto &op : simulationSet)
        {
            if (!op.getComenzi()->empty())
            {
                fout << "Comenzile cu id-urile:  ";
                for (const auto &comanda : *op.getComenzi())
                {
                    fout << comanda.getID() << " ";
                }
                fout << " îi sunt asignate operatorului cu id-ul: " << op.getID() << ".\n";
            }
        }

        // vreau sa faca procesarea tuturor comenzilor plasate in ziua x
        if(!comenzi.empty()){
            tm data = comenzi.front().getDataComanda();
            if (data_first.tm_mday != data.tm_mday) // am terminat de procesat pe ziua aceasta
            {
                data_first.tm_mday++;
                fout << "\nProcesare la momentul: " << data_first.tm_mday << "." << data_first.tm_mon + 1 << "." << data_first.tm_year + 1900 << "\n";
            }
        }
        else{
            data_first.tm_mday++;
            if(!simulationSet.empty()) 
                fout << "\nProcesare la momentul: " << data_first.tm_mday << "." << data_first.tm_mon + 1 << "." << data_first.tm_year + 1900 << "\n";
        }
    }

    fout << endl;
}

void MagazinRock::RAPORT_cel_mai_mare_nr_comenzi()
{
    ofstream fir("RAPORT_cel_mai_mare_nr_comenzi.csv");
    fir << "\nAngajatul cu cel mai mare numar de comenzi: \n\n";

    Angajat *best_angajat = angajati[0];
    for (auto i = 1; i < angajati.size(); ++i)
    {
        if (angajati[i]->getNrComenzi() > best_angajat->getNrComenzi())
            best_angajat = angajati[i];
    }

    fir << "Nume -- Prenume -- Număr de comenzi procesate:\n";
    fir << best_angajat->getNume() << " "
        << best_angajat->getPrenume() << " -- "
        << best_angajat->getNrComenzi() << "\n\n";

    fir.close();
}

void MagazinRock::RAPORT_cele_mai_valoroase_comenzi()
{
    ofstream fir("RAPORT_cele_mai_valoroase_comenzi.csv");
    fir << "\nAngajatii cu cele mai valoroase comenzi: \n\n";

    // functie lambda de sortare in mod descrescator, in functie de valoarea comenzilor
    sort(angajati.begin(), angajati.end(), [](const Angajat *a1, const Angajat *a2)
         { return a2->getValComenzi() < a1->getValComenzi(); });

    for (int i = 0; i < 3; i++)
    {
        fir << "Nume -- Prenume: " << angajati[i]->getNume() << " " << angajati[i]->getPrenume() << "\n";
        fir << "Valoarea comenzilor procesate: " << angajati[i]->getValComenzi();
        fir << "\n\n";
    }
    fir.close();
}

void MagazinRock::RAPORT_cele_mai_mari_salarii()
{
    ofstream fir("RAPORT_cele_mai_mari_salarii.csv");
    fir << "\nAngajatii cu cele mai mari salarii: \n\n";

    // functie lambda de sortare in mod descrescator, in functie de salariu
    sort(angajati.begin(), angajati.end(), [](const Angajat *a1, const Angajat *a2)
         {
        // sortare dupa salariu
        if(a1->calcul_salariu() == a2->calcul_salariu()){
            // sortare dupa nume si prenume
            if(a1->getNume() == a2->getNume())
                return a1->getPrenume() < a2->getPrenume();
            return a1->getNume() < a2->getNume();
        }
        return a1->calcul_salariu() > a2->calcul_salariu(); });

    for (int i = 0; i < 3; i++)
    {
        fir << "Nume -- Prenume: " << angajati[i]->getNume() << " " << angajati[i]->getPrenume() << "\n";
        fir << "Salariu: " << angajati[i]->calcul_salariu();
        fir << "\n\n";
    }
    fir.close();
}

bool MagazinRock::exists() const
{
    // verificare angajati
    int mg = 0, op = 0, as = 0;
    for (auto it : angajati)
    {
        string tip = it->getTip();

        if (tip == "Asistent")
            as++;
        else if (tip == "Manager")
            mg++;
        else
            op++;
    }
    if (mg < 1 && op < 3 && as < 1)
    {
        cout << "Magazinul are prea putini angajati! Nu poate functiona!\n";
        return false;
    }

    // verificare stocuri
    for (auto it : produse)
        if ((*it).getStoc() < 2)
        {
            cout << "Magazinul are prea putine produse! Nu poate functiona!\n";
            return false;
        }
    return true;
}

void MagazinRock::adaugare_angajat(const string nume, const string prenume, const string data, const string cnp, const string tip)
{
    Angajat angajat_temp(nume, prenume, stringToTm(data), cnp, tip); // crează un obiect temporar pentru validare
    if (!angajat_temp.validare_CNP())
    {
        cout << "CNP invalid pentru angajatul: " << nume << " " << prenume << endl;
    }
    angajat_temp.resetCounter();
    // transform data din string in tm
    tm dataAng_noua = {};
    stringstream ss(data); // lucrez pe stream de string
    string an, luna, zi;
    getline(ss, zi, '.');
    getline(ss, luna, '.');
    getline(ss, an);

    dataAng_noua.tm_year = std::stoi(an) - 1900;
    dataAng_noua.tm_mon = std::stoi(luna) - 1;
    dataAng_noua.tm_mday = std::stoi(zi);
    int luna_nasterii = std::stoi(cnp.substr(3, 2));
    int ziua_nasterii = std::stoi(cnp.substr(5, 2));
    int anul_nasterii = std::stoi(cnp.substr(1, 2));

    if (anul_nasterii < 24)
        anul_nasterii += 2000; // mileniul 3
    else
        anul_nasterii += 1900; // mileniul 2
    //  verificarea varstei la data angajarii
    int varsta = dataAng_noua.tm_year + 1900 - anul_nasterii;
    // am verificat functia de citire cu mai multe tipuri de cnp-uri invalide
    if (varsta > 18 || (varsta == 18 && dataAng_noua.tm_mon > luna_nasterii) ||
        ((varsta == 18 && dataAng_noua.tm_mon == luna_nasterii && dataAng_noua.tm_mday >= ziua_nasterii)))
        angajati.push_back(new Angajat(nume, prenume, stringToTm(data), cnp, tip));
}

void MagazinRock::modificare_nume(string nume, const string prenume, const string nume_nou)
{
    bool found = false;

    for (auto it : angajati)
        if ((*it).getNume() == nume && (*it).getPrenume() == prenume)
        {
            found = true;
            (*it).set_nume(nume_nou);
            break;
        }
    if (found == false)
        cout << "Nu am gasit un angajat cu acest nume si acest prenume!\n";
}

void MagazinRock::demisie(int id)
{

    vector<Angajat *>::iterator it = angajati.begin();
    while (it != angajati.end())
    {
        if ((*it)->getID() == id)
        {
            cout << "Angajatul cu ID-ul " << id << " a fost sters.\n";
            delete *it;
            it = angajati.erase(it);
            (*it)->resetCounter();
            return;
        }
        else
        {
            ++it;
        }
    }
}

void MagazinRock::stergere_angajat(const string nume, const string prenume)
{
    bool found = false;

    vector<Angajat *>::iterator it = angajati.begin();
    while (it != angajati.end())
    {
        if ((*it)->getNume() == nume && (*it)->getPrenume() == prenume)
        {
            found = true;
            angajati.erase(it);
            break;
        }
        it++;
    }
    if (found == false)
        cout << "Nu am gasit un angajat cu acest nume si acest prenume!\n";
}

void MagazinRock::afisare_angajat(ostream &dev, const string nume, const string prenume)
{
    bool found = false;

    vector<Angajat *>::iterator it = angajati.begin();
    while (it != angajati.end())
    {
        if ((*it)->getNume() == nume && (*it)->getPrenume() == prenume)
        {
            found = true;
            dev << (**it);
            break;
        }
        it++;
    }
    if (found == false)
        dev << "Nu am gasit un angajat cu acest nume si acest prenume!\n";
}

void MagazinRock::afisare_angajati(ostream &dev)
{
    for (auto iter : angajati)
    {
        dev << *iter;
        dev << endl;
    }
}

void MagazinRock::modifica_stoc_dupa_cod(const int cod, const int stoc)
{
    Produs::modifica_stoc_dupa_cod(produse, cod, stoc);
}

void MagazinRock::sterge_produs_dupa_cod(const int cod)
{
    Produs::sterge_produs_dupa_cod(produse, cod);
}

void MagazinRock::adaugare_produs_vest(const string denumire, const int stoc, const double pret_baza, const string culoare, const string marca)
{
    ;
    produse.push_back(new ArticolVestimentar("ArticolVestimentar", denumire, stoc, pret_baza, culoare, marca));
}

void MagazinRock::adaugare_produs_disc(const string denumire, const int stoc, const double pret_baza, const string csd, const string dtl, const string tr, const string alb, const string tipDisc)
{
    produse.push_back(new Disc("Disc", denumire, stoc, pret_baza, csd, dtl, tr, alb, tipDisc));
}

void MagazinRock::adaugare_produs_discVintage(const string denumire, const int stoc, const double pret_baza, const string csd, const string dtl, const string tr, const string alb, const string tipDisc, bool mint, int rar)
{
    produse.push_back(new DiscVintage("DiscVintage", denumire, stoc, pret_baza, csd, dtl, tr, alb, tipDisc, mint, rar));
}

void MagazinRock::modificare_produs(const int cod, const int cantitate)
{
    // functie lambda pentru cautarea in vector cu iterator
    vector<Produs *>::const_iterator it = find_if(produse.begin(), produse.end(), [cod](Produs *produs){ 
        return produs->getCod() == cod; 
        });
    // daca am gasit produsul, il actualizez
    if (it != produse.end())
    {
        if ((cantitate < 0 && (*it)->getStoc() > abs(cantitate)) || (cantitate > 0))
        {
            (*it)->modifica_stoc(cantitate);
            cout << "Stocul produsului cu codul " << cod << " a fost modificat la " << (*it)->getStoc() << endl;
        }else
            cout << "Stocul produsului cu codul " << cod << " are un stoc prea mic pentru a fi modificat cu " << cantitate << endl;
    }else
        cout << "Produsul cu codul " << cod << " nu a fost gasit!" << endl;
}

void MagazinRock::stergere_produs(const int cod)
{
    Produs::sterge_produs_dupa_cod(produse, cod);
}

void MagazinRock::afisare_produs(ostream &dev, const int cod)
{
    int ok = 0;

    vector<Produs *>::iterator it = produse.begin();
    while (it != produse.end())
    {
        if ((*it)->getCod() == cod)
        {
            ok = 1;
            (*it)->afisare_produs(dev);
            break;
        }
        it++;
    }
    if (ok == 0)
        dev << "Nu am gasit un produs cu acest cod!\n";
}

void MagazinRock::afisare_produse(ostream &dev)
{
    for (auto it : produse)
    {
        it->afisare_produs(dev);
        dev << endl;
    }
}

void MagazinRock::afisare_comenzi(ostream &dev)
{
    queue<Comanda> comenzi_temp = comenzi;
    while (!comenzi_temp.empty())
    {
        dev << comenzi_temp.front();
        comenzi_temp.pop();
    }
}

bool MagazinRock::este_valid(int opt, int val)
{
    if (typeid(opt) != typeid(int)) return false;
    return (opt >= 1 && opt <= val);
}

void MagazinRock::citire_opt(int &opt, int val)
{
    while (!este_valid(opt, val))
    {
        cout << "Nu exista aceasta operatiune! Incearca din nou!\n";
        cin >> opt;
    }
}

void MagazinRock::menu()
{
    while (this->exists())
    {
        cout << " Acesta este meniul cu optiuni: \n";
        cout << " Operatiunea 1- gestiune angajat.\n";
        cout << " Operatiunea 2- gestiune produse.\n";
        cout << " Operatiunea 3- procesare comenzi.\n";
        cout << " Operatiunea 4- rapoarte.\n";

        cout << "Ce operatiune doriti? Tastati o cifra: ";
        int operatiune;
        cin >> operatiune;
        citire_opt(operatiune, 4);

        switch (operatiune){
        case 1 :
        { // operatiunile cu angajati
            cout << " Acesta este meniul cu optiuni pentru angajati: \n";
            cout << " Operatiunea 1- Adaugare angajat.\n";
            cout << " Operatiunea 2- Modificare angajat.\n";
            cout << " Operatiunea 3- Stergere angajat.\n";
            cout << " Operatiunea 4- Afisarea datelor unui angajat.\n";
            cout << " Operatiunea 5- Afisarea datelor tuturor angajatilor.\n";

            cout << "Ce operatiune doriti? Tastati o cifra: ";
            int opt;
            cin >> opt;
            citire_opt(opt, 5);

            switch (opt)
            {
            case 1:
            {
                cout << "Numele, prenumele, cnp-ul, data angajarii si specializarea pentru angajatul dorit: \n";
                string num, pren, cnp, data, tip;
                cin >> num >> pren >> cnp >> data >> tip;
                adaugare_angajat(num, pren, data, cnp, tip);
                break;
            }
            case 2:
            {
                cout << "Care sunt numele si prenumele angajatului pe care doriti sa-l modificati?\n";
                string nume, prenume;
                cin >> nume >> prenume;
                cout << "Cum doriti sa-l/ sa o cheme acum pe angajatul/angajata " << prenume << "?";
                string nume_nou;
                cin >> nume_nou;
                modificare_nume(nume, prenume, nume_nou);
                break;
            }
            case 3:
            {
                cout << "Care sunt numele si prenumele angajatului care nu mai este angajat?\n";
                string nume, prenume;
                cin >> nume >> prenume;
                stergere_angajat(nume, prenume);
                break;
            }
            case 4:
            {
                cout << "Care sunt numele si prenumele angajatului dorit?\n";
                string nume, prenume;
                cin >> nume >> prenume;
                afisare_angajat(cout, nume, prenume);
                break;
            }
            case 5:
            {
                afisare_angajati(cout);
                break;
            }
            default:
                cout << "Optiune invalida! Incercati din nou.\n";
            }
            break;
        }
        case 2:
        { // operatiunile cu produse
            cout << " Acesta este meniul cu optiuni pentru produse: \n";
            cout << " Operatiunea 1- Adaugare produs.\n";
            cout << " Operatiunea 2- Modificare stoc produs dupa cod.\n";
            cout << " Operatiunea 3- Stergere produs.\n";
            cout << " Operatiunea 4- Afisarea datelor unui produs.\n";
            cout << " Operatiunea 5- Afisarea datelor tuturor produselor.\n";

            cout << "Ce operatiune doriti? Tastati o cifra: ";
            int opt;
            cin >> opt;
            citire_opt(opt, 5);
            switch (opt)
            {
            case 1:
            {
                cout << "Ce tip de produs adaugati(artV, disc, discV)?\n";
                string tip;
                cin >> tip;
                cout << "Denumirea, stocul si pretul de baza pentru produsul dorit: \n";
                string denumire;
                int stoc;
                double pret_baza;
                cin >> denumire >> stoc >> pret_baza;
                if (tip == "artV")
                {
                    cout << "Ce culoare si marca doriti pentru articolul vestimentar?\n";
                    string culoare, marca;
                    cin >> culoare >> marca;
                    tip = "ArticolVestimentar";
                    adaugare_produs_vest(denumire, stoc, pret_baza, culoare, marca);
                }
                else if (tip == "disc")
                {
                    cout << "Ce casa de discuri, data de lansare, trupa, album si tip de disc(CD sau vinil) doriti pentru acest disc?\n";
                    string csd, dtl, tr, alb, tipDisc;
                    cin >> csd >> dtl >> tr >> alb >> tipDisc;
                    tip = "Disc";
                    adaugare_produs_disc(denumire, stoc, pret_baza, csd, dtl, tr, alb, tipDisc);
                }
                else if (tip == "discV")
                {
                    cout << "Ce casa de discuri, data de lansare, trupa, album si tip de disc(CD sau vinil) doriti pentru acest disc vintage?\n";
                    string csd, dtl, tr, alb, tipDisc;
                    cin >> csd >> dtl >> tr >> alb >> tipDisc;
                    cout << "Este discul mint(0=nu/1=da)?\n";
                    bool mn;
                    cin >> mn;
                    cout << "Ce coeficient de raritate are(1-5)?\n";
                    int r;
                    cin >> r;
                    tip = "DiscVintage";
                    adaugare_produs_discVintage(denumire, stoc, pret_baza, csd, dtl, tr, alb, tipDisc, mn, r);
                }
                else {
                    cout << "Nu există produse de acest gen în Magazin!\n";
                }
                break;
            }
            case 2:
            {
                cout << "Care este codul produsului pe care doriti sa-l modificati si ce cantitate doriti sa adaugati/scadeti?\n";
                int cod, cantitate;
                cin >> cod >> cantitate;
                modificare_produs(cod, cantitate);
                break;
            }
            case 3:
            {
                cout << "Care este codul produsului pe care doriti sa-l stergeti din magazin?\n";
                int cod;
                cin >> cod;
                stergere_produs(cod);
                break;
            }
            case 4:
            {
                cout << "Care este codul produsului pe care doriti sa-l afisati?\n";
                int cod;
                cin >> cod;
                afisare_produs(cout, cod);
                break;
            }
            case 5:
            {
                afisare_produse(cout);
                break;
            }
            default:
                cout << "Optiune invalida! Incercati din nou.\n";
            }
            break;
        }
        case 3:
        { // operatiunea de procesare comenzi
            cout << "Procesarea comenzilor:\n";
            tm data_first;
            // preiau data primei comenzi
            if (!getComenzi().empty())
            {
                Comanda primaComanda = getComenzi().front();
                data_first = primaComanda.getDataComanda();
            }
            // procesarea comenzilor
            procesarea_comenzilor(data_first);
            cout << "Procesare finalizata! Datele se află în outDateProcesate.out .\n";
            break;
        }
        case 4: 
        { // operatiunile cu rapoarte
            cout << " Acesta este meniul cu optiuni pentru rapoarte: \n";
            cout << " Optiunea 1: Raportul cu angajatul cu cel mai mare număr de comenzi procesate.\n";
            cout << " Optiunea 2: Raportul cu topul de angajați cu cele mai valoroase comenzi.\n";
            cout << " Optiunea 3: Raportul cu topul de angajați cu cele mai mari salarii din luna aceasta.\n";

            cout << "Ce operatiune doriti? Tastati o cifra: ";
            int opt;
            cin >> opt;
            citire_opt(opt, 3);
            cout << "Generare raport ...";
            switch (opt)
            {
            case 1:
            {
                RAPORT_cel_mai_mare_nr_comenzi();
                cout << "RAPORT_cel_mai_mare_nr_comenzi.csv ." << endl;
                break;
            }
            case 2:
            {
                RAPORT_cele_mai_valoroase_comenzi();
                cout << "RAPORT_cele_mai_valoroase_comenzi.csv ." << endl;
                break;
            }
            case 3:
            {
                RAPORT_cele_mai_mari_salarii();
                cout << "RAPORT_cele_mai_mari_salarii.csv ." << endl;
                break;
            }
            default:
                cout << "Optiune invalida! Incercati din nou.\n";
            }
            break;
        }
        }

        cout << "Continuati cu operatiunile asupra magazinului? Introduceti da/nu cu litere mici!" << endl;
        string ask;
        cin >> ask;
        if (ask == "da")
            continue;
        else
        {
            cout << "\nLa revedere!\n";
            break;
        }
    }
}