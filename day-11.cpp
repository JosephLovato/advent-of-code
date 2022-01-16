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
#include "point.h"

using namespace std;

typedef unordered_set<point<uint8_t>, point_hasher<uint8_t>, point_equality<uint8_t>> unordered_point_set;

class octopi_array {
   public:
    vector<vector<uint8_t> >
        array;  // save some space with uint8_t (no need for 4 byte int)
    int width;
    int height;
    int total_flashes = 0;

    octopi_array(){};

    void parse(fstream& file) {
        while (file.good()) {
            vector<uint8_t> row;
            string line;
            getline(file, line);
            for (char c : line) {
                row.push_back(c - '0');
            }
            array.push_back(row);
        }
        width = array[0].size();
        height = array.size();
    }

    void print() {
        for (int i = 0; i < array.size(); i++) {
            for (int j = 0; j < array[i].size(); j++) {
                cout << (int)array[i][j] << ",";
            }
            cout << endl;
        }
    }

    // Are there any octopi that are flashing (> 9)
    // that haven't already flashed this step?
    bool octopi_flash_p(unordered_point_set& already_flashed) {
        for (int i = 0; i < array.size(); i++) {
            for (int j = 0; j < array[i].size(); j++) {
                if(array[i][j] > 9 && already_flashed.count(point<uint8_t>(i, j)) == 0) {
                    return true;
                }
            }
        }
        return false;
    }

    // "flash" the octopi @ (row, col) by incrementing the
    // energy levels of the octopi around it (8 possible)
    void flash(int row, int col) {
        total_flashes++;
        array[row][col]++;
        // north
        if(row - 1 >= 0) array[row - 1][col]++;
        // northeast
        if(row - 1 >= 0 && col + 1 < width) array[row - 1][col + 1]++;
        // east
        if(col + 1 < width) array[row][col + 1]++;
        // southeast
        if(row + 1 < height && col + 1 < width) array[row + 1][col + 1]++;
        // south
        if(row + 1 < height) array[row + 1][col]++;
        // southwest
        if(row + 1 < height && col - 1 >= 0) array[row + 1][col - 1]++;
        // west
        if(col - 1 >= 0) array[row][col - 1]++;
        // northwest
        if(row - 1 >= 0 && col - 1 >= 0) array[row - 1][col -1]++;
    }

    // take one step, updating octopi energy levels accordingly
    // (this includes the sub-step iterations)
    // for part 2: returns true if all octopi flashed in this step
    bool step() {
        // 1. increase energy level of every octopus
        for (int i = 0; i < array.size(); i++) {
            for (int j = 0; j < array[i].size(); j++) {
                array[i][j]++;
            }
        }
        // 2. flash octopi with energy level > 9
        // while octopi are flashing, perform the necessary
        // increase in energy levels, and repeat
        unordered_point_set flashed; // to store points that have flashed
        while(octopi_flash_p(flashed)) {
            for (int i = 0; i < array.size(); i++) {
                for (int j = 0; j < array[i].size(); j++) {
                    if(array[i][j] > 9 && flashed.count(point<uint8_t>(i, j)) == 0) {
                        flash(i, j);
                        flashed.insert(point<uint8_t>(i, j));
                    }
                }
            }
        }

        // 3. set all flashed octopi to energy level 0
        for(point<uint8_t> p : flashed) {
            array[p.row][p.col] = 0;
        }

        return flashed.size() == width * height;
    }
};

int main() {
    fstream file("inputs/octopi.txt");
    octopi_array o;
    o.parse(file);

    for(int i = 0; i < 500; i++) {
        if(o.step()) cout << "All flashed after step: " << i + 1 << endl;
    }
    cout << "TOTAL: " << o.total_flashes << endl;

    return 0;
}
