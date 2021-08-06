#pragma once

#include <string>
#include <fstream>
#include <stack>
#include <vector>

class Machine
{
private:
    std::ifstream in;
    std::stack<int32_t> stack;
    std::vector<int32_t> argument;
    std::vector<int32_t> local;

    enum Code
    {
        ADD = 0x00,
        SUBTRACT,
        NEGATIVE,
        EQUAL,
        GREAT_THAN,
        LESS_THAN,
        AND,
        OR,
        JUMP = 0x10,
        IF_JUMP,
        PUSH = 0x20,
        POP,
        CONSTANT = 0x30,
        ARGUMENT,
        LOCAL,
        IN,
        OUT
    };

public:
    Machine(const std::string &filename, const std::vector<int32_t> &arguments);

    void runUntilEnd();

    void runNextByte();

    int32_t getTop();

    template <typename T>
    T readCur();
};
