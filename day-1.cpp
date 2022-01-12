#include <algorithm>
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

int sonar_sweep(fstream& file) {

    //read values into vector
    vector<int> depths;
    while(!file.eof()) {
        string s;
        getline(file, s);
        depths.push_back(stoi(s));
    }

    //check how many times a measurement increases from i to i+1
    int count = 0;
    for(int i = 1; i < depths.size(); i++) {
        if(depths[i] > depths[i-1]) count++;
    }

    return count;
}

int sonar_sweep_windowed(fstream& file, int window_size) {
    //read values into vector
    vector<int> depths;
    while(!file.eof()) {
        string s;
        getline(file, s);
        depths.push_back(stoi(s));
    }

    //find all sums of each window
    vector<int> windows;
    for(int i = 0; i < depths.size() - window_size + 1; i++) {
        int sum = 0;
        for(int j = 0; j < window_size; j++) {
            sum += depths[i+j];
            cout << depths[i+j] << " ";
        }
        cout << endl;
        windows.push_back(sum);
    }

    //check how many times a window sum increases from i to i+1
    int count = 0;
    for(int i = 1; i < depths.size(); i++) {
        if(windows[i] > windows[i-1]) count++;
    }

    return count;
}

int main() {
    fstream file("measurments.txt");
    cout << sonar_sweep_windowed(file, 3) << endl;
    return 0;
}

