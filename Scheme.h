//
// Created by Brandon Jacobs on 3/4/24.
//

#ifndef CS236_SCHEME_H
#define CS236_SCHEME_H
using namespace std;
#include <vector>
#include <string>


class Scheme : public vector<string> {

public:
    Scheme() = default;
    Scheme(vector<string> names) : vector<string>(names) { }

};


#endif //CS236_SCHEME_H
