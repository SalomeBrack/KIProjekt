// KIProjekt.cpp

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

vector<char> vect = {};

void parseLine(string line) {
    int i = 0;

    while (line[i] != ';' && line.length() > i) {
        vect.push_back(line[i]);
        i++;
    }
}

void readLine(string line) {
    if (line != "" && line.substr(0,2) != "//") {
        bool min = true;

        if (vect.empty()) {
            if (line.find(":")) {
                if (line.find("min:")) {
                    min = true;
                }
                else if (line.find("max:")) {
                    min = false;
                }
                else {
                    //Fehler: min / max fehlt
                }
            }
            else {
                //Fehler: Erste Zeile legt kein Ziel fest
            }
            parseLine(line.substr(line.find(":") + 1, line.length()));

            //Variablen zählen
        }
        else {
            parseLine(line);
        }
    }
}

int main()
{
    ifstream input;
    string filename = "";

    //Datei öffnen:
    do {
        cout << "Geben Sie Namen der Datei ein, die gelesen werden soll.\nDateiname: ";
        cin >> filename;

        input.open(filename + ".txt");
    }
    while (!input.is_open());

    //Datei lesen:
    string line;

    while (input) {
        getline(input, line);
        readLine(line);
    }

    input.close();

    std::cout << "Vector = { ";
    for (char n : vect)
        std::cout << n << ", ";
    std::cout << "}; \n";

    return 0;
}
