//
// Created by Brandon Jacobs on 2/5/24.
//

#ifndef CS236_PARSER_H
#define CS236_PARSER_H

#include "../Scan/Token.h"
#include <utility>
#include <vector>
#include "DatalogProgram.h"
#include "Rule.h"

class Parser {
private:
    vector<Token> tokens;
    DatalogProgram program = DatalogProgram();

public:
    Parser(const vector<Token>& tokens) : tokens(tokens) { }

    [[nodiscard]] TokenType tokenType() const {
        return tokens.at(0).getType();
    }
    void advanceToken() {
        tokens.erase(tokens.begin());
    }
    class ParserException : public std::exception {
    public:
        explicit ParserException(std::string  message) : errorMessage(std::move(message)) {}

        [[nodiscard]] const char* what() const noexcept override {
            return errorMessage.c_str();
        }

    private:
        std::string errorMessage;
    };
    void match(TokenType t) {
        if (t == tokenType()) {
            advanceToken();
        } else {
            throw ParserException("match error");
        }
    }

    DatalogProgram parse() {
        bool isSuccessfulParse = datalogProgram();
        if (isSuccessfulParse) {
            cout << "Success!" << endl;
            cout << program.toString() << endl;
        } else {
            cout << "Failure!" << endl;
            cout << tokens.at(0).toString() << endl;
        }
        return program;
    }

    bool datalogProgram () {
        try {
            if (tokenType() == SCHEMES) {
                match(SCHEMES);
                match(COLON);
                schemeList();
                if (program.getSchemes().empty()) {
                    return false;
                }
                match(FACTS);
                match(COLON);
                factList();
                match(RULES);
                match(COLON);
                ruleList();
                match(QUERIES);
                match(COLON);
                queryList();
                if (program.getQueries().empty()) {
                    return false;
                }
                match(END);

                return true;
            }
        } catch (ParserException& e) {
            return false;
        }
        return false;
    }

    void schemeList () {
        if (tokenType() == ID) {
            program.addScheme(scheme());
            schemeList();
        }
    }

    void factList() {
        if (tokenType() == ID) {
            Predicate predicate = fact();
            program.addFact(predicate);
            factList();
        }
    }

    void ruleList () {
        if (tokenType() == ID) {
            Rule singleRule = rule();
            program.addRule(singleRule);
            ruleList();
        }
    }

    void queryList() {
        if (tokenType() == ID) {
            Predicate predicate = query();
            program.addQuery(predicate);
            queryList();
        }
    }

    Predicate scheme() {
        if(tokenType() == ID) {
            Predicate predicate = Predicate(tokens.at(0).getValue());
            match(ID);
            match(LEFT_PAREN);
            Parameter param = Parameter(tokens.at(0).getValue(), tokenType());
            predicate.addParameter(param);
            match(ID);
            idList(predicate);
            match(RIGHT_PAREN);
            return predicate;
        } else {
            throw ParserException("Expected ID token in scheme");
        }
    }

    Predicate fact() {
        if (tokenType() == ID){
            Predicate predicate = Predicate(tokens.at(0).getValue());
            match(ID);
            match(LEFT_PAREN);
            Parameter param = Parameter(tokens.at(0).getValue(), tokenType());
            predicate.addParameter(param);
            match(STRING);
            stringList(predicate);
            match(RIGHT_PAREN);
            match(PERIOD);
            return predicate;
        } else {
            throw ParserException("Expected ID token in scheme");
        }
    }

    Rule rule() {
        Rule rule = Rule(headPredicate());
        if (tokenType() == COLON_DASH){
            match(COLON_DASH);
            Predicate pred = predicate();
            rule.addPredicate(pred);
            predicateList(rule);
            match(PERIOD);
            return rule;
        } else {
            throw ParserException("No :-");
        }
    }

    Predicate query() {
        Predicate pred = predicate();
        if (tokenType() == Q_MARK) {
            match(Q_MARK);
        }
        return pred;
    }

    Predicate headPredicate () {
        if (tokenType() == ID){
            Predicate predicate(tokens.at(0).getValue());
            match(ID);
            match(LEFT_PAREN);
            Parameter param = Parameter(tokens.at(0).getValue(), tokenType());
            match(ID);
            predicate.addParameter(param);
            idList(predicate);
            match(RIGHT_PAREN);
            return predicate;
        } else {
            throw ParserException("Expected ID token in scheme");
        }
    }

    Predicate predicate() {
        if (tokenType() == ID) {
            Predicate predicate(tokens.at(0).getValue());
            match(ID);
            match(LEFT_PAREN);
            predicate.addParameter(parameter());
            parameterList(predicate);
            match(RIGHT_PAREN);
            return predicate;
        } else {
            throw ParserException("Expected ID token in scheme");
        }
    }

    void predicateList(Rule& rule) {
        if (tokenType() == COMMA) {
            match(COMMA);
            Predicate pred = predicate();
            rule.addPredicate(pred);
            predicateList(rule);
        }
    }

    void parameterList(Predicate& predicate) {
        if (tokenType() == COMMA) {
            match(COMMA);
            Parameter param = parameter();
            predicate.addParameter(param);
            parameterList(predicate);
        }
    }


    void stringList(Predicate& predicate) {
        if (tokenType() == COMMA) {
            match(COMMA);
            Parameter param = Parameter(tokens.at(0).getValue(), tokenType());
            match(STRING);
            predicate.addParameter(param);
            stringList(predicate);
        }
    }

    void idList(Predicate& predicate) {
        if (tokenType() == COMMA) {
            match(COMMA);
            Parameter param = Parameter(tokens.at(0).getValue(), tokenType());
            match(ID);
            predicate.addParameter(param);
            idList(predicate);
        }
    }

    Parameter parameter() {
        if (tokenType() == STRING) {
            Parameter param = {tokens.at(0).getValue(), tokenType()};
            match(STRING);
            return param;
        } else if (tokenType() == ID){
            Parameter param = {tokens.at(0).getValue(), tokenType()};
            match(ID);
            return param;
        } else {
            throw ParserException("parameter");
        }
    }
};

#endif //CS236_PARSER_H
