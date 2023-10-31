#include "language.h"

using Lexem = Language::Lexem;

std::istream& operator>> (std::istream& stream, Text& text) {
    while (true) {
        text.storage.push_back("");
        auto& conclusion = std::getline(stream, text.storage.back());
        if (!conclusion) break;
    }

    return stream;
}

bool isPrefix (const std::string& base, const std::string& pref) {
    if (pref.length() >= base.length()) return false;

    for (size_t i = 0; i < pref.length(); i++) {
        if (pref[i] != base[i]) return false;
    }

    return true;
}

bool operator== (const Lexem& lhv, const Lexem& rhv) { 
    return lhv.content == rhv.content;
}

bool Language::specialLexemComparator (const Lexem& lhv, const Lexem& rhv) {
    if (lhv.content.length() > rhv.content.length()) return !specialLexemComparator(rhv, lhv);

    if (isPrefix(rhv.content, lhv.content)) return false;
    
    return lhv.content < rhv.content;
}

Language::Language (const std::vector<Lexem>& specialLexems) : 
      specialLexems(specialLexems) {
    std::sort(this->specialLexems.begin(), this->specialLexems.end(), Language::specialLexemComparator);
}


bool operator!= (const Lexem& lhv, const Lexem& rhv) { 
    return !(lhv == rhv);
}

bool Language::trySpecLexem (const std::string& line, size_t line_no, size_t& pos) {
    for (size_t i = 0; i < specialLexems.size(); i++) {
        if (isPrefix(line.c_str() + pos, specialLexems[i].content)) {
            text.push_back(Lexem(specialLexems[i].content, line_no, pos));
            pos += specialLexems[i].content.length();
            return true;
        }
    }

    return false;
}

bool Language::tryEmptyLexem (const std::string& line, size_t line_no, size_t& pos) {
    const std::string empty = " \t";
    bool res = false;

    for (; pos < line.size(); pos++) {
        if (empty.find(line[pos]) == std::string::npos) {
            break;
        }

        res = true;
    }

    return res;
}

bool Language::tryEOLLexem (const std::string& line, size_t line_no, size_t& pos) {
    const char eol = '\n';
    if (line[pos] == eol) {
        text.push_back(Lexem("\n", line_no, pos));
        return true;
    }

    return false;
}
    
void Language::processCode (const Text& code) {
    for (size_t line = 0; line < code.storage.size(); line++) {
        bool newlex = true;
        for (size_t i = 0; i < code.storage[line].length();) {
            if (trySpecLexem (code.storage[line], line, i)) {
                newlex = true;
                continue;
            }
            if (tryEmptyLexem(code.storage[line], line, i)) {
                newlex = true;
                continue;
            }

            if (tryEOLLexem(code.storage[line], line, i)) break;
            if (newlex) text.push_back(Lexem("", line, i));
            text.back().content.push_back(code.storage[line][i]);

            newlex = false;
            i++;
        }

        if (text.back() != eolLexem) text.push_back(Lexem(("\n"), line, code.storage[line].length() - 1));
    }
}

std::ostream& operator<< (std::ostream& stream, Language::Lexem& lex) {
    stream << "(" << lex.line << ", " << lex.pos << ")\t>>";
    if (lex == Language::Lexem("\n")) {
        stream << "</eol>";
    } else {
        stream << lex.content;
    }

    return stream;
}
