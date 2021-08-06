#include <iostream>
#include <string>
#include <vector>
#include <cstring>
#include <set>
#include "Machine.h"

void printHelp(char *filename)
{
    std::cout << "Usage1: " << filename << " run <file> [param...]" << std::endl;
    std::cout << "Usage2: " << filename << " compile <file> -o [output]" << std::endl;
}

int main(int argc, char **argv)
{
    if (argc == 1)
    {
        std::string input = "";
        const std::set<std::string> choices = {"1", "2"};
        while (choices.find(input) == choices.end())
        {
            std::cout << "1. run" << std::endl;
            std::cout << "2. compile" << std::endl;
            std::cin >> input;
        }
    }

    if (strcmp(argv[1], "help") == 0)
    {
        printHelp(argv[0]);
    }
    else if (strcmp(argv[1], "run") == 0)
    {
        std::vector<int32_t> param;
        for (int i = 3; i < argc; i++)
        {
            param.push_back(std::stoi(argv[i]));
        }
        Machine machine(argv[2], param);
        machine.runUntilEnd();
    }

    return EXIT_SUCCESS;
}
