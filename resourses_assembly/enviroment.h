#pragma once

// this file uses C code preprocessor to make all assembly settings

#include <string>

namespace assemblyLang {

const size_t npos = ((size_t)-1);

enum ArgType {
    ARG_NONE  = 0b000,
    ARG_MEM   = 0b001,
    ARG_REG   = 0b010,
    ARG_CONST = 0b100,
    ARG_ANY   = ARG_REG | ARG_MEM | ARG_CONST,
    ARG_VARY  = ARG_REG | ARG_MEM            ,
};

const std::string registers[]  = {
    #define REGISTER(reg) #reg ,
    #include "registers.src"
    #undef REGISTER
};
const std::string opcodes[]    = {
    #define OPCODE(opcode, ...) #opcode ,
    #include "opcode.src"
    #undef OPCODE
};
const std::string directives[] = {
    #define DIRECTIVE(directive) #directive ,
    #include "directives.src"
    #undef DIRECTIVE
};

template <typename T>
constexpr size_t find (const T arr[], size_t size, const T& elem) {
    for (size_t i = 0; i < size; i++) 
        if (elem == arr[i]) return i;
    
    return npos;
}

enum RegCode {
    #define REGISTER(reg) REG_##reg = find(registers, sizeof(registers) / sizeof(std::string), #reg),
    #include "registers.src"
    #undef REGISTER
};
enum OpcodeCode {
    #define OPCODE(opcode, ...) OPCODE_##opcode = find(opcodes, sizeof(opcodes) / sizeof(std::string), #opcode),
    #include "opcode.src"
    #undef OPCODE
};
enum DirectivesCode {
    #define DIRECTIVE(directive) DIRECTIVE_##directive = find(directives, sizeof(directives) / sizeof(std::string), #directive),
    #include "directives.src"
    #undef DIRECTIVE
};

}