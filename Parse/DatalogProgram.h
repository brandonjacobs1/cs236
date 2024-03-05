//
// Created by Brandon Jacobs on 2/12/24.
//

#ifndef CS236_DATALOGPROGRAM_H
#define CS236_DATALOGPROGRAM_H
#include "Predicate.h"
#include "Rule.h"
#include <vector>
#include <sstream>
#include <set>

class DatalogProgram {
private:
    vector<Predicate> schemes;
    vector<Predicate> facts;
    vector<Rule> rules;
    vector<Predicate> queries;
public:
    DatalogProgram() = default;

    [[nodiscard]] const vector<Predicate> &getSchemes() const {
        return schemes;
    }
    [[nodiscard]] const vector<Predicate> &getFacts() const {
        return facts;
    }
    [[nodiscard]] const vector<Predicate> &getQueries() const {
        return queries;
    }
    [[nodiscard]] const vector<Rule> &getRules() const {
        return rules;
    }

    void addScheme(const Predicate& predicate) {
        schemes.push_back(predicate);
    }
    void addFact(const Predicate& predicate) {
        facts.push_back(predicate);
    }
    void addRule(const Rule& rule) {
        rules.push_back(rule);
    }
    void addQuery(const Predicate& predicate) {
        queries.push_back(predicate);
    }

    stringstream factsToString (vector<Predicate> data) {
        stringstream ss;
        for (Predicate row : data) {
            ss <<  "  " << row.toString() << '.' << endl;
        }
        return ss;
    }

    stringstream queriesToString (vector<Predicate> data) {
        stringstream ss;
        for (Predicate row : data) {
            ss <<  "  " << row.toString() << '?' << endl;
        }
        return ss;
    }

    stringstream schemesToString (vector<Predicate> data) {
        stringstream ss;
        for (Predicate row : data) {
            ss <<  "  " << row.toString() << endl;
        }
        return ss;
    }

    stringstream rulesToString(vector<Rule> data) {
        stringstream ss;
        for (Rule row : data) {
            ss << "  " << row.toString() << '.' << endl;
        }
        return ss;
    }

    set<string> getDomain() {
        set<string> domain;
        for (auto& fact : facts) {
            for(auto& param : fact.getParameters()) {
                domain.insert(param.getValue());
            }
        }
        return domain;
    }

    string printDomain(set<string> domain) {
        stringstream ss;

        auto it = domain.begin();
        while (it != domain.end()) {
            ss << "  " << *it;
            if (++it != domain.end()) {
                ss << endl;
            }
        }

        return ss.str();
    }
    string toString() {
        stringstream ss;
        ss << "Schemes(" << schemes.size() << "):"<< endl;
        ss << schemesToString(schemes).str();
        ss << "Facts(" << facts.size() << "):"<< endl;
        ss << factsToString(facts).str();
        ss << "Rules(" << rules.size() << "):"<< endl;
        ss << rulesToString(rules).str();
        ss << "Queries(" << queries.size() << "):"<< endl;
        ss << queriesToString(queries).str();
        set<string> domain = getDomain();
        ss << "Domain(" << domain.size() << "):"<< endl;
        ss << printDomain(domain);
        return ss.str();
    }
};



#endif //CS236_DATALOGPROGRAM_H
