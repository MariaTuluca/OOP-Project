#include "Angajat.h"
int Angajat::id_counter = 0;
int Angajat::counter = 0;

using namespace std;

Angajat::Angajat():ID(++id_counter),nume("John"), prenume("Doe"), CNP("0000000000000"), salariu(0), tip_angajat("necunoscut"){
    data_angajare = {0};
    data_angajare.tm_year = 2015 - 1900; // anul 2015 default
    data_angajare.tm_mon = 9 - 1; // septembrie
    data_angajare.tm_mday = 1; // prima zi a lunii septembrie, anul 2015, ziua înfințării companiei
}

Angajat::Angajat(const string nm, const string prenm, tm data_ang, const string cnp, const string tip_ang):
    nume(nm), prenume(prenm), data_angajare(data_ang), CNP(cnp), tip_angajat(tip_ang){
        counter++;
        ID = id_counter++;
        salariu = calcul_salariu();
}

bool Angajat::validare_CNP(){
    if(CNP.length() != 13)  // verificare lungime CNP
        return false;
       
    for(char c: CNP)    //verificare daca CNP-ul contine doar cifre
        if(!isdigit(c))
            return false;
    // pentru anul nasterii, consider 00-24 ca mileniul 3 si 25-99 ca mileniul 2
    int anul_nasterii = std::stoi(CNP.substr(1, 2));
    if( (anul_nasterii>0 && anul_nasterii<25) && (CNP[0] != '5' && CNP[0] != '6'))
        return false;
    if( (anul_nasterii>24 && anul_nasterii<99) && (CNP[0] != '1' && CNP[0] != '2'))
        return false;

    // validare cu cifra de control
    string constanta_verificare = "279146358279";
    int s = 0;
    for(int i = 0; i < 12; i++)
        s += (CNP[i] - '0') * (constanta_verificare[i] -'0');
    
    int rest = s % 11;
    int cifra_control;
    if(rest == 10)
        cifra_control = 1;
    else if(rest < 10)
        cifra_control = rest;
    
    if(cifra_control != (CNP[12] - '0'))
        return false;

    return true;
}

double Angajat::calcul_salariu() const {
    
    //salariul se calculeaza in functie de tipul angajatului
    time_t timp = time(0); // timpul curent
    struct tm data_curenta = *localtime(&timp); //convertesc in structura de luna si data curenta

    int ani_vechime = data_curenta.tm_year + 1900 - (data_angajare.tm_year + 1900);
    if(data_curenta.tm_mon < data_angajare.tm_mon || (data_curenta.tm_mon == data_angajare.tm_mon && data_curenta.tm_mday < data_angajare.tm_mday))
        ani_vechime--; // inca nu s-au implinit anii de vechime aferenti diferentei dintre anul curenti si anul de angajare

    double salariu = 3500 + 100 * ani_vechime;
    if(tip_angajat == "Manager")
        salariu *= 1.25; // managerii au un bonus de 25%
    if(tip_angajat == "Operator")
        salariu *= 1; // operatorii nu au niciun bonus
    if(tip_angajat == "Asistent")
        salariu *= 0.75;

    //bonusul de 100 lei de ziua de nastere
    if(tip_angajat == "Operator" || tip_angajat == "Asistent"){
        int luna_nasterii = std::stoi(CNP.substr(3, 2));

        if(data_curenta.tm_mon == luna_nasterii)
            salariu += 100; 
    }

    if(tip_angajat == "Operator")
        salariu += 0.005 * this->getValComenzi();
    
    return salariu;
}

ostream& operator<<(ostream& dev, const Angajat& angajat){
    dev<<angajat.tip_angajat<<" cu ID-ul: "<<angajat.ID<<endl;
    dev<<"Nume: "<<angajat.nume<<endl;
    dev<<"Prenume: "<<angajat.prenume<<endl;
    dev<<"Data angajarii: "<<angajat.data_angajare.tm_mday<<"."<<angajat.data_angajare.tm_mon+1<<"."<<angajat.data_angajare.tm_year+1900<<endl;
    dev<<"CNP: "<<angajat.CNP<<endl;
    dev<<"Salariu: "<<angajat.salariu<<" RON."<<endl;
    dev<<endl; 

    return dev;
}

istream& operator>>(istream& dev, Angajat& angajat){
    dev >>angajat.nume>>angajat.prenume>>angajat.CNP;
    dev >>angajat.data_angajare.tm_mday>>angajat.data_angajare.tm_mon>>angajat.data_angajare.tm_year;
    dev >>angajat.tip_angajat>>angajat.salariu;
    angajat.data_angajare.tm_mon--; 
    angajat.data_angajare.tm_year -= 1900;

    return dev;
} 

void Angajat::resetCounter(){
    if(id_counter != 1) id_counter--;
}

