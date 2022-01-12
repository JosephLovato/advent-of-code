#include <algorithm>
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <unordered_set>
#include <unordered_map>
#include <bitset>
#include <map>
// #include <math>
using namespace std;

class lantern_fish {
public:
    lantern_fish(int t, int i): timer(t), id(i) {};
    int timer;
    int id;
    operator int() const {return id;}
};

struct lantern_fish_hash {
    inline size_t operator() ( const lantern_fish& e ) const
    {
        static std::hash<size_t> H;
        return H(e.id);
    }
};


class school {
public: 
    static const int new_fish_timer = 8;
    static const int fish_timer = 6;
    unordered_map<unsigned long, unsigned long> fish;
    unsigned long size = 0;
    int day = 0;
    int current_id = 0;

    
    void parse(fstream &file) {
        while(file.good()) {
            string val_s;
            getline(file, val_s, ',');
            fish[stoi(val_s)]++;
        }
        size = fish.size();
    }

    void increment_day() {
        cout << day << "," << size << endl;
        day++;
        //map to hold the next round's fish
        unordered_map<unsigned long, unsigned long> next_fish;
        for(auto f = fish.begin(); f != fish.end(); f++) {
            //if no fish have this timer, skip
            if(f->second == 0) continue;
            //fish timer @ 0
            if(f->first == 0) {
                next_fish[6] += f->second; //put all of these fish into the '6' bin
                next_fish[8] += f->second; //add new fish to the '8' bin
            } else {
                //fish timer > 0, we simply decrement
                next_fish[f->first - 1] += f->second;
            }
        }
        fish = next_fish;

        //update size of school
        unsigned long count = 0;
        for_each(fish.begin(), fish.end(), [&](auto p) { count += p.second; });
        size = count;
    }

};



int main() {
    fstream file("inputs/lantern.txt");
    school s;
    s.parse(file);
    for(int i = 0; i < 256; i++) {
        s.increment_day();
    }
    cout << s.size << endl;
    return 0;
}
