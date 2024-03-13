cmake_minimum_required(VERSION 3.26)
project(cs236)

set(CMAKE_CXX_STANDARD 17)

add_executable(cs236 main.cpp
        Parser.h
        DatalogProgram.h
        Rule.h
        Predicate.h
        Parameter.h
        Scheme.h
        Tuple.h
        Relation.h
        Database.h
        Interpreter.h)
