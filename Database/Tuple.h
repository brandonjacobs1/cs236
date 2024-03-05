//
// Created by Brandon Jacobs on 3/4/24.
//

#ifndef CS236_TUPLE_H
#define CS236_TUPLE_H
using namespace std;
#include <vector>
#include <string>
#include "Scheme.h"
#include <sstream>


class Tuple : public vector<string> {

public:

    Tuple(vector<string> values) : vector<string>(values) { }

    string toString(const Scheme& scheme) const {
        const Tuple& tuple = *this;
        stringstream out;
        for (size_t i = 0; i < scheme.size(); ++i) {
            out << scheme.at(i) << "=" << tuple.at(i);
            if (i < scheme.size() - 1) {
                out << ", ";
            }
        }
        return out.str();
    }

};


#endif //CS236_TUPLE_H
