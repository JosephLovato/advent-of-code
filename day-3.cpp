#include <algorithm>
#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_set>
#include <bitset>

using namespace std;

#define BIT_LENGTH 12

int diagnostic(fstream &file)
{
    // read in file to vector of strings
    vector<string> ss;
    int count = 0;
    while (!file.eof())
    {
        count++;
        string line;
        getline(file, line);
        ss.push_back(line);
    }

    // convert to uint16_t array
    uint16_t bb[count];
    for (int i = 0; i < ss.size(); i++)
    {
        bb[i] = stoi(ss[i], nullptr, 2);
    }

    // perform calculations
    int bin_counts[BIT_LENGTH] = {};
    // iterate though all binary numbers
    for (int i = 0; i < count; i++)
    {
        // iterate over each bit
        uint16_t mask = 0x0001;
        for (int j = 0; j < BIT_LENGTH; j++) {
            uint8_t bit = (bb[i] >> j) & mask;
            if(bit) bin_counts[j]++;
        }
    }
    string gamma = "";
    string epsilon = "";
    for(int i = 0; i < BIT_LENGTH; i++) {
        cout << bin_counts[i] << endl;
        if(bin_counts[i] > count/2) {
            gamma = "1" + gamma;
            epsilon = "0" + epsilon;
        } else {
            gamma = "0" + gamma;
            epsilon = "1" + epsilon;
        }
    }
    cout << gamma << ", " << epsilon << endl;
    
    uint16_t g = stoi(gamma, nullptr, 2);
    uint16_t e = stoi(epsilon, nullptr, 2);
    return g*e;
}

string ls_oxy(vector<uint16_t>& bb, int index) {
    //cout << "index: " << index << endl;
    for(uint16_t v: bb) {
        //cout << bitset<BIT_LENGTH>(v).to_string() << endl;
    }
    //cout << "-------" << endl;
    //base case
    if(bb.size() == 1) {
        // << bb[0] << endl;
        return bitset<BIT_LENGTH>(bb[0]).to_string();
    }
    // iterate though all curent binary numbers to find most common digit in farthes
    int curr_idx_bin_count = 0;
    unordered_set<uint16_t> zero_indicies;
    unordered_set<uint16_t> one_indicies;

    //iterate over all binary numbers currently available
    for (int i = 0; i < bb.size(); i++)
    {
        uint16_t mask = 0x0001;
        uint8_t bit = (bb[i] >> index) & mask;
        //keep track of number of 1s and indicies of binary numbers of 0 and 1 at index
        if(bit) {
            curr_idx_bin_count++;
            one_indicies.insert(bb[i]);
        } else {
            zero_indicies.insert(bb[i]);
        }
    }
    string ret = "";
    //cout << curr_idx_bin_count << ": " << bb.size()/2 << endl;
    if (curr_idx_bin_count >= bb.size() - curr_idx_bin_count) {
        ret = "1";
        for(int idx : zero_indicies) {
            bb.erase(find(bb.begin(), bb.end(), idx));
        }
    } else {
        ret = "0";
        for(int idx : one_indicies) {
             bb.erase(find(bb.begin(), bb.end(), idx));
        }
    }
    return ret;
}

string ls_co(vector<uint16_t>& bb, int index) {
    cout << "index: " << index << endl;
    for(uint16_t v: bb) {
        cout << bitset<BIT_LENGTH>(v).to_string() << endl;
    }
    cout << "-------" << endl;
    //base case
    if(bb.size() == 1) {
        //cout << bb[0] << endl;
        return bitset<BIT_LENGTH>(bb[0]).to_string();
    }
    // iterate though all curent binary numbers to find most common digit in farthes
    int curr_idx_bin_count = 0;
    unordered_set<uint16_t> zero_indicies;
    unordered_set<uint16_t> one_indicies;

    //iterate over all binary numbers currently available
    for (int i = 0; i < bb.size(); i++)
    {
        uint16_t mask = 0x0001;
        uint8_t bit = (bb[i] >> index) & mask;
        //keep track of number of 1s and indicies of binary numbers of 0 and 1 at index
        if(bit) {
            curr_idx_bin_count++;
            one_indicies.insert(bb[i]);
        } else {
            zero_indicies.insert(bb[i]);
        }
    }
    string ret = "";
    //cout << curr_idx_bin_count << ": " << bb.size()/2 << endl;
    if (curr_idx_bin_count < bb.size() - curr_idx_bin_count) {
        ret = "1";
        for(int idx : zero_indicies) {
            bb.erase(find(bb.begin(), bb.end(), idx));
        }
    } else {
        ret = "0";
        for(int idx : one_indicies) {
             bb.erase(find(bb.begin(), bb.end(), idx));
        }
    }
    return ret;
}


int life_support(fstream &file)
{
    // read in file to vector of strings
    vector<string> ss;
    int count = 0;
    while (!file.eof())
    {
        count++;
        string line;
        getline(file, line);
        ss.push_back(line);
    }

    // convert to vector of uint16_t
    vector<uint16_t> bb;

    for (int i = 0; i < ss.size(); i++)
    {
        bb.push_back(stoi(ss[i], nullptr, 2));
    }
    vector<uint16_t> bb_c = bb;

    //calculate oxygen
    string oxy = "";
    for(int i = BIT_LENGTH - 1; i >= 0; i--) {
        string result = ls_oxy(bb, i);
        //cout << result << endl;
        if(result.length() > 1) {
            //found base case
            oxy = result;
            break;
        } else {
            oxy += result;
        }
    }
    cout << "oxy: " << oxy << endl;

    //calculate co2
    string co = "";
    for(int i = BIT_LENGTH - 1; i >= 0; i--) {
        string result = ls_co(bb_c, i);
        cout << "result: " << result << endl;
        if(result.length() > 1) {
            //found base case
            co = result;
            break;
        } else {
            co += result;
        }
    }
    cout << "co2: " << co << endl;
    return stoi(oxy, nullptr, 2) * stoi(co, nullptr, 2);
}


int main()
{
    fstream file("inputs/binary.txt");
    cout << life_support(file) << endl;
    return 0;
}
