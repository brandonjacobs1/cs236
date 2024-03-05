//
// Created by Brandon Jacobs on 3/4/24.
//

#ifndef CS236_RELATION_H
#define CS236_RELATION_H
using namespace std;
#include <string>
#include <set>
#include "Scheme.h"
#include "Tuple.h"


class Relation {

private:

    string name;
    Scheme scheme;
    set<Tuple> tuples;

public:

    Relation(const string& name, const Scheme& scheme)
            : name(name), scheme(scheme) { }

    void addTuple(const Tuple& tuple) {
        tuples.insert(tuple);
    }
    string toString() const {
        stringstream out;
        for (const Tuple& tuple : tuples) {
            out << tuple.toString(scheme) << endl;
        }
        return out.str();
    }
    Relation select(int index, const string& value) const {
        Relation result(name, scheme);
        for (const Tuple& tuple : tuples) {
            if (tuple.at(index) == value) {
                result.addTuple(tuple);
            }
        }
        return result;
    }
};


#endif //CS236_RELATION_H
