#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stdexcept>
#include <ctime>
#include <cctype>
#include <sstream>
#include <stdlib.h>
#include <cctype>
#include <chrono>
#include <exception>
#include <regex>

using namespace std; 

// se verifica daca un string contine doar litere
bool containsOnlyLetters(const std::string& str) {
    for (char ch : str) {
        if (!std::isalpha(ch)) {
            return false;
        }
    }
    return true;
}

// se verifica daca data este valida
bool isValidDate(const std::string& dateStr) {
    if (dateStr.length() != 10) {
        return false;  // lungimea datei trebuie sa fie 10 cu tot cu cele 2 "-"
    }

    for (int i = 0; i < 10; ++i) {
        if (i == 4 || i == 7) {
            if (dateStr[i] != '-') {
                return false;  // se verifica daca separatorii sunt "-"
            }
        } else {
            if (!isdigit(dateStr[i])) {
                return false;  // se verifica dacă caracterele sunt cifre
            }
        }
    }
    int year = stoi(dateStr.substr(0, 4));
    int month = stoi(dateStr.substr(5, 2));
    int day = stoi(dateStr.substr(8, 2));
    // obține data curenta
    auto currentTime = std::chrono::system_clock::now();
    time_t currentTime_t = std::chrono::system_clock::to_time_t(currentTime);
    struct tm* localTime = localtime(&currentTime_t);
    int currentYear = localTime->tm_year + 1900;
    int currentMonth = localTime->tm_mon + 1;
    int currentDay = localTime->tm_mday;

    if (year < currentYear || (year == currentYear && month < currentMonth) ||
        (year == currentYear && month == currentMonth && day < currentDay) ||
        month < 1 || month > 12 || day < 1 || day > 31) {
        return false;  // se verifica sa nu fie in trecut valorile
    }
    return true;  
}

// se verifica daca adresa de e-mail este valida
bool isValidEmail(const std::string& email) {
    // expresie regulata pentru validarea adresei de e-mail
    std::regex emailRegex(R"([a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,})");
    return std::regex_match(email, emailRegex);
}

// se verifica daca parola e slaba
bool isWeakPassword(const std::string& password) {
    if (password.length() < 8) {
        return true; // parola este prea scurta
    }

    bool hasUppercase = false;
    bool hasLowercase = false;
    bool hasDigit = false;
    bool hasSpecialChar = false;
    for (char ch : password) {
        if (std::isupper(ch)) {
            hasUppercase = true;
        } else if (std::islower(ch)) {
            hasLowercase = true;
        } else if (std::isdigit(ch)) {
            hasDigit = true;
        } else if (std::ispunct(ch)) {
            hasSpecialChar = true;
        }
    }
    // se verifica daca exista cel putin o cifra, un caracter special, o litera mare si una mica
    if (!hasUppercase || !hasLowercase || !hasDigit || !hasSpecialChar) {
        return true; // parola nu indeplineste toate criteriile
    }
    return false; // parola este considerata suficient de puternica
}

class Calatorie
{
public:
    string Destinatie, Plecare, Data;
    int NrCalatorie, NrZile, Ora, Pret, NrLocuri;
    Calatorie(int nrCalatorie, string plecare, string destiantie, string data, int nrZile, int ora, int pret, int nrLocuri)
    {
        NrCalatorie = nrCalatorie;
        Destinatie = destiantie;
        Plecare = plecare;
        NrZile = nrZile;
        Ora = ora;
        Pret = pret;
        NrLocuri = nrLocuri;
        Data = data;
    }
    ~Calatorie() {}
    void toString() {
        cout << NrCalatorie << " " << Destinatie << " " << Plecare << " " << NrZile << " " << Ora << " " << Pret << " " << NrLocuri << " " << Data <<"\n";
    }
};

class Operator
{
private:
    string Username, Password;

public:
    Operator(string uname, string pass)
    {
        Password = pass;
        Username = uname;
    }
    ~Operator() {}
    string getUsername()
    {
        return this->Username;
    }
    string getPassword()
    {
        return this->Password;
    }

    void adaugaCalatorie(vector<Calatorie*> &calatorii) {
        string Destinatie, Plecare, Data;
        int NrCalatorie, NrZile, Ora, Pret, NrLocuri;
        cout << "Pentru a creea o calatorie, introduceti nr calatorie, orasul plecarii, destinatia, data, nr zile, ora, pretul si nr locuri\n";
        
        cout << "Introduceti nr calatoriei:\n";
        cin >> NrCalatorie;

        cout << "Introduceti orasul plecarii: \n";
        cin >> Plecare;
        if (!containsOnlyLetters(Plecare)) {
            cout << "Numele orasului de plecare trebuie sa contina doar litere\n";
            throw 1;
        }

        cout << "Introduceti orasul destinatie: \n";
        cin >> Destinatie;
        if (!containsOnlyLetters(Destinatie)){
            cout << "Numele destinatiei trebuie sa contina doar litere\n";
            throw 1;
        }

        cout << "Introduceti data: \n";
        cin >> Data;
        if (!isValidDate(Data)) {
            cout << "Formatul datei este invalid\n";
            throw 1;
        }

        cout << "Introduceti nr de zile, ora plecarii, pretul si nr de locuri: \n";
        cin >> NrZile >> Ora >> Pret >> NrLocuri;
        cout << endl;
        Calatorie *ct = new Calatorie(calatorii.size() + 1, Plecare, Destinatie, Data, NrZile, Ora, Pret, NrLocuri);
        calatorii.push_back(ct);
    }
};

class Utilizator
{
private:
    string Mail, Password;
    int NrCalatorie;

public:
    Utilizator(string mail, string pass, int nrCalatorie)
    {
        Mail = mail;
        Password = pass;
        NrCalatorie = nrCalatorie;
    }
    string getMail()
    {
        return this->Mail;
    }
    string getPassword()
    {
        return this->Password;
    }
    int getNrCalatorie()
    {
        return this->NrCalatorie;
    }
    ~Utilizator() {}

    void autentificareUtilizator(vector<Utilizator*> utilizatori) 
    {
        string mail, pass, pass2;
        int nr_calatorie;
        cout << "Autentificare ca si utilizator, introduceti mail si parola:\n";

        cout << "Mail: \n";
        cin >> mail;
        if (!isValidEmail(mail)) {
            cout << "Adresa de e-mail nu este valida\n";
            throw 1;
        }

        cout << "Pass: \n";
        cin >> pass;
        if (isWeakPassword(pass)) {
            cout << "Parola trebuie sa contina cel putin o litera mare, una mica, o cifra si un caracter special\n";
            throw 1;
        }

        cout << "Repetati parola: ";
        cin >> pass2;
        if(pass2 != pass) {
            cout << "Parola repetata a fost introdusa gresit\n";
            throw 1;
        }

        cout << "Nr Calatorie: ";
        cin >> nr_calatorie;
        Utilizator *ut = new Utilizator(mail, pass, nr_calatorie);
        utilizatori.push_back(ut);
        cout << "Autentificare cu succes.\n";
    }

    void rezervare_cursa(vector<Calatorie*> calatorii, vector<Utilizator*> utilizatori)
    { 
        string mail, pass;
        int nr_calatorie;
        cout << "Introduceti mail-ul, parola si nr calatoriei dorite ca sa puteti face rezervarea:\n";
        
        cout << "Mail: \n";
        cin >> mail;
        int aux = 0;
        for (int i = 0; i < utilizatori.size(); i++) {
            if (mail == utilizatori[i]->getMail()) {
                aux++;
                break;
            } 
        }
        if (aux == 0) {
            cout << "Adresa de e-mail nu exista in baza de date\n";
            throw 1;
        }

        cout << "Pass: \n";
        cin >> pass;
        aux = 0;
        for (int i = 0; i < utilizatori.size(); i++) {
            if (pass == utilizatori[i]->getPassword()) {
                aux++;
                break;
            } 
        }
        if (aux == 0) {
            cout << "Parola e gresita\n";
            throw 1;
        }

        cout<< "Nr Calatorie: ";
        cin >> nr_calatorie;
        aux  = 0;
        for (int i = 0; i < calatorii.size(); i++) {
            if (nr_calatorie == calatorii[i]->NrCalatorie) {
                aux++;
                break;
            }
        }
        if (aux == 0) {
            cout << "Calatorie inexistenta\n";
            throw 1;
        }

        Utilizator *util = new Utilizator(mail, pass, nr_calatorie);
        utilizatori.push_back(util);
    }

    void cautare_cursa(vector<Calatorie*> calatorii)
    {
        string destinatie_cautata;
        cout << "Introduceti o calatorie pe care doriti sa o cautati \n" << endl;
        cin >> destinatie_cautata;
        int aux = 0;
        for (int i = 0; i < calatorii.size(); i++)
        {
            if (calatorii[i]->Destinatie == destinatie_cautata)  
            {
                calatorii[i]->toString();
                aux++;
                break;
            } 
        }
        if (aux == 0) {
            cout << "Calatorie inexistenta\n";
            throw 1;
        }           
    }
};