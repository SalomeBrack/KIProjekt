// KIProjekt.cpp

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

bool debug = false;

vector<double> vect = {};
int spalten = 0;
int zeilen = 0;

void printNumbers(vector<double> v)
{
    cout << endl;

    for (int j = 0; j < v.size() / spalten; j++) {
        int i = 0;

        while (i < spalten)
        {
            cout << v[j * spalten + i] << "  ";
            i++;
        }

        cout << endl;
    }
}

void primalerSimplex() {
    //Todo
}

void dualerSimplex() {
    vector<double> v = vect;
    int i = 0;
    double temp1 = 0;
    double temp2 = 0;
    double temp3 = 0;
    int pivotZeile = 0;
    int pivotSpalte = 0;
    int pivotElement = 0;
    bool rechtsNegativ = false;
    
    do {
        //Pivotzeile: Kleinsten Wert finden
        i = 1;
        pivotZeile = 0;
        temp1 = 0;
        temp2 = v[spalten - 1];

        while (i < zeilen - 1) {
            temp1 = v[i * spalten + spalten - 1];

            if (temp1 < temp2) {
                pivotZeile = i;
                temp2 = temp1;
            }

            i++;
        }

        //Pivotspalte: Gr��ten Wert, kleiner 0 finden
        i = 0;
        pivotSpalte = 0;
        temp1 = 0;
        temp2 = 0;

        //...nach einer negativen Zahl suchen
        while (i < spalten - 1 && temp1 >= 0) {
            temp2 = v[pivotZeile * spalten + i];

            if (temp2 != 0) {
                temp1 = v[(zeilen - 1) * spalten + i] / temp2;
            }

            pivotSpalte = i;
            i++;
        }

        //...nach einer gr��eren negativen Zahl suchen
        while (i < spalten - 1) {
            temp3 = v[pivotZeile * spalten + i];

            if (temp3 != 0) {
                temp2 = v[(zeilen - 1) * spalten + i] / temp3;
            }

            if (temp2 < 0 && temp2 > temp1) {
                temp1 = temp2;
                pivotSpalte = i;
            }

            i++;
        }

        //Pivotelement: neue Tabelle
        pivotElement = pivotZeile * spalten + pivotSpalte;

        //...Pivotzeile
        temp1 = v[pivotElement];

        for (int j = 0; j < spalten; j++)
        {
            temp2 = pivotZeile * spalten + j;

            v[temp2] = v[temp2] / temp1;
        }

        //...andere Zeilen
        i = 0;
        
        while (i < zeilen) {
            if (i != pivotZeile) {
                temp3 = v[i * spalten + pivotSpalte];

                for (int j = 0; j < spalten; j++)
                {
                    temp1 = pivotZeile * spalten + j;
                    temp2 = i * spalten + j;

                    v[temp2] = v[temp2] - (v[temp1] * temp3);
                }
            }

            i++;
        }

        //Rechte Seite auf negative Werte pr�fen
        rechtsNegativ = false;
        for (int i = 0; i < zeilen - 1; i++)
        {
            if (v[i * spalten + spalten - 1] < 0) {
                rechtsNegativ = true;
            }
        }

        if (debug) { printNumbers(v); }
    } while (rechtsNegativ);

    //Ergebnis ausgeben
    bool schlupfNull = true;

    cout << endl;

    cout << "Z: " << v[v.size() - 1] * (-1) << endl;

    for (int i = 0; i < spalten - zeilen; i++)
    {
        temp1 = 0;

        for (int j = 0; j < zeilen - 1; j++)
        {
            if (v[j * spalten + i] == 1) {
                temp1 = v[j * spalten + spalten - 1];
            }
        }

        cout << "x" << i << ": " << temp1 << endl;
    }
}

void setupSimplex() {
    //Tabellenform
    for (int i = 0; i < spalten; i++)
    {
        vect.push_back(vect[0]);
        vect.erase(vect.begin());
    }

    //Schlupfvariablen hinzuf�hgen
    zeilen = vect.size() / spalten;

    int z = zeilen - 1;
    int temp = 0;

    vect.insert(vect.begin() + z * spalten + spalten - 1, zeilen - 1, temp);
    z--;

    while (z >= 0) {
        for (int i = zeilen - 2; i >= 0; i--)
        {
            if (i == z) {
                temp = 1;
            }
            else {
                temp = 0;
            }

            vect.insert(vect.begin() + z * spalten + spalten - 1, temp);
        }

        z--;
    }

    spalten = vect.size() / zeilen;

    if (debug) { printNumbers(vect); }

    //Passenden Simplex w�hlen
    bool rechtsNegativ = false;

    for (int i = 0; i < zeilen - 1; i++)
    {
        if (vect[i * spalten + spalten - 1] < 0) {
            rechtsNegativ = true;
        }
    }

    if (rechtsNegativ) {
        dualerSimplex();
    }
    else {
        primalerSimplex();
    }
}

void parseLine(string line, int i, bool invert)
{
    bool num = false;
    bool inVarName = false;
    bool negative = false;
    int val = 0;
    int temp = 0;

    while (i < line.length())
    {
        val = line[i] - 48;
        
        if (val < 10 && val >= 0 && !inVarName)
        {
            if (!num)
            {
                num = true;
                temp = 0;
            }
            temp = temp * 10 + val;

            if (negative)
            {
                temp = temp * (-1);
                negative = false;
            }
        }
        else if (line[i] == '+' || line[i] == '<' || line[i] == '>' || line[i] == '=')
        {
            inVarName = false;
            negative = false;
        }
        else if (line[i] == '-')
        {
            inVarName = false;
            negative = true;
        }
        else if (num)
        {
            if (invert) {
                temp = temp * (-1);
            }

            vect.push_back(temp);
            temp = 0;
            num = false;
            inVarName = true;
        }

        i++;
    }
}

void readLine(string line) {
    bool invert = false;

    if (line != "" && line.substr(0,2) != "//") {
        if (vect.empty() && line.find(":"))
        {
            if (!line.find("min"))
            {
                invert = false;
            }
            else if (!line.find("max"))
            {
                invert = true;
            }

            parseLine(line, line.find(":") + 1, invert);
            vect.push_back(0);
            spalten = vect.size();
        }
        else
        {
            if (line.find(">"))
            {
                invert = true;
            }
            else
            {
                invert = false;
            }

            parseLine(line, 0, invert);
        }
    }
}

int main()
{
    ifstream input;
    string filepath = "";

    while (true) {
        //Datei �ffnen:
        do
        {
            cout << "Geben Sie Dateipfad ein.\nDateipfad: ";
            cin >> filepath;

            input.open(filepath);
        } while (!input.is_open());

        //Datei lesen:
        string line;

        while (input)
        {
            getline(input, line);
            readLine(line);
        }

        input.close();

        setupSimplex();

        //Reset
        cout << endl;
        vect.clear();
        spalten = 0;
        zeilen = 0;
    }

    return 0;
}
