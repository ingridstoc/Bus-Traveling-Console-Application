#include "help.hpp"
#include <bits/stdc++.h>

// clasa pentru criptarea parolelor
class Encryption
{
public:
    set<int>
        prime; // un set o sa contina nr prime, de unde selectam nr prime random p si q
    int public_key;
    int private_key;
    int n;
    void primefiller()
    {
        // metoda folosita ca sa se umple setul de nr prime
        vector<bool> seive(250, true);
        seive[0] = false;
        seive[1] = false;
        for (int i = 2; i < 250; i++)
        {
            for (int j = i * 2; j < 250; j += i)
            {
                seive[j] = false;
            }
        }
        for (int i = 0; i < seive.size(); i++)
        {
            if (seive[i])
                prime.insert(i);
        }
    }
    // se alege un nr prim random si se sterge din lista, deoarece p!=q
    int pickrandomprime()
    {
        int k = rand() % prime.size();
        auto it = prime.begin();
        while (k--)
            it++;
        int ret = *it;
        prime.erase(it);
        return ret;
    }
    void setkeys()
    {
        int prime1 = pickrandomprime(); // primul nr prim
        int prime2 = pickrandomprime(); // al doilea
        n = prime1 * prime2;
        int fi = (prime1 - 1) * (prime2 - 1);
        int e = 2;
        while (1)
        {
            if (__gcd(e, fi) == 1)
                break;
            e++;
        } // d = (k*Φ(n) + 1) / e
        public_key = e;
        int d = 2;
        while (1)
        {
            if ((d * e) % fi == 1)
                break;
            d++;
        }
        private_key = d;
    }
    // criptarea parolei
    long long int encrypt(double message)
    {
        int e = public_key;
        long long int encrpyted_text = 1;
        while (e--)
        {
            encrpyted_text *= message;
            encrpyted_text %= n;
        }
        return encrpyted_text;
    }
    // decriptarea parolei
    long long int decrypt(int encrpyted_text)
    {
        int d = private_key;
        long long int decrypted = 1;
        while (d--)
        {
            decrypted *= encrpyted_text;
            decrypted %= n;
        }
        return decrypted;
    }
    // se converteste fiecare litera la forma ASCII
    vector<int> encoder(string message)
    {
        vector<int> form;
        for (auto &letter : message)
            form.push_back(encrypt((int)letter));
        return form;
    }
    string decoder(vector<int> encoded)
    {
        string s;
        for (auto &num : encoded)
            s += decrypt(num);
        return s;
    }
};

class Database
{
public:
    vector<Calatorie *> calatorii;
    vector<Utilizator *> utilizatori;
    vector<Operator *> operatori;

    // clasa cu toti vectorii
    Database()
    {
        this->calatorii = citeste_toate_calatoriile();
        this->utilizatori = citeste_toti_utilizatorii();
        this->operatori = citeste_toti_operatorii();
    }

    Operator *loginOperator()
    {
        string username, pass;
        cout << "Logati-va ca si operator, introduceti username si parola:\n";
        cout << "Username: \n";
        cin >> username;
        cout << "Parola: \n";
        cin >> pass;
        for (int i = 0; i < operatori.size(); i++)
        {
            int aux = 0;
            if (operatori[i]->getUsername().compare(username) == 0)
            {
                aux++;
            }
            if (operatori[i]->getPassword().compare(pass) == 0)
            {
                aux++;
            }
            if (aux == 2)
            {
                cout << "Login reusit\n";
                return operatori[i];
            }
        }
        cout << "NU v-ati logat cu succes.\n";
        throw 1;
        return NULL;
    }

    Utilizator *loginUtilizator()
    {
        string mail, pass;
        int nr_calatorie;
        cout << "Logati-va ca si user, introduceti mail, parola si nr_calatoriei:\n";
        cout << "Mail: \n";
        cin >> mail;
        cout << "Parola: \n";
        cin >> pass;
        cout << "Nr Calatorie: \n";
        cin >> nr_calatorie;
        for (int i = 0; i < utilizatori.size(); i++)
        {
            int aux = 0;
            if (utilizatori[i]->getMail().compare(mail) == 0)
            {
                aux++;
            }
            if (utilizatori[i]->getPassword().compare(pass) == 0)
            {
                aux++;
            }
            if (utilizatori[i]->getNrCalatorie() == nr_calatorie)
            {
                aux++;
            }
            if (aux == 3)
            {
                cout << "Login reusit\n\n";
                return utilizatori[i];
            }
        }
        cout << "NU v-ati logat cu succes.\n";
        throw 1;
        return NULL;
    }

    Utilizator* autentificareUtilizator() 
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
        return ut;
    }


    void delete_calatorie()
    {
        cout << "Introduceti nr calatoriei pe care doriti sa o stergeti " << endl;
        int nr_calatorie;
        cin >> nr_calatorie;
        for (int i = 0; i < calatorii.size(); i++)
        {
            if (calatorii[i]->NrCalatorie == nr_calatorie)
            {
                calatorii.erase(calatorii.begin() + i);
                break;
            }
        }
    }

    // citirea din CSV si introducerea in vectorii corespunzatori
private:
    vector<Operator *> citeste_toti_operatorii()
    {
        ifstream file("Operatori.csv");
        if (!file.is_open())
        {
            cerr << "Error opening the file." << endl;
            exit(1);
        }
        // vector in care se introduc randurile din csv
        string line;
        vector<Operator *> operatori;
        while (getline(file, line))
        {
            // Tokenize pe baza virgulelor
            istringstream ss(line);
            vector<string> row;
            string token;
            while (getline(ss, token, ','))
            {
                row.push_back(token); // push la randul din csv intr-un vector
            }
            Operator *op = new Operator(row[1], row[2]);
            operatori.push_back(op); // se pune obiectul in vector
        }
        file.close();
        return operatori;
    }

    vector<Utilizator *> citeste_toti_utilizatorii()
    {
        ifstream file("Utilizatori.csv");
        if (!file.is_open())
        {
            cerr << "Error opening the file." << endl;
            exit(1);
        }
        string line;
        vector<Utilizator *> utilizatori;
        while (getline(file, line))
        {
            istringstream ss(line);
            vector<string> row;
            string token;
            while (getline(ss, token, ','))
            {
                row.push_back(token);
            }
            int userNumber1 = stoi(row[2]);
            Utilizator *ut = new Utilizator(row[0], row[1], userNumber1);
            utilizatori.push_back(ut);
        }
        file.close();
        return utilizatori;
    }

    vector<Calatorie *> citeste_toate_calatoriile()
    {
        ifstream file("Calatorii.csv");
        if (!file.is_open())
        {
            cerr << "Error opening the file." << endl;
            exit(1);
        }
        string line;
        vector<Calatorie *> calatorii;
        while (getline(file, line))
        {
            istringstream ss(line);
            vector<string> row;
            string token;
            while (getline(ss, token, ','))
            {
                row.push_back(token);
            }
            int travel_number1 = stoi(row[0]);
            int travel_number2 = stoi(row[4]);
            int travel_number3 = stoi(row[5]);
            int travel_number4 = stoi(row[6]);
            int travel_number5 = stoi(row[7]);
            Calatorie *cl = new Calatorie(travel_number1, row[1], row[2], row[3], travel_number2, travel_number3, travel_number4, travel_number5);
            calatorii.push_back(cl);
        }
        file.close();
        return calatorii;
    }
};

int main()
{
    Database *db = new Database();
    // type e folosit ca sa vada daca e operator, utilizator sau daca doreste sa iasa din aplicatie
    // value e folosit ca sa selecteze comanda dorita din aplicatie 
    int type, value;
    cout << "Apasati 1 pentru utilizator.\n";
    cout << "Apasati 2 pentru operator.\n";
    cout << "Apasati 3 pentru a iesi din aplicatie.\n";
    cin >> type;
    Utilizator *ut = NULL;
    Operator *op = NULL;
    while (1)
    {
        if (type == 1) { // utilizator
            if (ut == NULL) {
                cout << "Apasati 3 pentru autentificare.\n";
                cout << "Apasati 4 pentru login.\n";
                cin >> value;

                // autentificare
                if (value == 3) {
                    while (1) {
                        try { // metoda de autentificare se afla in help.hpp
                            ut = db->autentificareUtilizator();
                            break;
                        }
                        catch (int eroare) {}
                    }
                }
                // logare
                if (value == 4) {
                    while (1) {
                        try {
                            ut = db->loginUtilizator();
                            break;
                        }
                        catch (int eroare) {}
                    }

                    // criptare parola
                    Encryption encryptedPass;
                    encryptedPass.primefiller();
                    encryptedPass.setkeys();
                    cout << "Parola criptata:\n";
                    vector<int> coded = encryptedPass.encoder(ut->getPassword());
                    cout << "Parola necriptata:\n"
                         << ut->getPassword();
                    cout << "\nParola criptata cu ajutorul cheii publice:\n";
                    for (auto &p : coded)
                        cout << p;
                    cout << "\nParola decriptata cu ajutorul cheii private:\n";
                    cout << encryptedPass.decoder(coded) << endl;
                }
            }

            // daca utilizatorul s-a logat, inseamna ca exista si poate sa caute si sa rezerve curse
            if (ut != NULL) {
                // cautare curse
                cout << "Daca doriti sa cautati o calatorie, apasati 7: \n";
                cout << "Daca doriti sa rezervati o calatorie, apasati 8: \n";
                cout << "Daca doriti sa iesiti din aplicatie, apasati 3:\n";
                cout << "Daca doriti sa deveniti operator, apasati 2\n";
                cin >> value;
                if (value == 3) {
                    type = 3;
                }
                if (value == 2) {
                    type = 2;
                    ut = NULL;
                }
                if (value == 7) {
                    while (1) {
                        try {
                            ut->cautare_cursa(db->calatorii);
                            break;
                        }
                        catch (int eroare) {}
                    }
                }

                // rezervare curse
                if (value == 8) {
                    while (1) {
                        try {
                            ut->rezervare_cursa(db->calatorii, db->utilizatori);
                            break;
                        }
                        catch (int eroare) {}
                    }
                }
            }
        }

        if (type == 2) { // operator
            if (op == NULL) {
                // logare
                while (1) {
                    try {
                        op = db->loginOperator();
                        break;
                    }
                    catch (int eroare) {}
                }

                // criptare parola
                Encryption encryptedPass;
                encryptedPass.primefiller();
                encryptedPass.setkeys();
                cout << "Parola criptata:\n";
                vector<int> coded = encryptedPass.encoder(op->getPassword());
                cout << "Parola necriptata:\n"
                     << op->getPassword();
                cout << "\nParola criptata cu ajutorul cheii publice:\n";
                for (auto &p : coded)
                    cout << p;
                cout << "\nParola decriptata cu ajutorul cheii private:\n";
                cout << encryptedPass.decoder(coded) << endl;
            }

            if (op != NULL) {
                // adaugare calatorie
                cout << "Daca doriti sa adaugati o calatorie, apasati 5: \n";
                cout << "Daca doriti sa stergeti o calatorie, apasati 6: \n";
                cout << "Daca doriti sa iesiti din aplicatie, apasati 3:\n";
                cout << "Daca doriti sa deveniti utilizator, apasati 1\n";
                cin >> value;
                if (value == 3) {
                    type = 3;
                }
                if (value == 1) {
                    type = 1;
                    op = NULL;
                }
                if (value == 5) {
                    while (1) {
                        try {
                            cout << "Calatorii existente:\n";
                            for (int i =0; i < db->calatorii.size(); i++)
                            {
                                cout << db->calatorii.at(i)->Destinatie << "\n";
                            }
                            cout << "\n";
                            op->adaugaCalatorie(db->calatorii);
                            cout << "Calatorii dupa adaugare:\n";
                            for (int i =0; i < db->calatorii.size(); i++)
                            {
                                cout << db->calatorii.at(i)->Destinatie << "\n";
                            }
                            break;
                        }
                        catch (int eroare) {}
                    }
                }

                // stergere calatorie
                if (value == 6) {
                    db->delete_calatorie();
                }
            }
        }

        // iesire din aplicatie
        if (type == 3) {
            cout << "Va multumim pentru folosirea aplicatiei noastre de calatorii cu autocarul!";
            break;
        }
    }

    // adaugarea in CSV a vectorului de utilizatori
    ofstream file1;
    file1.open("Utilizatori.csv");
    for (int i = 0; i < db->utilizatori.size(); i++) {
        file1 << db->utilizatori.at(i)->getMail() << "," << db->utilizatori.at(i)->getPassword() << "," << db->utilizatori.at(i)->getNrCalatorie() << "\n";
    }
    file1.close();

    // adaugarea in CSV a vectorului de calatorii
    ofstream file2;
    file2.open("Calatorii.csv");
    for (int i = 0; i < db->calatorii.size(); i++) {
        file2 << db->calatorii.at(i)->NrCalatorie << "," << db->calatorii.at(i)->Plecare << "," << db->calatorii.at(i)->Destinatie << ",";
        file2 << db->calatorii.at(i)->Data << "," << db->calatorii.at(i)->NrZile << "," << db->calatorii.at(i)->Ora << ",";
        file2 << db->calatorii.at(i)->Pret << "," << db->calatorii.at(i)->NrLocuri << "\n";
    }
    file2.close();
    
    return 0;
}