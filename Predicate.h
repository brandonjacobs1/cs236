//
// Created by Brandon Jacobs on 2/12/24.
//

#ifndef CS236_PREDICATE_H
#define CS236_PREDICATE_H
using namespace std;
#include <string>
#include <utility>
#include "Parameter.h"
#include <sstream>

class Predicate {
private:
    string name;
    vector<Parameter> parameters;
public:
    explicit Predicate(string value): name(std::move(value)){}
    string getName() {
        return name;
    }
    void addParameter (const Parameter& newParam) {
        parameters.push_back(newParam);
    }
    vector<Parameter> getParameters() {
        return parameters;
    }

    string toString() {
        stringstream ss;

        ss << name << '('; // Append the name and opening parenthesis

        if (!parameters.empty()) {
            ss << parameters[0].toString();

            for (size_t i = 1; i < parameters.size(); ++i) {
                ss << "," << parameters[i].toString();
            }
        }

        ss << ')'; // Append the closing parenthesis
        return ss.str();
    }
};


#endif //CS236_PREDICATE_H
