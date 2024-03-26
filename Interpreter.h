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
    stringstream ruleEvaluationOutput;
    stringstream queryEvaluationOutput;

public:
    Interpreter(const DatalogProgram& dp, const Database& db) {
        datalogProgram = dp;
        database = db;
        loadData();
    }

    void run() {
        ruleEvaluationOutput << "Rule Evaluation" << endl;
        fixedPointRuleAlgorithm(datalogProgram.getRules());
        cout << ruleEvaluationOutput.str();

        queryEvaluationOutput << endl << "Query Evaluation" << endl;
        for (Predicate query: datalogProgram.getQueries()) {
            Relation relation = evaluateQuery(query);
            queryToString(query, relation);
        }
        cout << queryEvaluationOutput.str();
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
    Relation evaluateQuery(Predicate query) {
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
        return relation;
    }
    void queryToString(Predicate query, Relation relation) {
        queryEvaluationOutput << query.toString() << "? ";
        Tuple firstTuple = *(relation.getTuples().begin());
        if (!relation.getTuples().empty() && !firstTuple.empty()){
            queryEvaluationOutput << "Yes(" << relation.getTuples().size() << ")" << endl;
        } else {
            queryEvaluationOutput << "No" << endl;
        }
        Scheme scheme = relation.getScheme();
        if (scheme.empty() || allConstants) {
            return;
        }
        for (Tuple tuple : relation.getTuples()) {
            queryEvaluationOutput << "  ";
            for (size_t i = 0; i < scheme.size(); ++i) {
                queryEvaluationOutput << scheme.at(i) << "=" << tuple.at(i);
                if (i < scheme.size() - 1) {
                    queryEvaluationOutput << ", ";
                }
            }
            queryEvaluationOutput << endl;
        }
    }

    void evaluateRule(Rule rule) {
        Predicate headPredicate = rule.getHeadPredicate();
        vector<Predicate> predicateList = rule.getPredicateList();

        vector<Relation> relations;
        for (Predicate predicate : predicateList) {
            Relation relation = evaluateQuery(predicate);
            relations.push_back(relation);
        }
        Relation finalRelation = relations.at(0);
        for (int i=1; i<relations.size(); i++) {
            Relation rel = finalRelation.join(relations.at(i));
            finalRelation = rel;
        }

        // Get indicies of head predicate params in final relation
        vector<int> indicies = vector<int>();
        vector<Parameter> headParams = headPredicate.getParameters();
        for (size_t i=0; i<headParams.size(); i++) {
            Parameter param = headParams.at(i);
            for (size_t j=0; j<finalRelation.getScheme().size(); j++) {
                if (param.toString() == finalRelation.getScheme().at(j)) {
                    indicies.push_back(j);
                    break;
                }
            }
        }

        Relation projected;
        projected = finalRelation.project(indicies);
        Relation renamed;
        Relation existingRelation = database.findRelation(headPredicate.getName());
        renamed = projected.rename(existingRelation.getScheme());
        Relation existing = database.findRelation(headPredicate.getName());
        Relation unionized = database.unionize(existing, renamed, ruleEvaluationOutput);
        database.replaceRelation(unionized);
    }

    void fixedPointRuleAlgorithm(vector<Rule> rules) {
        int numPreviousTuples;
        int numTuples;
        int numPasses = 0;
        do {
            numPreviousTuples = database.countTuples();
            for (Rule rule : rules) {
                ruleEvaluationOutput << rule.toString() << endl;
                evaluateRule(rule);
            }
            numTuples = database.countTuples();
            numPasses++;
        } while (numTuples != numPreviousTuples);

        ruleEvaluationOutput << endl << "Schemes populated after " << numPasses << " passes through the Rules." << endl;
    }
};



#endif //CS236_INTERPRETER_H
