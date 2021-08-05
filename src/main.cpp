#include <iostream>
#include <string>
#include <vector>
#include "Machine.h"

int main(int, char **)
{
    vector<int32_t> param;
    param.push_back(2);
    param.push_back(4);
    Machine machine("sample", param);

    machine.runUntilEnd();
}
