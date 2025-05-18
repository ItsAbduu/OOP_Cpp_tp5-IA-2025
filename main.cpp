#include <exception>
#include <iostream>
#include <string>
using namespace std;

class OperationException: public exception {
    private:
        string msg;
    public:
        OperationException(string const& s="") throw() : msg(s) {}


        virtual const char* what() const throw() {
            return msg.c_str();
        }

        ~OperationException() throw() {}

};

class Personne {
private:
    string nom, prenom, cine;
    string date;
public:
    Personne() {}
    void identifie() {}
    void affiche() {}
};

class Compte {
protected:
    Personne p;
    int numero;
    float soldeinit;
public:
    Compte(int n, float s) {
        numero = n;
        soldeinit = s;
    }

    virtual float calculer_solde() {
        return soldeinit;
    }

    int getNumero() {
        return numero;
    }
};

class Compte_depot : public Compte {
private:
    float* operations;
    int max_operations;
    int current_op;

public:
    Compte_depot(int n, float s, int max_ops) : Compte(n, s) {
        max_operations = max_ops;
        current_op = 0;
        operations = new float[max_operations];
    }

    ~Compte_depot() {
        delete[] operations;
    }

    virtual float calculer_solde() override {
        float total = soldeinit;
        for (int i = 0; i < current_op; i++) {
            total += operations[i];
        }
        return total;
    }

    void operator>(float montant) {

        /* if (current_op < max_operations) {
            operations[current_op++] = montant;
        } else {
            cout << "Erreur: Max d'opérations." << endl;
        }
        */

        if(current_op >= max_operations) {
            OperationException err("Erreur: Max d'opérations. Exception");
            throw err;

        }
        operations[current_op++] = montant;
    }

    float operator[](int idx) {
        /* if (idx >= 0 && idx < current_op) {
            return operations[idx];
        } else {
            cout << "Erreur: invalide idx." << endl;
            return 0.0f;
            } */

        if(idx < 0 || idx >= current_op) {
            OperationException err_idx("Erreur: invalide idx. Exception");
            throw err_idx;

        }
        return operations[idx];
    }
};

class Compte_Epargne : public Compte {
private:
    float taux;
    int duree;

public:
    Compte_Epargne(int n, float s, float t, int d) : Compte(n, s) {
        if(d <= 0 || t <= 0) {
            OperationException err_duree("Taux et Duree doivent etre positives!!!");
            throw err_duree;
        }
        taux = t;
        duree = d;
    }

    virtual float calculer_solde() override {
        float total = soldeinit;
        for (int i = 0; i < duree; i++) {
            total *= (1 + taux);
        }
        return total;
    }
};

void editer_solde(Compte &compte) {
    cout << "Compte " <<  "\033[31;4m" << compte.getNumero() << "\033[0m"
    << " -> Solde: " << "\033[34;1m"
    << compte.calculer_solde() << "\033[0m" << endl;
}

float calculer_solde_moyen(Compte* comptes[], int n) {
    if (n <= 0) return 0;

    float total = 0;
    for (int i = 0; i < n; i++) {
        total += comptes[i]->calculer_solde();
    }
    return total / n;
}

int main() {

    cout << "\n==== Debut du programme ====" << endl;
    Compte_depot cd1(101, 1000, 5);
    Compte_depot cd2(102, 2000, 3);
    Compte_Epargne ce1(201, 5000, 0.05, 3); // 5% pour 3ans
    Compte_Epargne ce2(202, 10000, 0.03, 5); // 3% pour 5ans


    try {
        Compte_Epargne ce3(201, 5000, 0.05, -3);
    } catch (OperationException &err_d) {
        cerr << "\033[31m" << err_d.what() << "\033[0m" << endl;
    }

    try {
        cd1 > 500;
        cd1 > -200;
        cd1 > 300;
        cd1 > 700;
        cd1 > 800;
        cd1 > 100;
    } catch (OperationException &e_idx) {
        cerr << "\033[31m" << e_idx.what() << "\033[0m" << endl;
    }

    cd2 > 1000;
    cd2 > -500;

    cout << "\033[33m" << "---- Tous les comptes ----" << "\033[0m" << endl;
    Compte* comptes[4];
    comptes[0] = &cd1;
    comptes[1] = &cd2;
    comptes[2] = &ce1;
    comptes[3] = &ce2;

    editer_solde(cd1);
    editer_solde(cd2);
    editer_solde(ce1);
    editer_solde(ce2);

    try {
        cout << cd1[9] << endl;
    } catch (OperationException &e) {
        cerr << "\033[31m" << e.what() << "\033[0m" << endl;
    }

    cout <<  "\033[33m";
    cout << "\n---- Solde moyen ----" << "\033[0m" << endl;
    cout << "Solde moyen: " << "\033[1;32m"
    << calculer_solde_moyen(comptes, 4) << endl;
    cout << "\033[0m";

    cout << "==== Fin du programme ==== \n" << endl;

    return 0;
}
