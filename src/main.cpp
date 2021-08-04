#include <iostream>
#include <string>
#include "Machine.h"

int main(int, char **)
{
    Machine machine("sample.txt");

    machine.runNextByte();
    machine.runNextByte();
    machine.runNextByte();
}
