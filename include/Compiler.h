#pragma once

#include <functional>
#include <string>
#include <fstream>
#include <array>

class Compiler
{
private:
    const std::string output;
    std::ifstream in;

    void printError(std::function<void(int, int)>);

public:
    Compiler(const std::string &filename);
    Compiler(const std::string &filename, const std::string output);

    void compile();
};