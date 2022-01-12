#include <algorithm>
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <unordered_set>
#include <bitset>
#include <map>
// #include <math>
using namespace std;

class point {
public:
    point(int x_i, int y_i) : x(x_i), y(y_i) {};
    int x;
    int y;
};

class diagram {
public: 

    vector<vector<int>> space;
    vector<pair<point, point>> line_segments;

    //parse input file into line_segments
    void parse(fstream &file) {
        while(!file.eof()) {
            string p1_s;
            string p2_s;
            file >> p1_s;
            file >> p2_s >> p2_s;
            stringstream p1_r(p1_s);
            stringstream p2_r(p2_s);
            string p1_x_s;
            string p1_y_s;
            string p2_x_s;
            string p2_y_s;
            getline(p1_r, p1_x_s, ',');
            getline(p1_r, p1_y_s);
            getline(p2_r, p2_x_s, ',');
            getline(p2_r, p2_y_s);
            line_segments.push_back(pair(point(stoi(p1_x_s), stoi(p1_y_s)), point(stoi(p2_x_s), stoi(p2_y_s))));
        }
    }

    void paint() {
        space = vector<vector<int>>(1000, vector<int>(1000, 0));
        for(auto line: line_segments) {
            if(line.first.x == line.second.x) {
                if(line.first.y < line.second.y) {
                    for(int y = line.first.y; y <= line.second.y; y++) {
                        space[line.first.x][y]++;
                    }
                } else {
                    for(int y = line.second.y; y <= line.first.y; y++) {
                        space[line.first.x][y]++;
                    }
                }
            } else if(line.first.y == line.second.y) {
                if(line.first.x < line.second.x) {
                    for(int x = line.first.x; x <= line.second.x; x++) {
                        space[x][line.second.y]++;
                    } 
                } else {
                    for(int x = line.second.x; x <= line.first.x; x++) {
                        space[x][line.second.y]++;
                    } 
                }
            } else {
                int x_step = 0;
                int y_step = 0;
                if(line.first.y < line.second.y) {
                    y_step = 1;
                } else {
                    y_step = -1;
                }
                if(line.first.x < line.second.x) {
                    x_step = 1;
                } else {
                    x_step = -1;
                }
                int x = line.first.x;
                int y = line.first.y;
                while(x - line.second.x != x_step || y - line.second.y != y_step) {
                    space[x][y]++;
                    x += x_step;
                    y += y_step;
                }
            }
        }
        for(int y = 0; y < 10; y++) {
            for(int x = 0; x < 10; x++) {
                cout << space[x][y] << " ";
            }
            cout << endl;
        }
    }

    int covered(int quantity) {
        int count = 0;
        for(int x = 0; x < space[0].size(); x++) {
            for(int y = 0; y < space.size(); y++) {
                if(space[x][y] >= 2) count++;
            }
        }
        return count;
    }
};



int main() {
    fstream file("inputs/hydro.txt");
    //cout << life_support(file) << endl;
    diagram d;
    d.parse(file);
    d.paint();
    cout << d.covered(2) << endl;
    return 0;
}
