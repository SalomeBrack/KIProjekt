// KIProjekt.cpp

#include <iostream>
#include <fstream>
#include <string>
using namespace std;

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
    while (input) {
        string varName;

        getline(input, varName, '+');

        int varValue;
        input >> varValue;

        input >> ws;

        if (!input) {
            break;
        }

        cout << "'" << varName << "': " << varValue << endl;
    }

    input.close();

    return 0;
}
