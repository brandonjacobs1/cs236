//
// Created by Brandon Jacobs on 4/2/24.
//

#ifndef CS236_NODE_H
#define CS236_NODE_H
#include <set>
#include <string>
#include <sstream>

using namespace std;


class Node {

private:

    set<int> adjacentNodeIDs;
    bool visited = false;
    int ID;

public:

    void addEdge(int adjacentNodeID) {
        adjacentNodeIDs.insert(adjacentNodeID);
    }
    int getID() {
        return ID;
    }
    void setID(int id) {
        ID = id;
    }

    set<int> getAdjacentNodeIDs() {
        return adjacentNodeIDs;
    }

    void setVisited() {
        visited = true;
    }
    void resetVisited(){
        visited = false;
    }

    bool isVisited() {
        return visited;
    }

    string toString() {
        stringstream ss;
        for (int i=0; i<adjacentNodeIDs.size(); i++) {
            auto it = next(adjacentNodeIDs.begin(), i);
            ss << "R" << *it;
            if (i < adjacentNodeIDs.size() - 1) {
                ss << ",";
            }
        }

        return ss.str();
    }

};


#endif //CS236_NODE_H
