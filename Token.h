//
// Created by Brandon Jacobs on 1/23/24.
//
#pragma once

#ifndef CS236_TOKEN_H
#define CS236_TOKEN_H

#include <sstream>
#include <string>
#include <utility>

using namespace std;

enum TokenType {
    COMMA, PERIOD, Q_MARK, LEFT_PAREN, RIGHT_PAREN, COLON, COLON_DASH,
    MULTIPLY, ADD, SCHEMES, FACTS, RULES, QUERIES, ID, STRING, COMMENT, END,
    UNDEFINED
};



class Token {
private:
    TokenType type;
    int line;
    string value;

    [[nodiscard]] std::string typeName(TokenType tempType) const {
        switch (type) {
            case COMMA:
                return "COMMA";
            case LEFT_PAREN:
                return "LEFT_PAREN";
            case RIGHT_PAREN:
                return "RIGHT_PAREN";
            case PERIOD:
                return "PERIOD";
            case Q_MARK:
                return "Q_MARK";
            case COLON:
                return "COLON";
            case COLON_DASH:
                return "COLON_DASH";
            case MULTIPLY:
                return "MULTIPLY";
            case ADD:
                return "ADD";
            case SCHEMES:
                return "SCHEMES";
            case FACTS:
                return "FACTS";
            case RULES:
                return "RULES";
            case QUERIES:
                return "QUERIES";
            case STRING:
                return "STRING";
            case COMMENT:
                return "COMMENT";
            case ID:
                return "ID";
            case END:
                return "EOF";
            case UNDEFINED:
                return "UNDEFINED";
            default:
                return "UNKNOWN";
        }
    }

public:
    Token(TokenType type, string value, int line) : type(type), line(line), value(std::move(value)) { }

    [[nodiscard]] string toString() const {
        stringstream out;
        out << "(" << typeName(type) << "," << "\"" << value << "\"" << "," << line << ")";
        return out.str();
    }

    TokenType getType() const {
        return type;
    }
    string getValue() const {
        return value;
    }

};




#endif //CS236_TOKEN_H
