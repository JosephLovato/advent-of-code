#include <algorithm>
#include <bitset>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <unordered_map>
#include <unordered_set>
#include <vector>
// #include <math>
using namespace std;

class crabs {
   public:
    unordered_map<int, int> crab_map;  // holds {start_pos, quantity of crabs}
    int max;
    vector<int> fuel_costs; //index by distance, get fuel cost

    void parse(fstream& file) {
        while (file.good()) {
            string val_s;
            getline(file, val_s, ',');
            crab_map[stoi(val_s)]++;
        }
    }

    void init() {
        // calculate max position
        vector<int> crabs;
        for_each(crab_map.begin(), crab_map.end(),
            [&](auto& p) { crabs.push_back(p.first); });
        max = *max_element(crabs.begin(), crabs.end());

        // calculate fuel_costs (ahead of time, so we don't have to 
        // waste time on the CPU doing this everytime)
        for(int i = 0; i <= max; i++) {
            //basically just geometric series
            fuel_costs.push_back(((i+1) * i ) / 2);
        }
    }

    int minimize_fuel() {
        int min_pos = 0;
        int min_fuel = INT_MAX;
        for (int i = 0; i <= max; i++) {
            int curr_fuel = 0;
            for_each(crab_map.begin(), crab_map.end(),
                     [&](auto& p) { curr_fuel += fuel_costs[abs(i - p.first)]*p.second; });
            if(curr_fuel < min_fuel) {
                min_fuel = curr_fuel;
                min_pos = i;
            }
        }
        return min_fuel;
    }
};

int main() {
    fstream file("inputs/crabs.txt");
    crabs c;
    c.parse(file);
    c.init();
    cout << c.minimize_fuel() << endl;
    return 0;
}
