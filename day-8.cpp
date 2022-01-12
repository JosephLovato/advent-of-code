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

    map<char, char> unscramble(vector<string> scrambled) {
        // build possibilities map (segment -> possible scrambled segments it could be
        // attached to)
        map<char, set<char>> possibilities;
        for (int i = 0; i < segments.length(); i++) {
            set<char> possible;
            for (int j = 0; j < segments.length(); j++) {
                if (i != j) possible.insert(segments[j]);
            }
            possibilities[segments[i]] = possible;
        }
        for (auto p : possibilities) {
            cout << p.first << ": ";
            for (auto c : p.second) {
                cout << c << " ";
            }
            cout << endl;
        }
        // narrow the possibilities by applying length-uniqueness
        vector<int> unique_digits = {1, 4, 7, 8};
        for (string s : scrambled) {
            for (int u : unique_digits) {
                // if digit has same length as one the unique digits,
                // remove all segments from each char that are NOT possible
                // anymore
                if (s.length() == lengths[u]) {
                    // calculate segments not in s (so we can erase them)
                    vector<char> diff(10);
                    vector<char>::iterator it;
                    it = set_difference(segments.begin(), segments.end(),
                                        s.begin(), s.end(), diff.begin());
                    diff.resize(it - diff.begin());

                    // for each segment in this digit
                    for (int i = 0; i < encodings[u].length(); i++) {
                        // update the possible mappings to only {segments in
                        // current string}
                        set<char> curr = possibilities[encodings[u][i]];
                        // for(char c: diff) {
                        //     cout << c << " ";
                        // }
                        // cout << endl;
                        for_each(diff.begin(), diff.end(), [&](char c) {
                            possibilities[encodings[u][i]].erase(c);
                        });
                    }
                }
            }
        }
        cout << "------------" << endl;
        for (auto p : possibilities) {
            cout << p.first << ": ";
            for (auto c : p.second) {
                cout << c << " ";
            }
            cout << endl;
        }
        // narrow the possibilities using deduction
        // length 5: missing from digits of length 5 MUST be (or b f c e)
        vector<char> missing_from_length_five;
        for(string s: scrambled) {
            if(s.length() == 5) {
                // find which segments are missing
                vector<char> diff(10);
                vector<char>::iterator it;
                it = set_difference(segments.begin(), segments.end(),
                                    s.begin(), s.end(), diff.begin());
                diff.resize(it - diff.begin());
                for(char c: diff) missing_from_length_five.push_back(c);
            }
        }
        // find not missing and erase them
        vector<char> diff_f(10);
        vector<char>::iterator it_f;
        it_f = set_difference(segments.begin(), segments.end(),
                            missing_from_length_five.begin(), missing_from_length_five.end(), diff_f.begin());
        diff_f.resize(it_f - diff_f.begin());
        vector<char> ded_f = {'b', 'f', 'c', 'e'};
        for(char d: ded_f) {
            for_each(diff_f.begin(), diff_f.end(), [&](char c) {
                possibilities[d].erase(c);
            });
        }
        cout << "------------" << endl;
        for (auto p : possibilities) {
            cout << p.first << ": ";
            for (auto c : p.second) {
                cout << c << " ";
            }
            cout << endl;
        }

        // length 6: missing from digits of length 6 MUST be (or d e)
        vector<char> missing_from_length_six;
        for(string s: scrambled) {
            if(s.length() == 6) {
                // find which segments are missing
                vector<char> diff(10);
                vector<char>::iterator it;
                it = set_difference(segments.begin(), segments.end(),
                                    s.begin(), s.end(), diff.begin());
                diff.resize(it - diff.begin());
                for(char c: diff) missing_from_length_six.push_back(c);
            }
        }
        // find not missing and erase them
        vector<char> diff(10);
        vector<char>::iterator it;
        it = set_difference(segments.begin(), segments.end(),
                            missing_from_length_six.begin(), missing_from_length_six.end(), diff.begin());
        diff.resize(it - diff.begin());
        vector<char> ded = {'d', 'c', 'e'};
        for(char d: ded) {
            for_each(diff.begin(), diff.end(), [&](char c) {
                possibilities[d].erase(c);
            });
        }
        cout << "------------" << endl;
        for (auto p : possibilities) {
            cout << p.first << ": ";
            for (auto c : p.second) {
                cout << c << " ";
            }
            cout << endl;
        }

        return map<char, char>();
    }
};

int main() {
    fstream file("inputs/displays-ex.txt");
    display d;
    d.parse(file);
    d.unscramble(d.scrambled[6]);
    return 0;
}
