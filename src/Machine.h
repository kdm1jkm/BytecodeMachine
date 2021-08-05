#include <string>
#include <fstream>
#include <stack>
#include <vector>

using namespace std;

class Machine
{
private:
    ifstream in;
    stack<int32_t> stack;
    vector<int32_t> argument;
    vector<int32_t> local;

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
    Machine(const string &filename, const vector<int32_t> &arguments) : in(filename, ios::binary)
    {
        uint8_t localLength = readCur<uint8_t>();
        uint8_t argumentLength = readCur<uint8_t>();

        for (int i = 0; i < argumentLength; i++)
        {
            if (arguments.size() > i)
            {
                argument.push_back(arguments[i]);
            }
            else
            {
                argument.push_back(0);
            }
        }

        for (int i = 0; i < localLength; i++)
        {
            local.push_back(0);
        }
    }

    void runUntilEnd()
    {
        while (!in.eof())
        {
            runNextByte();
        }
    }

    void runNextByte()
    {
        int pos = in.tellg();

        uint8_t command = readCur<uint8_t>();

        switch (command)
        {
        case Code::ADD:
        {
            int32_t value2 = getTop();
            int32_t value1 = getTop();

            stack.push(value1 + value2);
            break;
        }
        case Code::SUBTRACT:
        {
            int32_t value2 = getTop();
            int32_t value1 = getTop();

            stack.push(value1 - value2);
            break;
        }
        case Code::NEGATIVE:
        {
            uint32_t value = getTop();

            stack.push(((2L << 32) - 1) - value);
            break;
        }
        case Code::EQUAL:
        {
            int32_t value2 = getTop();
            int32_t value1 = getTop();

            stack.push((value1 == value2) ? 0xFFFFFFFF : 0x00000000);
            break;
        }
        case Code::GREAT_THAN:
        {
            int32_t value2 = getTop();
            int32_t value1 = getTop();

            stack.push(value1 > value2);
            break;
        }
        case Code::LESS_THAN:
        {
            int32_t value2 = getTop();
            int32_t value1 = getTop();

            stack.push(value1 < value2);
            break;
        }
        case Code::AND:
        {
            int32_t value2 = getTop();
            int32_t value1 = getTop();

            stack.push(value1 & value2);
            break;
        }
        case Code::OR:
        {
            int32_t value2 = getTop();
            int32_t value1 = getTop();

            stack.push(value1 | value2);
            break;
        }
        case Code::IF_JUMP:
        {
            uint32_t value = getTop();
            if (value == 0x00000000)
            {
                readCur<uint32_t>();
                break;
            }
        }
        case Code::JUMP:
        {
            uint32_t address = readCur<uint32_t>();
            in.seekg(address);
            break;
        }
        case Code::PUSH:
        {
            uint8_t segment = readCur<uint8_t>();
            int32_t value;
            switch (segment)
            {
            case Code::CONSTANT:
            {
                value = readCur<int32_t>();
                break;
            }
            case Code::ARGUMENT:
            {
                uint8_t index = readCur<uint8_t>();
                value = argument.at(index);
                break;
            }
            case Code::LOCAL:
            {
                uint8_t index = readCur<uint8_t>();
                value = local.at(index);
                break;
            }
            case Code::IN:
            {
                uint8_t index = readCur<uint8_t>();
                switch (index)
                {
                // 정수
                case 0:
                {
                    cin >> value;
                    break;
                }
                // 문자
                case 1:
                {
                    char input;
                    cin >> input;
                    value = input;
                    break;
                }
                // hex
                case 2:
                {
                    cin >> hex >> value;
                    break;
                }
                }
                break;
            }
            }

            stack.push(value);
            break;
        }
        case Code::POP:
        {
            uint8_t segment = readCur<uint8_t>();
            uint32_t value = getTop();

            switch (segment)
            {
            case Code::LOCAL:
            {
                uint8_t index = readCur<uint8_t>();
                local.at(index) = value;
                break;
            }
            case Code::OUT:
            {
                uint8_t index = readCur<uint8_t>();
                switch (index)
                {
                // 정수
                case 0:
                {
                    cout << value;
                    break;
                }
                // 문자
                case 1:
                {
                    char c = (char)value;
                    cout << c;
                }
                // hex
                case 2:
                {
                    cout << hex << value;
                }
                }
                break;
            }
            }

            break;
        }
        }
    }

    int32_t getTop()
    {
        int32_t value = stack.top();
        stack.pop();
        return value;
    }

    template <typename T>
    T readCur()
    {
        T value;
        in.read((char *)&value, sizeof(T));
        return value;
    }
};
