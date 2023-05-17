// KIProjekt.cpp

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

vector<int> vect = {};
int numberOfVariables = 0;

void parseLine(string line, int i)
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
            vect.push_back(temp);
            temp = 0;
            num = false;
            inVarName = true;
        }

        i++;
    }
}

void readLine(string line) {
    if (line != "" && line.substr(0,2) != "//") {
        int minOrMax = 0;

        if (vect.empty())
        {
            if (line.find(":"))
            {
                if (!line.find("min"))
                {
                    minOrMax = 0;
                }
                else if (!line.find("max"))
                {
                    minOrMax = 1;
                }
            }

            vect.push_back(minOrMax);
            parseLine(line, line.find(":") + 1);
            numberOfVariables = vect.size() - 1;
        }
        else
        {
            parseLine(line, 0);
        }
    }
}

void printNumbers()
{
    int i = 1;

    if (vect[0] == 0) { cout << "min:  "; } else { cout << "max:  "; }

    for (int j = 0; j < vect.size() / (numberOfVariables + 1); j++) {
        while (i <= numberOfVariables)
        {
            cout << vect[j * (numberOfVariables + 1) + i] << "  ";
            i++;
        }

        cout << endl;
        i = 0;
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

    printNumbers();

    return 0;
}
