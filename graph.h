#include <algorithm>
#include <bitset>
#include <fstream>
#include <iostream>
#include <map>
#include <queue>
#include <set>
#include <sstream>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <functional>
#include "point.h"
#include <string>


using namespace std;

class node {
public:
    node(string s) {
        id = s;
        big = isupper(s[0]);
    }
    string id;
    bool big;
};

bool operator==(const node& a, const node& b) {
    return a.id == b.id;
}

struct node_equality {
   public:
    bool operator()(const node& n1, const node& n2) const {
        if (n1.id == n2.id)
            return true;
        else
            return false;
    }
};

struct node_hasher {
   public:
    size_t operator()(const node& n) const {
        hash<string> h;
        return h(n.id);
    }
};

typedef pair<node, node> edge;


class graph {
public:
    unordered_set<node, node_hasher, node_equality> node_list;
    unordered_map<node, vector<node>, node_hasher, node_equality> adjacency_list;

    graph() {};

    graph(vector<edge> edges) {
        for(edge e: edges) {
            node_list.insert(e.first);
            node_list.insert(e.second);
            adjacency_list[e.first].push_back(e.second);
            adjacency_list[e.second].push_back(e.first);
        }
    }

    vector<node> neighbors(node n) {
        return adjacency_list[n];
    }
};