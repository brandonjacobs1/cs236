#include <iostream>
#include "Scanner.h"
#include <vector>
#include "Parser.h"
using namespace std;
#include "Scheme.h"
#include "Interpreter.h"


int main(int argc, char** argv) {
    string fileName = argv[1];
    Scanner scanner = Scanner();
    scanner.scan(fileName);
    Parser parser = Parser(scanner.getTokens());
    parser.parse();
    DatalogProgram program = parser.getProgram();
    Interpreter interpreter = Interpreter(program, Database());
}