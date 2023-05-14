// KIProjekt.cpp

#include <iostream>
#include <fstream>
#include <string>
using namespace std;

void readLine(string line) {
    if (line != "" && line.substr(0,2) != "//") {
        cout << "'" << line << "'" << endl;
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

    return 0;
}
