#include <string>
#include <fstream>
#include <stack>

using namespace std;

class Machine
{
private:
    ifstream in;
    stack<int32_t> stack;

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
        POP = 0x21
    };

public:
    Machine(const string &filename) : in(filename, ios::binary)
    {
    }

    void runNextByte()
    {
        unsigned char command;
        in.read((char *)&command, 1);

        switch (command)
        {
        case Code::PUSH:
        {
            int32_t value;
            in.read((char *)&value, 4);

            stack.push(value);
            break;
        }
        }
    }
};
