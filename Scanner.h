#pragma once

#ifndef CS236_SCANNER_H
#define CS236_SCANNER_H

#include "Token.h"
#include <cctype>
#include <vector>
#include <fstream>

using namespace std;

class Scanner {
private:
    string input;
    int line;
    vector<Token> tokens;

    void addToken(TokenType type, string value, int lineNum){
        Token t = Token(type, value, lineNum);
        tokens.push_back(t);
    }

public:
    Scanner() : line(1) { }

    vector<Token> getTokens() {
        return tokens;
    }

    vector<Token> scan(const string& fileName) {
        ifstream infile(fileName);
        if (infile.is_open()) {
            stringstream ss;
            ss << infile.rdbuf();
            input = ss.str();
            infile.close();
        }

        for (size_t i=0; i < input.length(); i++) {
            char c = input[i];
            if (c == '\n') {
                line++;
            } else if (isspace(c)) {
                continue;
            } else if (c == ','){
                    addToken(COMMA, ",", line);
            } else if (c == ':'){
                if (input[i+1] == '-') {
                    addToken(COLON_DASH, ":-", line);
                    i++;
                } else {
                    addToken(COLON, ":", line);
                }
            }else if (c == '.'){
                addToken(PERIOD, ".", line);
            }else if (c == '('){
                addToken(LEFT_PAREN, "(", line);
            }else if (c == ')'){
                addToken(RIGHT_PAREN, ")", line);
            }else if (c == '+'){
                addToken(ADD, "+", line);
            }else if (c == '*'){
                addToken(MULTIPLY, "*", line);
            }else if (c == '?'){
                addToken(Q_MARK, "?", line);
            }else if (c == '\'') {
                std::string characters = "'";
                ++i;
                int tempLine = line;
                while (i < input.size()) {
                    if (input[i] != '\'') {
                        if (input[i] == '\n') line++;

                        characters += input[i];
                        ++i;
                    } else {
                        characters += '\'';
                        break;
                    }
                }
                if (i <= input.size() && characters[characters.length() - 1] == '\'') {
                    Token t = Token(STRING, characters, tempLine);
                    tokens.push_back(t);
                } else {
                    Token t = Token(UNDEFINED, characters, tempLine);
                    tokens.push_back(t);
                }
                continue;
            }else if (c == '#') {
                std::string characters = "#";
                ++i;

                while (i < input.size() && input[i] != '\n') {
                    characters += input[i];
                    ++i;
                }
                if (i <= input.size()) {
                    Token t = Token(COMMENT, characters, line);
                    // tokens.push_back(t);
                } else {
                    Token t = Token(UNDEFINED, characters, line);
                    // tokens.push_back(t);
                }
                i--;
                continue;
            }else if (isalpha(c)){
                std::string characters;
                while (i < input.size() && (isalpha(input[i]) || isdigit(input[i]))) {
                    characters += input[i];
                    ++i;
                }
                if (i <= input.size()) {
                    if (characters == "Queries") {
                        Token t = Token(QUERIES, characters, line);
                        tokens.push_back(t);
                    } else if (characters == "Facts") {
                        Token t = Token(FACTS, characters, line);
                        tokens.push_back(t);
                    } else if (characters == "Schemes") {
                        Token t = Token(SCHEMES, characters, line);
                        tokens.push_back(t);
                    } else if (characters == "Rules") {
                        Token t = Token(RULES, characters, line);
                        tokens.push_back(t);
                    } else {
                        Token t = Token(ID, characters, line);
                        tokens.push_back(t);
                    }
                } else {
                    Token t = Token(UNDEFINED, characters, line);
                    tokens.push_back(t);
                }
                i--;
                continue;
            } else {
                string character(1, input[i]);
                Token t = Token(UNDEFINED, character, line);
                tokens.push_back(t);
            }
        }
        addToken(END, "", line);
        return tokens;
    }
};

#endif