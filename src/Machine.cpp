#include <iostream>

#include "Machine.h"

Machine::Machine(const std::string &filename, const std::vector<int32_t> &arguments) : in(filename, std::ios::binary)
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

void Machine::runUntilEnd()
{
    while (!in.eof())
    {
        runNextByte();
    }
}

void Machine::runNextByte()
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

        stack.push(0xFFFFFFFF - value);
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
                std::cin >> value;
                break;
            }
            // 문자
            case 1:
            {
                char input;
                std::cin >> input;
                value = input;
                break;
            }
            // hex
            case 2:
            {
                std::cin >> std::hex >> value;
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
                std::cout << value;
                break;
            }
            // 문자
            case 1:
            {
                char c = (char)value;
                std::cout << c;
            }
            // hex
            case 2:
            {
                std::cout << std::hex << value;
            }
            }
            break;
        }
        }

        break;
    }
    }
}

int32_t Machine::getTop()
{
    int32_t value = stack.top();
    stack.pop();
    return value;
}

template <typename T>
T Machine::readCur()
{
    T value;
    in.read((char *)&value, sizeof(T));
    return value;
}
