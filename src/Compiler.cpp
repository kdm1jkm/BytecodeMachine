#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <queue>
#include <fstream>

#include "Compiler.h"

std::string convertUpper(std::string str)
{
    std::transform(str.begin(), str.end(), str.begin(), ::toupper);
    return str;
}

enum Symbol
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
    OUT,
    LABEL,
    LITERAL,
    LABEL_NAME
};

const std::map<std::string, Symbol> SymbolTable{
    {"ADD", Symbol::ADD},
    {"SUB", Symbol::SUBTRACT},
    {"NEG", Symbol::NEGATIVE},
    {"EQ", Symbol::EQUAL},
    {"GT", Symbol::GREAT_THAN},
    {"LT", Symbol::LESS_THAN},
    {"AND", Symbol::AND},
    {"OR", Symbol::OR},
    {"JUMP", Symbol::JUMP},
    {"IF-JUMP", Symbol::IF_JUMP},
    {"PUSH", Symbol::PUSH},
    {"POP", Symbol::POP},
    {"CONSTANT", Symbol::CONSTANT},
    {"ARGUMENT", Symbol::ARGUMENT},
    {"LOCAL", Symbol::LOCAL},
    {"IN", Symbol::IN},
    {"OUT", Symbol::OUT},
    {"LABEL", Symbol::LABEL}};

const std::vector<Symbol> segments{Symbol::CONSTANT, Symbol::ARGUMENT, Symbol::LOCAL, Symbol::IN, Symbol::OUT};

Compiler::Compiler(const std::string &filename)
    : in(filename, std::ios::binary),
      output((filename.find_last_of(".") == std::string::npos) ? filename : filename.substr(0, (filename.find_last_of("."))))
{
}

Compiler::Compiler(const std::string &filename, const std::string output) : in(filename, std::ios::binary), output(output)
{
}

void Compiler::compile()
{
    std::vector<std::string> labels;
    in.seekg(0, std::ios::beg);
    while (!in.eof())
    {
        std::string word;
        in >> word;

        if (convertUpper(word) == "LABEL")
        {
            std::string label;
            in >> label;

            // 이미 등록한 라벨이라면
            if (std::find(labels.begin(), labels.end(), convertUpper(label)) != labels.end())
            {
                printError([&label](int line, int column)
                           { std::cout << "\"" << label << "\" is already used.[" << line << ":" << column << "]" << std::endl; });
            }

            // 미리 예약된 기호라면
            if (SymbolTable.find(convertUpper(label)) != SymbolTable.end())
            {
                // 라벨로 쓸 수 없음
                printError([&label](int line, int column)
                           { std::cout << "\"" << label << "\" cannot be used as label. [" << line << ":" << column << "]" << std::endl; });
                return;
            }
            labels.push_back(convertUpper(label));
        }
    }

    // 토큰화(?)
    std::vector<Symbol> symbols;
    std::queue<int32_t> literals;
    std::queue<uint32_t> labelTable;
    int localCount = 0;
    int argumentCount = 0;
    in.seekg(0, std::ios::beg);
    while (!in.eof())
    {
        std::string word;
        in >> word;

        auto symbolIter = SymbolTable.find(convertUpper(word));
        // 미리 예약된 기호
        if (symbolIter != SymbolTable.end())
        {
            symbols.push_back(symbolIter->second);

            // POP, PUSH 다음은 무조선 SEGMENT, 리터럴
            if (symbolIter->second == Symbol::POP || symbolIter->second == Symbol::PUSH)
            {
                std::string segment;
                in >> segment;
                auto symbolSegmentIter = SymbolTable.find(convertUpper(segment));
                // 존재하지 않는 Symbol
                if (symbolSegmentIter == SymbolTable.end())
                {
                    printError([&segment](int l, int c)
                               { std::cout << "\"" << segment << "\" is not defined.[" << l << ":" << c << "]" << std::endl; });
                    return;
                }

                Symbol symbolSegment = symbolSegmentIter->second;

                // Segment가 아님
                if (std::find(segments.begin(), segments.end(), symbolSegment) == segments.end())
                {
                    printError([&segment](int line, int column)
                               { std::cout << "\"" << segment << "\" is not a segment.[" << line << ":" << column
                                           << "]" << std::endl; });
                    return;
                }

                symbols.push_back(symbolSegment);

                // 리터럴 구하기
                std::string literal;
                in >> literal;

                // 숫자가 아니면
                if (literal.find_first_of("0123456789") == std::string::npos)
                {
                    printError([&literal](int line, int column)
                               { std::cout << "\"" << literal << "\" is not literal.[" << line << ":" << column << "]" << std::endl; });
                    return;
                }

                symbols.push_back(Symbol::LITERAL);

                int32_t value;
                std::stringstream(literal) >> value;
                literals.push(value);

                if (symbolSegment == Symbol::LOCAL)
                    localCount = std::max<int>(localCount, value + 1);
                if (symbolSegment == Symbol::ARGUMENT)
                    argumentCount = std::max<int>(argumentCount, value + 1);
            }
        }
        // 라벨
        else if (std::find(labels.begin(), labels.end(), convertUpper(word)) != labels.end())
        {
            if (*(symbols.end() - 1) == Symbol::LABEL)
                continue;
            symbols.push_back(Symbol::LABEL_NAME);
            labelTable.push(std::find(labels.begin(), labels.end(), convertUpper(word)) - labels.begin());
        }
        else // 해석 불가
        {
            printError([&word](int line, int column)
                       { std::cout << "\"" << word << "\" is not defined.[" << line << ":" << column << "]" << std::endl; });
            return;
        }
    }

    // 해석

    // 라벨 주소
    std::vector<int> labelAddress;
    unsigned int pos = 0;
    for (auto &&symbol : symbols)
    {
        switch (symbol)
        {
        case Symbol::LABEL:
            labelAddress.push_back(pos + 2);
            break;

        case Symbol::LABEL_NAME:
            break;

        case Symbol::JUMP:
        case Symbol::IF_JUMP:
            pos++;
        case Symbol::CONSTANT:
            pos += 4;
            break;

        default:
            pos++;
            break;
        }
    }

    // 해석하기
    std::vector<uint8_t> commands;
    commands.push_back(localCount);
    commands.push_back(argumentCount);

    for (auto i = symbols.begin(); i != symbols.end(); ++i)
    {
        switch (*i)
        {
        case Symbol::LABEL_NAME:
        {
            if (*(i - 1) == Symbol::LABEL)
                continue;
            auto label = labelTable.front();
            uint32_t address = labelAddress.at(label);

            commands.push_back(
                0x000000FF & address);
            commands.push_back(
                (0x0000FF00 & address >> 2));
            commands.push_back(
                (0x00FF0000 & address) >> 4);
            commands.push_back(
                address >> 6);

            labelTable.pop();
            break;
        }
        case Symbol::LITERAL:
        {
            Symbol last = ((Symbol) * (commands.end() - 1));
            if (last == Symbol::CONSTANT)
            {
                commands.push_back(
                    0x000000FF & literals.front());
                commands.push_back(
                    (0x0000FF00 & literals.front()) >> 2);
                commands.push_back(
                    (0x00FF0000 & literals.front()) >> 4);
                commands.push_back(
                    literals.front() >> 6);
            }
            else
            {
                commands.push_back(0x000000FF & literals.front());
            }
            literals.pop();
            break;
        }

        case LABEL:
            break;

        default:
            commands.push_back(*i);
        }
    }

    // 출력하기
    std::ofstream out(output, std::ios::binary | std::ios::trunc);
    std::for_each(commands.begin(), commands.end(), [&out](uint8_t command)
                  { out.write((char *)&command, 1); });
}

Compiler::~Compiler()
{
    in.close();
}

void Compiler::printError(std::function<void(int line, int column)> showMessage)
{
    int index = in.tellg();

    int line = 1, column = 1;
    in.seekg(0, std::ios::beg);

    for (int i = 0; i < index; i++)
    {
        column++;
        char c;
        in.read(&c, 1);
        if (c == '\n')
        {
            column = 1;
            line++;
        }
    }

    showMessage(line, column);
}
