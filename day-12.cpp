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

#include "graph.h"

using namespace std;

class passages {
   public:
    graph g;
    map<node, node> parent_of;   // map to store parent (value) of the (key)
    vector<vector<node>> paths;  // vector of paths (ordered in each vector)

    passages() { g = graph(); }

    void parse(fstream &file) {
        vector<edge> edges;
        while (file.good()) {
            string n1;
            getline(file, n1, '-');
            string n2;
            getline(file, n2);
            edges.push_back(edge(node(n1), node(n2)));
        }
        g = graph(edges);
    }

    // perform dfs from start to end, adding
    // paths from start to end to a running
    // list (only paths with no duplicate small
    // caves) as the bfs executes
    // vector<vector<node>> dfs() {}

    void dfs_recurse(vector<node> current_path) {
        // capture the current node (last in current path)
        node current = current_path[current_path.size() - 1];

        // base case:
        if (current.id == "end") {
            paths.push_back(current_path);
            return;
        }

        // recursive case: (check each neighbor)
        for (node neighbor : g.neighbors(current)) {
            // - if small, only recurse if not in current_path
            if (neighbor.big ||
                (!neighbor.big && find(current_path.begin(), current_path.end(),
                                       neighbor) == current_path.end())) {
                vector<node> new_path = vector<node>(current_path);
                new_path.push_back(neighbor);
                dfs_recurse(new_path);
            }
        }
    }

    void dfs_recurse_part_two(vector<node> current_path,
                              bool visit_small_cave_twice) {
        // capture the current node (last in current path)
        node current = current_path[current_path.size() - 1];

        // base case:
        if (current.id == "end") {
            paths.push_back(current_path);
            return;
        }

        // recursive case: (check each neighbor)
        for (node neighbor : g.neighbors(current)) {
            // if small cave:
            //   - if hasn't visited it, recurse
            //   - (else) if back at start, DO NOT recurse
            //   - (else) if has visited AND not a small cave twice, recurse
            if (!neighbor.big) {
                if (find(current_path.begin(), current_path.end(), neighbor) ==
                    current_path.end()) {
                    vector<node> new_path = vector<node>(current_path);
                    new_path.push_back(neighbor);
                    dfs_recurse_part_two(new_path, visit_small_cave_twice);
                } else if(neighbor.id == "start") {
                    continue;
                } else if (!visit_small_cave_twice) {
                    vector<node> new_path = vector<node>(current_path);
                    new_path.push_back(neighbor);
                    dfs_recurse_part_two(new_path, true);
                }
            // if big cave: always recurse
            } else {
                vector<node> new_path = vector<node>(current_path);
                new_path.push_back(neighbor);
                dfs_recurse_part_two(new_path, visit_small_cave_twice);
            }
        }
    }

    void print_paths() {
        int num = 0;
        for (auto path : paths) {
            cout << "Path " << num << ": ";
            for (node n : path) {
                cout << n.id << ",";
            }
            num++;
            cout << endl;
        }
    }

    int num_paths() { return paths.size(); }
};

int main() {
    fstream file("inputs/passages.txt");

    passages p;
    p.parse(file);
    // for (node n : p.g.node_list) {
    //     cout << "-- " << n.id << " --" << endl;
    //     for (node x : p.g.neighbors(n)) {
    //         cout << x.id << " " << x.big << endl;
    //     }
    // }
    vector<node> start;
    start.push_back(node("start"));
    p.dfs_recurse_part_two(start, false);
    //p.print_paths();
    cout << p.num_paths() << endl;
    return 0;
}
