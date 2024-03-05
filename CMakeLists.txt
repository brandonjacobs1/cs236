cmake_minimum_required(VERSION 3.26)
project(cs236)

set(CMAKE_CXX_STANDARD 17)

add_executable(cs236 main.cpp
        Parse/Parser.h
        Parse/DatalogProgram.h
        Parse/Rule.h
        Parse/Predicate.h
        Parse/Parameter.h
        Database/Scheme.h
        Database/Tuple.h
        Database/Relation.h)
