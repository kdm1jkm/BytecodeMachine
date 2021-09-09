#pragma once

#include <string>
#include <fstream>
#include <stack>
#include <vector>

class Machine {
private:
    std::ifstream in;
    std::stack<int32_t> stack;
    std::vector<int32_t> argument;
    std::vector<int32_t> local;

public:
    Machine(const std::string &filename, const std::vector<int32_t> &arguments);

    void runUntilEnd();

    void runNextByte(uint8_t command);

    int32_t getTop();

    template<typename T>
    T readCur();

    ~Machine();
};
