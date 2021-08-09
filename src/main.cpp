#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <functional>
#include <filesystem>

#include "Machine.h"
#include "Compiler.h"

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
        while (true)
        {
            cout << "Enter file>>";
            cin >> filename;
            if (std::filesystem::exists(filename))
                break;
        }

        switch (stoi(choice))
        {
        case 1:
        {
            vector<int32_t> param;
            string input;

            cin.ignore();
            cout << "Enter nothing to stop entering parameter." << endl;
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
            Compiler compiler(filename);
            compiler.compile();
            return EXIT_SUCCESS;
        }
        }
    }

    if ("help"s == argv[1])
    {
        printHelp(argv[0]);
    }
    else if ("run"s == argv[1])
    {
        vector<int32_t> param;
        for (int i = 3; i < argc; i++)
        {
            param.push_back(stoi(argv[i]));
        }
        Machine machine(argv[2], param);
        machine.runUntilEnd();
    }
    else if ("compile"s == argv[1])
    {
        Compiler compiler(argv[2]);
        compiler.compile();
    }
    else
    {
        printHelp(argv[0]);
    }

    return EXIT_SUCCESS;
}
