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
// #include <math>
using namespace std;

class point {
   public:
    point(int r, int c) : row(r), col(c){};
    int row;
    int col;
};

struct point_equality {
   public:
    bool operator()(const point& p1, const point& p2) const {
        if (p1.col == p2.col && p1.row == p2.row)
            return true;
        else
            return false;
    }
};

struct point_hasher {
   public:
    int width;
    size_t operator()(const point& p) const {
        int v = p.row * 100 + p.col;
        return v;
    }
};

typedef unordered_set<point, point_hasher, point_equality> unordered_point_set;

class smoke_basin {
   public:
    vector<vector<int>> heightmap;  // access using [row][col]
    vector<unordered_set<int>> basins;
    int height;
    int width;

    void parse(fstream& file) {
        while (file.good()) {
            vector<int> row;
            string line;
            getline(file, line);
            for (char c : line) {
                row.push_back(c - '0');
            }
            heightmap.push_back(row);
        }
        height = heightmap.size();
        width = heightmap[0].size();
    }

    bool low_point(int row, int col) {
        int current = heightmap[row][col];
        // waterfall boolean, set to false if not out of bounds and
        // less than or equal to current point
        bool low_point = true;
        // up
        if (row - 1 >= 0 && heightmap[row - 1][col] <= current)
            low_point = false;
        // down
        if (row + 1 < height && heightmap[row + 1][col] <= current)
            low_point = false;
        // right
        if (col + 1 < width && heightmap[row][col + 1] <= current)
            low_point = false;
        // left
        if (col - 1 >= 0 && heightmap[row][col - 1] <= current)
            low_point = false;

        return low_point;
    }

    int risk_level_fun() {
        int total = 0;
        for (int row = 0; row < height; row++) {
            for (int col = 0; col < width; col++) {
                if (low_point(row, col)) total += heightmap[row][col] + 1;
            }
        }
        return total;
    }

    vector<point> find_low_points() {
        vector<point> low_points;
        for (int row = 0; row < height; row++) {
            for (int col = 0; col < width; col++) {
                if (low_point(row, col)) low_points.push_back(point(row, col));
            }
        }
        return low_points;
    }

    bool is_wall(int row, int col) {
        bool is_wall = (row < 0 || col < 0 || row >= height || col >= width ||
                        heightmap[row][col] == 9);
        return is_wall;
    }

    unordered_set<point, point_hasher, point_equality> dfs(point start) {
        unordered_set<point, point_hasher, point_equality>
            visited;     // stores all visited points (and will eventually
                         // hold the points in the basin)
        queue<point> q;  // queue to facilitate DFS
        q.push(start);   // push initial point on queue
        while (!q.empty()) {
            // pop current point and add to visited
            point current = q.front();
            q.pop();
            visited.insert(current);
            // add up, down, right, left if NOT wall and no already visited
            // up
            if (!is_wall(current.row - 1, current.col) &&
                visited.count(point(current.row - 1, current.col)) == 0)
                q.push(point(current.row - 1, current.col));
            // down
            if (!is_wall(current.row + 1, current.col) &&
                visited.count(point(current.row + 1, current.col)) == 0)
                q.push(point(current.row + 1, current.col));
            // right
            if (!is_wall(current.row, current.col + 1) &&
                visited.count(point(current.row, current.col + 1)) == 0)
                q.push(point(current.row, current.col + 1));
            // left
            if (!is_wall(current.row, current.col - 1) &&
                visited.count(point(current.row, current.col - 1)) == 0)
                q.push(point(current.row, current.col - 1));
        }
        return visited;
    }

    int find_basins() {
        // perform BFS (queue) from each low-point, treating 9's as walls
        vector<point> low_points = find_low_points();
        vector<int> basin_sizes;
        for(point p : low_points) {
            unordered_point_set basin = dfs(p);
            basin_sizes.push_back(basin.size());
        }
        sort(basin_sizes.begin(), basin_sizes.end());
        int first_largest = basin_sizes[basin_sizes.size() - 1];
        int second_largest = basin_sizes[basin_sizes.size() - 2];
        int third_largest = basin_sizes[basin_sizes.size() - 3];
        return first_largest * second_largest * third_largest;
    }
};

int main() {
    fstream file("inputs/smoke.txt");
    smoke_basin sb;
    sb.parse(file);
    cout << sb.find_basins() << endl;
    return 0;
}
