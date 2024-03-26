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
#include <cctype>


class Relation {

private:

    string name;
    Scheme scheme;
    set<Tuple> tuples;

public:
    Relation() = default;

    Relation(const string &name, const Scheme &scheme)
            : name(name), scheme(scheme) {}

    void addTuple(const Tuple &tuple) {
        tuples.insert(tuple);
    }

    string toString() const {
        stringstream out;
        for (const Tuple &tuple: tuples) {
            out << "  " << tuple.toString(scheme) << endl;
        }
        return out.str();
    }

    Relation selectConstant(int index, const string &value) const {
        Relation result(name, scheme);
        for (const Tuple &tuple: tuples) {
            if (tuple.at(index) == value) {
                result.addTuple(tuple);
            }
        }
        return result;
    }

    Relation selectMultiple(vector<int> indicies) {
        Relation result(name, scheme);
        for (const Tuple &tuple: tuples) {
            string value = tuple.at(indicies.at(0));
            bool match = true;
            for (int index: indicies) {
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
        for (int index: indicies) {
            newScheme.push_back(scheme.at(index));
        }
        Relation relation = Relation(name, newScheme);
        for (Tuple tuple: tuples) {
            vector<string> values;
            for (int index: indicies) {
                values.push_back(tuple.at(index));
            }
            relation.addTuple(Tuple(values));
        }
        return relation;

    }

    Relation rename(vector<string> newNames) {
        Relation relation = Relation(name, newNames);
        for (const Tuple &tuple: tuples) {
            relation.addTuple(tuple);
        }
        return relation;
    }

    bool operator<(const Relation &other) const {
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

    static bool joinable(const Scheme &leftScheme, const Scheme &rightScheme, const Tuple &leftTuple, const Tuple &rightTuple) {
        for (int leftIndex = 0; leftIndex < leftScheme.size(); ++leftIndex) {
            const string& leftName = leftScheme.at(leftIndex);
            for (int rightIndex = 0; rightIndex < rightScheme.size(); ++rightIndex) {
                const string& rightName = rightScheme.at(rightIndex);
                if (leftName == rightName) {
                    if (leftTuple.at(leftIndex) != rightTuple.at(rightIndex)) {
                        return false;
                    }
                }
            }
        }
        return true;
    }

    Relation join(Relation &right) {
        Relation &left = *this;
        Relation result;

        Scheme joinedScheme = joinSchemes(left.getScheme(), right.getScheme());
        result = Relation(left.getName() + "_and_" + right.getName(), joinedScheme);

        for (Tuple leftTuple: left.getTuples()) {
            for (Tuple rightTuple: right.getTuples()) {
                if (joinable(left.getScheme(), right.getScheme(), leftTuple, rightTuple)) {
                    Tuple joinedTuple = joinTuples(joinedScheme, left.getScheme(), right.getScheme(), leftTuple, rightTuple);
                    result.addTuple(joinedTuple);
                }
            }
        }

        return result;
    }

    Scheme joinSchemes(const Scheme &leftScheme, const Scheme &rightScheme) {
        set<string> uniqueNames(leftScheme.begin(), leftScheme.end());
        uniqueNames.insert(rightScheme.begin(), rightScheme.end());
        vector<string> joinedScheme(uniqueNames.begin(), uniqueNames.end());
        return joinedScheme;
    }

    Tuple joinTuples(Scheme& newScheme, Scheme leftScheme, Scheme rightScheme, Tuple& leftTuple, Tuple& rightTuple) {
        vector<string> values(newScheme.size());
        for (int i = 0; i < leftScheme.size(); i++) {
            string col = leftScheme.at(i);
            auto findResult = find(newScheme.begin(), newScheme.end(), col);
            if (findResult != newScheme.end()) {
                int newIndex = findResult - newScheme.begin();
                values.at(newIndex) = leftTuple.at(i);
            }
        }
        for (int i = 0; i < rightScheme.size(); i++) {
            string col = rightScheme.at(i);
            auto findResult = find(newScheme.begin(), newScheme.end(), col);
            if (findResult != newScheme.end()) {
                int newIndex = findResult - newScheme.begin();
                values.at(newIndex) = rightTuple.at(i);
            }
        }
        return Tuple(values);
    }
};

#endif //CS236_RELATION_H
