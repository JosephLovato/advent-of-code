#include <algorithm>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

class coords {
public:
    coords() {
        horiz = 0;
        depth = 0;
    }
    int horiz;
    int depth;
    void forward(int arg) { horiz += arg; }
    void down(int arg) { depth += arg; }
    void up(int arg) { depth -= arg;} 
};

class coords_aim {
public:
    coords_aim() {
        horiz = 0;
        depth = 0;
        aim = 0;
    }
    int horiz;
    int depth;
    int aim;
    void forward(int arg) { horiz += arg; depth += aim * arg; }
    void down(int arg) { aim += arg; }
    void up(int arg) { aim -= arg;} 
};

int navigate(fstream &f) {
    coords_aim c;

    //do parsing
    while(!f.eof()) {
        string command;
        string arg;
        f >> command >> arg;
        if(command == "forward") c.forward(stoi(arg));
        else if(command == "up") c.up(stoi(arg));
        else if(command == "down") c.down(stoi(arg));
    }
    return c.depth * c.horiz;
}

int main() {
    fstream file("inputs/commands.txt");
    cout << navigate(file) << endl;
    return 0;
}

