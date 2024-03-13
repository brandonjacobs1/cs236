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
    Relation() = default;
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
    Relation selectConstant(int index, const string& value) const {
        Relation result(name, scheme);
        for (const Tuple& tuple : tuples) {
            if (tuple.at(index) == value) {
                result.addTuple(tuple);
            }
        }
        return result;
    }
    Relation selectMultiple(vector<int> indicies) {
        Relation result(name, scheme);
        for (const Tuple& tuple : tuples) {
            string value = tuple.at(indicies.at(0));
            bool match = true;
            for (int index : indicies) {
                if (tuple.at(index) != value) {
                    match = false;
                }
            }
            if (match) {
                result.addTuple(tuple);
            }
        }
        return result;
    }

    Relation project(vector<int> indicies) {
        vector<string> newScheme;
        for (int index : indicies) {
            newScheme.push_back(scheme.at(index));
        }
        Relation relation = Relation(name, newScheme);
        for (Tuple tuple : tuples) {
            vector<string> values;
            for (int index : indicies) {
                values.push_back(tuple.at(index));
            }
            relation.addTuple(Tuple(values));
        }
        return relation;

    }
    Relation rename(vector<string> newNames){
        Relation relation = Relation(name, newNames);
        for (const Tuple& tuple : tuples) {
            relation.addTuple(tuple);
        }
        return relation;
    }
    bool operator<(const Relation& other) const {
        return name < other.name;
    }
    string getName() {
        return name;
    }
    set<Tuple> getTuples() {
        return tuples;
    }
    Scheme getScheme() {
        return scheme;
    }
};


#endif //CS236_RELATION_H
