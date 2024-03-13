//
// Created by Brandon Jacobs on 3/11/24.
//

#ifndef CS236_INTERPRETER_H
#define CS236_INTERPRETER_H

#include <map>
#include "Database.h"
#include "DatalogProgram.h"

class Interpreter {
    Database database;
    DatalogProgram datalogProgram;

public:
    Interpreter(const DatalogProgram& dp, const Database& db) {
        datalogProgram = dp;
        database = db;
        loadData();
        query();
    }
private:
    void loadData() {
        for (Predicate scheme : datalogProgram.getSchemes()) {
            vector<string> columns;
            for (Parameter param : scheme.getParameters()) {
                columns.push_back(param.toString());
            }
            Relation relation = Relation(scheme.getName(), columns);
            for (Predicate fact : datalogProgram.getFacts()) {
                if (fact.getName() == scheme.getName()) {
                    vector<string> values;
                    for (Parameter param : fact.getParameters()) {
                        values.push_back(param.toString());
                    }
                    Tuple tuple = Tuple(values);
                    relation.addTuple(tuple);
                }
            }
            database.addRelation(relation);
        }
    }
    bool allConstants;
    void query() {
        for (Predicate query: datalogProgram.getQueries()) {
            // select
            Relation relation = database.select(query);
            allConstants = true;
            for (Parameter param : query.getParameters()) {
                if (param.getType() == ID) {
                    allConstants = false;
                }
            }
            if (!allConstants) {
                // project
                relation = database.project(query, relation);
                // rename
                relation = database.rename(query, relation);
            }
            printQuery(query, relation);

        }
    }
    void printQuery(Predicate query, Relation relation) {
        cout << query.toString() << "? ";
        Tuple firstTuple = *(relation.getTuples().begin());
        if (!relation.getTuples().empty() && !firstTuple.empty()){
            cout << "Yes(" << relation.getTuples().size() << ")" << endl;
        } else {
            cout << "No" << endl;
        }
        Scheme scheme = relation.getScheme();
        if (scheme.empty() || allConstants) {
            return;
        }
        for (Tuple tuple : relation.getTuples()) {
            cout << " ";
            for (size_t i = 0; i < scheme.size(); ++i) {
                cout << scheme.at(i) << "=" << tuple.at(i);
                if (i < scheme.size() - 1) {
                    cout << ", ";
                }
            }
            cout << endl;
        }
    }
};



#endif //CS236_INTERPRETER_H
