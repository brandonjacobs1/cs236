//
// Created by Brandon Jacobs on 2/12/24.
//

#ifndef CS236_PARAMETER_H
#define CS236_PARAMETER_H

using namespace std;

#include <string>
#include "../Scan/Token.h"

class Parameter {
private:
    string value;
    TokenType type;

public:
    Parameter(std::string value, TokenType type) : value(std::move(value)), type(type) {}

    string getValue() {
        return value;
    }
    TokenType getType() {
        return type;
    }
    string toString() {
       return value;
    }
    };


#endif //CS236_PARAMETER_H
