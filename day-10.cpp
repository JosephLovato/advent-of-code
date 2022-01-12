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
using namespace std;

class syntax_checker {
   public:
    unordered_map<char, char> char_pairs;
    unordered_map<char, int> scores;
    unordered_map<char, int> completion_scores;
    unordered_set<char> opening_chars = {'<', '[', '{', '('};
    vector<string> lines;

    syntax_checker() {
        // initialize char_pairs and scores
        char_pairs['<'] = '>';
        char_pairs['['] = ']';
        char_pairs['{'] = '}';
        char_pairs['('] = ')';
        scores['>'] = 25137;
        scores[']'] = 57;
        scores['}'] = 1197;
        scores[')'] = 3;
        scores['*'] = 0;
        completion_scores['>'] = 4;
        completion_scores[']'] = 2;
        completion_scores['}'] = 3;
        completion_scores[')'] = 1;
    }

    void parse(fstream& file) {
        while (file.good()) {
            string line;
            getline(file, line);
            lines.push_back(line);
        }
    }
    // Check syntax of one line, returning the fist illegal char if one exists.
    // Else, returns '*'
    char check_syntax_of_line(string line) {
        stack<char> s;
        for (char c : line) {
            // if opening character, push onto stack
            if (opening_chars.count(c) > 0) {
                s.push(c);
            } else {
                // else, if the top of stack does not match
                // the current char, SYNTAX ERROR, return the character
                if (c != char_pairs[s.top()]) return c;
                // if no syntax error, pop and continue
                s.pop();
            }
        }
        // if we reach here, no syntax errors have been found
        return '*';
    }

    // Check syntax of all lines, returning a map of the storing each illegal
    // char and its number of occurrences
    unordered_map<char, int> check_syntax() {
        unordered_map<char, int> occurrences;
        for (string l : lines) {
            occurrences[check_syntax_of_line(l)]++;
        }
        return occurrences;
    }

    int calculate_score(unordered_map<char, int> o) {
        int score = 0;
        for (auto p : o) {
            score += scores[p.first] * p.second;
        }
        return score;
    }

    // find string to complete an incomplete line, if possible
    // else, returns empty string
    string find_completion_of_line(string line) {
        stack<char> s;
        for (char c : line) {
            // if opening character, push onto stack
            if (opening_chars.count(c) > 0) {
                s.push(c);
            } else {
                // else, if the top of stack does not match
                // the current char, SYNTAX ERROR, return empty string
                if (c != char_pairs[s.top()]) return "";
                // if no syntax error, pop and continue
                s.pop();
            }
        }
        // if we reach here, no syntax errors have been found
        // but, the line may be incomplete. We will compile
        // the completion string by looking at the stack
        string completion = "";
        while (!s.empty()) {
            completion += char_pairs[s.top()];
            s.pop();
        }
        return completion;
    }

    vector<string> find_completion() {
        vector<string> all;
        for_each(lines.begin(), lines.end(),
                 [&](string s) { all.push_back(find_completion_of_line(s)); });
        return all;
    }

    long calculate_completion_score() {
        vector<string> completions = find_completion();
        vector<long> scores;
        // for each completion string, calculate score (ignore empty strings)
        for (string s : completions) {
            long score = 0;
            if (s != "") {
                for (char c : s) {
                    score *= 5;
                    score += completion_scores[c];
                }
                scores.push_back(score);
            }
        }
        // from stack overflow:
        // https://stackoverflow.com/questions/1719070/what-is-the-right-approach-when-using-stl-container-for-median-calculation/1719155#1719155
        size_t n = scores.size() / 2;
        nth_element(scores.begin(), scores.begin() + n, scores.end());
        return scores[n];
    }
};

    int main() {
        fstream file("inputs/syntax.txt");
        syntax_checker sc;
        sc.parse(file);
        // cout << sc.calculate_score(sc.check_syntax()) << endl;
        vector<string> c = sc.find_completion();
        for (string s : c) {
            cout << s << endl;
        }
        cout << sc.calculate_completion_score() << endl;

        return 0;
    }
