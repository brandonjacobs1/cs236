//
// Created by Brandon Jacobs on 2/12/24.
//

#ifndef CS236_RULE_H
#define CS236_RULE_H
using namespace std;
#include "Predicate.h"
#include <utility>
#include <vector>
#include <sstream>


class Rule {
private:
    Predicate headPredicate;
    vector<Predicate> predicateList;
public:
    Rule(Predicate head): headPredicate(std::move(head)){}

    void addPredicate(const Predicate& p) {
        predicateList.push_back(p);
    }

    Predicate getHeadPredicate() {
        return headPredicate;
    }

    vector<Predicate> getPredicateList() {
        return predicateList;
    }

    string toString() {
        stringstream ss;
        stringstream outer;
        if (!predicateList.empty()) {
            outer << predicateList[0].toString();

            for (size_t i = 1; i < predicateList.size(); ++i) {
                outer << "," << predicateList[i].toString();
            }
        }
        ss << headPredicate.toString() << " :- " << outer.str() << ".";
        return ss.str();
    }


};


#endif //CS236_RULE_H
