#pragma once

#include <vector>
#include <string>
#include <compare>
#include <iostream>
#include <algorithm>

bool isPrefix (const std::string& str, const std::string& pref);

class Text {
    std::vector<std::string> storage;

    friend std::istream& operator>> (std::istream& stream, Text& text);
    friend class Language;
};

class Language {
protected:
    const static size_t npos = -1;
public:
    struct Lexem {
        size_t pos;
        size_t line;
        std::string content;

        Lexem (const char* old_st_content, size_t line = npos, size_t pos = npos) : 
               pos(pos), line(line), content(old_st_content) {}
        Lexem (const std::string& content, size_t line = npos, size_t pos = npos) : 
               pos(pos), line(line), content(content) {}

        friend bool operator== (const Lexem& lhv, const Lexem& rhv);
        friend bool operator!= (const Lexem& lhv, const Lexem& rhv);
        friend std::ostream& operator<< (std::ostream& stream, Language::Lexem& lex);
    };
protected:
    std::vector<Lexem> specialLexems;
    std::vector<Lexem> text;

    const Lexem eolLexem = "\n";

    static bool specialLexemComparator (const Lexem& lhv, const Lexem& rhv);

    bool tryEOLLexem   (const std::string& line, size_t line_no, size_t& pos);
    bool trySpecLexem  (const std::string& line, size_t line_no, size_t& pos);
    bool tryEmptyLexem (const std::string& line, size_t line_no, size_t& pos);
public:
    explicit Language (const std::vector<Lexem>&       specialLexems);

    ~Language () {}

    void processCode (const Text& code);
};

std::istream& operator>> (std::istream& stream, Text& text);
std::ostream& operator<< (std::ostream& stream, Language::Lexem& lex);
bool operator== (const Language::Lexem& lhv, const Language::Lexem& rhv);
