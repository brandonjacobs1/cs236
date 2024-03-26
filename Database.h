//
// Created by Brandon Jacobs on 3/11/24.
//

#ifndef CS236_DATABASE_H
#define CS236_DATABASE_H

using namespace std;
#include "Relation.h"
#include <unordered_set>

class Database {
    set<Relation> relations;

public:
    Database () {
        relations = set<Relation>();
    }
    set<Relation> getRelations() {
        return relations;
    }
        [[nodiscard]] string toString() const {
            stringstream out;
            for (const Relation& relation : relations) {
                out << relation.toString() << endl;
            }
            return out.str();
        }
    void addRelation(const Relation& relation) {
        relations.insert(relation);
    }
    Relation findRelation (string name) {
        for (Relation relation : relations) {
            if (relation.getName() == name) {
                return relation;
            }
        }
        return {};
    }
    Relation project(Predicate query, Relation relation) {
        vector<int> indicies = vector<int>();
        vector<Parameter> addedParams = vector<Parameter>();
        for (size_t i=0; i<query.getParameters().size(); i++) {
            Parameter param = query.getParameters().at(i);
            bool found = false;
            for (Parameter addedParam : addedParams) {
                if (param.toString() == addedParam.toString()) {
                   found = true;
                   break;
                }
            }
            if (!found) {
                if (param.getType() == ID) {
                    indicies.push_back(i);
                    addedParams.push_back(param);
                }
            }
        }
        return relation.project(indicies);
    }
    Relation select(Predicate query) {
        Relation currentRelation = findRelation(query.getName());
        // select
        vector<Parameter> parameters = query.getParameters();
        map<Parameter, vector<int>> variables = map<Parameter, vector<int>>();
        for(size_t i=0; i<parameters.size(); i++) {
            Parameter param = parameters.at(i);
            variables[param].push_back(i);
        }
        for (auto& [param, indicies] : variables) {
            if (param.getType() == ID) {
                if (indicies.size() > 1) {
                    currentRelation = currentRelation.selectMultiple(indicies);
                } else {
                    continue;
                }
            } else {
                for (int i : indicies) {
                    currentRelation = currentRelation.selectConstant(i, param.toString());
                }
            }
        }
        return currentRelation;
    }
    Relation rename(Predicate query, Relation relation) {
        vector<string> variables;
        for (size_t i = 0; i < query.getParameters().size(); i++) {
            Parameter param = query.getParameters().at(i);
            bool found = false;
            for (string variable : variables) {
                if (param.toString() == variable) {
                    found = true;
                    break;
                }
            }
            if (param.getType() == ID && !found) {
                variables.push_back(param.toString());
            }
        }
        return relation.rename(variables);
    }

    void replaceRelation(Relation relation) {
        for (Relation currentRelation : relations) {
            if (currentRelation.getName() == relation.getName()) {
                relations.erase(currentRelation);
                relations.insert(relation);
                return;
            }
        }
    }

    Relation unionize(Relation firstRelation, Relation secondRelation, stringstream& output) {
        Relation result = Relation(firstRelation.getName(), firstRelation.getScheme());
        // check if the schemes are the same
        if (firstRelation.getScheme().size() == secondRelation.getScheme().size()) {
            for (int i = 0; i < secondRelation.getScheme().size(); i++) {
                string firstValue = firstRelation.getScheme().at(i);
                string secondValue = secondRelation.getScheme().at(i);
                if (firstValue != secondValue) {
                    return result;
                }
            }
        } else {
            return result;
        }
        for (Tuple tuple: firstRelation.getTuples()) {
            result.addTuple(tuple);
        }
        for (Tuple tuple: secondRelation.getTuples()) {
            int numTuples = result.getTuples().size();
            result.addTuple(tuple);
            int newNumTuples = result.getTuples().size();
            if (numTuples != newNumTuples) {
                output << "  " << tuple.toString(firstRelation.getScheme()) << endl;
            }
        }
        return result;

    }
    int countTuples(){
        int count = 0;
        for (Relation relation : relations) {
            count += relation.getTuples().size();
        }
        return count;
    }

};


#endif //CS236_DATABASE_H
