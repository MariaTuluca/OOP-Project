#pragma once
#include "Comanda.h"
#include <vector>

class Operator {
private: 
    int id;
    vector<Comanda>* comenzi;

public:
    Operator(const int id, vector<Comanda>* com):id(id){
        comenzi = com;
    };

    int getID() const {
        return id;
    }

    vector<Comanda>* getComenzi() const {
        return comenzi;
    }

    bool operator<(const Operator& other) const {
        return id < other.id;
    }

    void adauga_comanda(const Comanda &com) {
        comenzi->push_back(com);
    }

    friend bool operator==(const Operator& op1, const Operator& op2){
        return (op1.id == op2.id);
    }

    friend bool operator!=(const Operator& op1, const Operator& op2){
        return (op1.id != op2.id);
    }
};