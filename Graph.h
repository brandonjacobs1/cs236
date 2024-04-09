//
// Created by Brandon Jacobs on 4/2/24.
//

#ifndef CS236_GRAPH_H
#define CS236_GRAPH_H

#include "Node.h"
#include <map>
#include <stack>


class Graph {

private:

    map<int,Node> nodes;

public:

    Graph(int size) {
        for (int nodeID = 0; nodeID < size; nodeID++)
            nodes[nodeID] = Node();
    }

    void addEdge(int fromNodeID, int toNodeID) {
        nodes[fromNodeID].addEdge(toNodeID);
        nodes[fromNodeID].setID(fromNodeID);
    }

    void setID (int id) {
        nodes[id].setID(id);
    }

    string toString() {
        stringstream ss;
        for (int i=0; i<nodes.size(); i++) {
            auto it = next(nodes.begin(), i);
            ss << "R" << it->first << ":" << it->second.toString() << endl;
        }

        return ss.str();
    }

    stack<Node> dfs () {
        stack<Node> postOrderStack;

        for (int i=0; i<nodes.size(); i++) {
            int nodeID = i;
            set<int> adjacentNodeIDs = nodes[nodeID].getAdjacentNodeIDs();
            if (!nodes[nodeID].isVisited()) {
                dfs(nodeID, postOrderStack);
            }
        }
        return postOrderStack;
    }

    int findNode(Node node) {
        for (auto key : nodes) {
            int id = key.second.getID();
            int findID = node.getID();
            if (id == findID){
                return key.second.getID();
            }
        }
        return -1;
    }

    void setNodesNotVisited () {
        for (auto key : nodes) {
            key.second.resetVisited();
        }
    }


    vector<vector<Node>> scc(stack<Node> postOrderStack) {
        vector<vector<Node>> sccs;
        setNodesNotVisited();

        while (!postOrderStack.empty()) {
            Node ad = postOrderStack.top();
            int id = findNode(postOrderStack.top());
            Node node;
            if (id >= 0) {
                node = nodes[id];
            }
            postOrderStack.pop();
            if (!node.isVisited()) {
                vector<Node> scc;
                stack<Node> dfsResult = stack<Node>();
                dfs(node.getID(), dfsResult);
                while (!dfsResult.empty()) {
                    Node sccNode = dfsResult.top();
                    dfsResult.pop();
                    scc.push_back(sccNode);
                }
                sccs.push_back(scc);
            }
        }
        return sccs;
    }


    void dfs(int nodeID, stack<Node>& postOrderStack) {
        nodes[nodeID].setVisited();
        set<int> adjacentNodeIDs = nodes[nodeID].getAdjacentNodeIDs();
        for (int adjacentNodeID : adjacentNodeIDs) {
            if (!nodes[adjacentNodeID].isVisited()) {
                dfs(adjacentNodeID, postOrderStack);
            }
        }
        postOrderStack.push(nodes[nodeID]);
    }
};


#endif //CS236_GRAPH_H
