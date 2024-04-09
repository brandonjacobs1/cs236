#include <iostream>
#include "Scanner.h"
#include "Parser.h"
using namespace std;
#include "Scheme.h"
#include "Interpreter.h"
#include "Node.h"
#include "Graph.h"


int main(int argc, char** argv) {
    string fileName = argv[1];
    Scanner scanner = Scanner();
    scanner.scan(fileName);
    Parser parser = Parser(scanner.getTokens());
    parser.parse();
    DatalogProgram program = parser.getProgram();
    Interpreter interpreter = Interpreter(program, Database());
    interpreter.run();



    return 0;
}