// KIProjekt.cpp

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

bool debug = true;

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

}

void duaSimIteration() {

}

void dualerSimplex() {
    vector<double> v = vect;

    if (debug) { printNumbers(v); }
}

void setupSimplex() {
    //Tabellenform
    for (int i = 0; i < spalten; i++)
    {
        vect.push_back(vect[0]);
        vect.erase(vect.begin());
    }

    if (debug) { printNumbers(vect); }

    //Schlupfvariablen hinzufühgen
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

    //Passenden Simplex wählen
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
    string filename = "";

    //Datei öffnen:
    do
    {
        cout << "Geben Sie Namen der Datei ein, die gelesen werden soll.\nDateiname: ";
        cin >> filename;

        input.open(filename + ".txt");
    }
    while (!input.is_open());

    //Datei lesen:
    string line;

    while (input)
    {
        getline(input, line);
        readLine(line);
    }

    input.close();

    setupSimplex();

    return 0;
}
