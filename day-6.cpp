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
    unordered_map<int, lantern_fish*> fish;
    int size = 0;
    int day = 0;
    int current_id = 0;
    ~school() {
        for(auto &p: fish) {
            delete p.second;
        }
    }
    
    void parse(fstream &file) {
        while(file.good()) {
            string val_s;
            getline(file, val_s, ',');
            lantern_fish *f = new lantern_fish(stoi(val_s), current_id);
            fish.insert({current_id, f});
            current_id++;
        }
        size = fish.size();
    }

    void increment_day() {
        cout << day << "," << size << endl;
        day++;
        //vector to hold new baby fish
        vector<pair<int, lantern_fish*>> babies;
        for(auto f = fish.begin(); f != fish.end(); f++) {
            //cout << f->first << " " << f->second->timer << endl;
            //if timer has run out
            if(f->second->timer == 0) {
                //reset timer
                f->second->timer = fish_timer;
                //create new fish with new fish timer 
                lantern_fish* l = new lantern_fish(new_fish_timer, current_id);
                babies.push_back({current_id, l});
                current_id++;
            } else {
                //else, just decrement timer
                f->second->timer -= 1;
            }
        }
        //add babies to school
        for(auto b: babies) {
            fish.insert(b);
        }
        //update size of school
        size = fish.size();
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
