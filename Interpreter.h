//
// Created by Brandon Jacobs on 3/11/24.
//

#ifndef CS236_INTERPRETER_H
#define CS236_INTERPRETER_H

#include <map>
#include <iostream>
#include "Database.h"
#include "DatalogProgram.h"
#include "Graph.h"

using namespace std;

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
        Graph graph = makeGraph(datalogProgram.getRules(), false);
        cout << "Dependency Graph" << endl;
        cout << graph.toString() << endl;

        Graph reverseGraph = makeGraph(datalogProgram.getRules(), true);

        stack<Node> postOrderStack = reverseGraph.dfs();
        vector<vector<Node>> sccs = graph.scc(postOrderStack);


        ruleEvaluationOutput << "Rule Evaluation" << endl;

        for (vector<Node> scc : sccs) {
            sort(scc.begin(), scc.end(), nodeSorter);
            int ruleId = scc[0].getID();
            ruleEvaluationOutput << "SCC: ";
            sccToString(scc);
            ruleEvaluationOutput << endl;
            bool isSelf = isSelfLoop(datalogProgram.getRules().at(ruleId));
            if (scc.size() == 1 && !isSelf){
                Rule rule = datalogProgram.getRules().at(ruleId);
                ruleEvaluationOutput << rule.toString() << endl;
                evaluateRule(rule);
                ruleEvaluationOutput << "1 passes: ";
                sccToString(scc);
                ruleEvaluationOutput << endl;
            } else {
                vector<Rule> rules;
                for (Node node: scc) {
                    int id = node.getID();
                    rules.push_back(datalogProgram.getRules().at(id));
                }
                fixedPointRuleAlgorithm(rules);
                sccToString(scc);
                ruleEvaluationOutput << endl;
            }
        }
        cout << ruleEvaluationOutput.str();

        queryEvaluationOutput << endl << "Query Evaluation" << endl;
        for (Predicate query: datalogProgram.getQueries()) {
            Relation relation = evaluateQuery(query);
            queryToString(query, relation);
        }
        cout << queryEvaluationOutput.str();
    }
    bool static nodeSorter(Node a, Node b) {
        if (a.getID() < b.getID()) {
            return true;
        }
        return false;
    }

    void sccToString (vector<Node> scc) {
        for (int i=0; i<scc.size(); i++) {
            ruleEvaluationOutput << "R" << scc.at(i).getID();
            if (i < scc.size() - 1) {
                ruleEvaluationOutput << ",";
            }
        }
    }

    bool isSelfLoop(Rule rule) {
        Predicate headPredicate = rule.getHeadPredicate();
            vector<Predicate> predicateList = rule.getPredicateList();
            for (Predicate predicate : predicateList) {
                if (predicate.getName() == headPredicate.getName()) {
                    return true;
                }
            }
        return false;
    }

    static Graph makeGraph(const vector<Rule>& rules, bool reverse) {
        Graph graph(rules.size());

        for (int i = 0; i < rules.size(); ++i) {
            Rule rule = rules.at(i);
            for (Predicate predicate : rule.getPredicateList()) {
                for (int j = 0; j < rules.size(); ++j) {
                    Rule newRule = rules.at(j);
                    if (predicate.getName() == newRule.getHeadPredicate().getName()) {
                        if(reverse) {
                            graph.addEdge(j, i);
                        } else {
                            graph.addEdge(i, j);
                        }
                    } else {
                        if(reverse) {
                            graph.setID(j);
                        } else {
                            graph.setID(i);
                        }
                    }

                }
            }
        }

        return graph;

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

        ruleEvaluationOutput << numPasses << " passes: ";
    }
};



#endif //CS236_INTERPRETER_H
