#include <iostream>
#include <string>
#include <vector>
#include <cstring>
#include <set>
#include <functional>
#include "Machine.h"

using namespace std;

void printHelp(char *filename)
{
    cout << "Usage1: " << filename << " run <file> [param...]" << endl;
    cout << "Usage2: " << filename << " compile <file> -o [output]" << endl;
}

string getInput(const set<string> &choices, const function<void()> &runBeforeAsk)
{
    string input;
    while (true)
    {
        runBeforeAsk();
        getline(cin, input);
        if (choices.find(input) != choices.end())
            break;
    }

    return input;
}

int main(int argc, char **argv)
{
    if (argc == 1)
    {
        string choice = getInput({"1", "2"}, []
                                 { cout << "1. run" << endl
                                        << "2. compile" << endl
                                        << "Enter>>"; });

        string filename;
        cout << "Enter file>>";
        cin >> filename;

        switch (stoi(choice))
        {
        case 1:
        {
            vector<int32_t> param;
            string input;

            cin.ignore();
            while (true)
            {
                cout << "Enter param[" << param.size() << "]>>";
                getline(cin, input);
                if (input == "")
                    break;

                param.push_back(stoi(input));
            }

            Machine machine(filename, param);
            machine.runUntilEnd();
            return EXIT_SUCCESS;
        }
        case 2:
        {
            cout << "Not supported" << endl;
            return EXIT_SUCCESS;
        }
        }
    }

    if (strcmp(argv[1], "help") == 0)
    {
        printHelp(argv[0]);
    }
    else if (strcmp(argv[1], "run") == 0)
    {
        vector<int32_t> param;
        for (int i = 3; i < argc; i++)
        {
            param.push_back(stoi(argv[i]));
        }
        Machine machine(argv[2], param);
        machine.runUntilEnd();
    }
    else if (strcmp(argv[1], "compile") == 0)
    {
        cout << "Not supported" << endl;
    }
    else
    {
        printHelp(argv[0]);
    }

    return EXIT_SUCCESS;
}
