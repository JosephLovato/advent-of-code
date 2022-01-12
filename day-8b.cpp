#include <algorithm>
#include <bitset>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <unordered_map>
#include <unordered_set>
#include <vector>
// #include <math>
using namespace std;

class display {
   public:
    const string segments = "abcdefg";
    const vector<int> lengths = {6, 2, 5, 5, 4, 5, 6, 3, 7, 6};
    const vector<string> encodings = {"abcefg",  "cf",    "acdeg",  "acdfg",
                                      "bcdf",    "abdfg", "abdefg", "acf",
                                      "abcedfg", "abcdfg"};
    vector<vector<string>> scrambled;
    vector<vector<string>> output;

    void parse(fstream &file) {
        while (file.good()) {
            string line;
            getline(file, line);
            stringstream ss(line);
            bool before_pipe = true;
            vector<string> s;
            vector<string> o;
            while (ss.good()) {
                string curr;
                ss >> curr;
                if (curr == "|") {
                    before_pipe = false;
                    continue;
                }
                sort(curr.begin(), curr.end());
                if (before_pipe)
                    s.push_back(curr);
                else
                    o.push_back(curr);
            }
            scrambled.push_back(s);
            output.push_back(o);
        }
    }

    int num_of_unique() {
        int count = 0;
        vector<int> unique_digits = {1, 4, 7, 8};
        for (auto v : output) {
            for (auto s : v) {
                for (int u : unique_digits) {
                    if (s.length() == lengths[u]) count++;
                }
            }
        }
        return count;
    }

    unordered_map<string, int> unscramble(vector<string> scrambled) {
        // instantiate vector to hold mappings (index of vector = decoded
        // number)
        vector<string> decodings = vector<string>(10);

        // deduce some of the decodings by unique length
        vector<int> unique_length_digits = {1, 4, 7, 8};
        for (string s : scrambled) {
            for (int u : unique_length_digits) {
                if (s.length() == lengths[u]) {
                    decodings[u] = s;
                }
            }
        }
        cout << "------------" << endl;
        for (int i = 0; i < decodings.size(); i++) {
            cout << i << " : " << decodings[i] << endl;
        }

        // deduce more decodings of the length 6 strings in relation to what we
        // already know length 6 missing a char NOT in strings for 1, 4, 7 MUST
        // be 9 length 6 missing one of the chars in string for 1 MUST be 6 the
        // other length 6 then MUST be 0
        set<char> chars_in_one_four_seven;
        vector<int> one_four_seven = {1, 4, 7};
        for (int n : one_four_seven) {
            for (char c : decodings[n]) chars_in_one_four_seven.insert(c);
        }
        // for each length 6 string, find the missing char
        for (string s : scrambled) {
            if (s.length() == 6) {
                // find which char is missing
                vector<char> diff(10);
                vector<char>::iterator it;
                it = set_difference(segments.begin(), segments.end(), s.begin(),
                                    s.end(), diff.begin());
                diff.resize(it - diff.begin());
                char missing = diff[0];
                // MUST be 9
                if (chars_in_one_four_seven.count(missing) == 0) {
                    decodings[9] = s;
                    // MUST be 6
                } else if (decodings[1].find(missing) != string::npos) {
                    decodings[6] = s;
                    // MUST be 0
                } else {
                    decodings[0] = s;
                }
            }
        }
        cout << "------------" << endl;
        for (int i = 0; i < decodings.size(); i++) {
            cout << i << " : " << decodings[i] << endl;
        }

        // deduce more decodings of the length 5 strings in relation to what we
        // already know length 5 with both chars in the string for 1 MUST be 3
        // length 5 missing two chars from 4 MUST be 2
        // the other length 5 then MUST be 5
        for (string s : scrambled) {
            if (s.length() == 5) {
                // MUST be 3
                if (s.find(decodings[1][0]) != string::npos &&
                    s.find(decodings[1][1]) != string::npos) {
                    decodings[3] = s;
                    continue;
                }
                // find missing chars
                vector<char> diff(10);
                vector<char>::iterator it;
                it = set_difference(segments.begin(), segments.end(), s.begin(),
                                    s.end(), diff.begin());
                diff.resize(it - diff.begin());
                char first_missing = diff[0];
                char second_missing = diff[1];
                // MUST be 2
                if (decodings[4].find(first_missing) != string::npos &&
                    decodings[4].find(second_missing) != string::npos) {
                    decodings[2] = s;
                    // MUST be 5
                } else {
                    decodings[5] = s;
                }
            }
        }
        cout << "------------" << endl;
        for (int i = 0; i < decodings.size(); i++) {
            cout << i << " : " << decodings[i] << endl;
        }

        // build map to output where we key = string
        unordered_map<string, int> output;
        for (int i = 0; i < decodings.size(); i++) {
            output[decodings[i]] = i;
        }
        return output;
    }

    int add() {
        int total = 0;
        // for each scrambled-ouput set
        for (int i = 0; i < scrambled.size(); i++) {
            // decode the scrambled strings
            unordered_map<string, int> decodings = unscramble(scrambled[i]);
            // find number represented by output (4 digits)
            string number = "";
            for (string o : output[i]) {
                number += to_string(decodings[o]);
                cout << stoi(number) << endl;
            }
            total += stoi(number);
        }
        return total;
    }
};

int main() {
    fstream file("inputs/displays.txt");
    display d;
    d.parse(file);
    cout << d.add() << endl;
    return 0;
}
