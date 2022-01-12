#include <algorithm>
#include <bitset>
#include <fstream>
#include <iostream>
#include <sstream>
#include <unordered_set>
#include <vector>

#define BINGO_WIDTH 5
#define BINGO_HEIGHT 5
#define BINGO_SIZE (BINGO_WIDTH * BINGO_HEIGHT)

using namespace std;

class square {
   public:
    square(int n, int r, int c) : number(n), row(r), col(c), marked(false){};
    int number;
    int row;
    int col;
    bool marked;
};

class bingo_card {
   public:
    // constructor
    bingo_card(vector<vector<square>> b) : board(b){};
    bingo_card() {};

    // member variable to store bingo card numbers
    vector<vector<square>> board;

    // helper functions
    int sum_of_unmarked() {
        int sum = 0;
        for (auto row : board) {
            for (square sq : row) {
                if (!sq.marked) sum += sq.number;
            }
        }
        return sum;
    }

    bool is_winner() {
        // check each row
        for (auto row : board) {
            if (all_of(row.begin(), row.end(),
                       [](square sq) { return sq.marked; }))
                return true;
        }
        // check each column
        for (int col = 0; col < board.size(); col++) {
            bool all_marked = true;
            for (int row = 0; row < board[0].size(); row++) {
                if (!board[row][col].marked) all_marked = false;
            }
            if (all_marked) return true;
        }

        // no columns or rows had BINGO's
        return false;
    }

    void mark_called_square(int called_num) {
        // look through entire board and mark any squares with the called number
        for (auto &row : board) {
            for (square &sq : row) {
                if (sq.number == called_num) {
                    // cout << "MARKING " << sq.row << "," << sq.col << ": "
                    //      << sq.number << endl;
                    sq.marked = true;
                }
            }
        }
    }
};

class bingo_hall {
   public:
    // constructors
    bingo_hall(vector<bingo_card> bc, vector<int> b) : cards(bc), balls(b){};
    bingo_hall() {};

    // member variables
    vector<bingo_card> cards;
    vector<int> balls;
    int iteration = 0;
    unordered_set<int> won_boards; //stores indicides of boards that have won relative to the cards vector

    // helper functions
    // call current number and mark all boards accordingly
    void call() {
        for (bingo_card& card : cards) {
            card.mark_called_square(balls[iteration]);
        }
        iteration++;
    }

    bool any_winners() {
        cout << any_of(cards.begin(), cards.end(), [](bingo_card card) {
            cout << card.is_winner() << ", ";
            return card.is_winner();
        }) << endl;
        return any_of(cards.begin(), cards.end(),
                      [](bingo_card card) { return card.is_winner(); });
    }

    // returns the winning card. Must only be called if a winner exists
    bingo_card find_winner() {
        for (bingo_card card : cards) {
            if (card.is_winner()) return card;
        }
    }

    // calculate the final score given a winning bingo card
    int final_score(bingo_card winner) {
        return balls[--iteration] * winner.sum_of_unmarked();
    }

    bingo_card note_winners() {
        bingo_card winner;
        for (int i = 0; i < cards.size(); i++) {
            if (cards[i].is_winner()) {
                if(won_boards.count(i) == 0) winner = cards[i];
                won_boards.insert(i);
            }
        }
        return winner;
    }

    
};

int main() {
    // parse input iflie
    fstream file("inputs/bingo.txt");
    // get bingo ball order
    string s_balls;
    getline(file, s_balls);
    stringstream bs(s_balls);
    vector<int> balls;
    while (!bs.eof()) {
        string s_num;
        getline(bs, s_num, ',');
        balls.push_back(stoi(s_num));
    }

    // get bingo cards
    vector<bingo_card> cards;
    while (!file.eof()) {
        vector<vector<square>> board(
            BINGO_HEIGHT, vector<square>(BINGO_WIDTH, square(0, 0, 0)));
        for (int i = 0; i < BINGO_HEIGHT; i++) {
            for (int j = 0; j < BINGO_WIDTH; j++) {
                string s_num;
                file >> s_num;
                board[i][j] = square(stoi(s_num), i, j);
            }
        }
        cards.push_back(bingo_card(board));
    }

    bingo_hall game(cards, balls);
    bingo_card last;
    while (game.won_boards.size() < game.cards.size()) {
        cout << "Calling iteration " << game.iteration << endl;
        game.call();
        last = game.note_winners();
        cout << game.won_boards.size() << "/" << game.cards.size() << endl;
    }

    // for(int i = 0; i < game.cards.size(); i++) {
    //     if(game.won_boards.count(i) == 0) {
    //         last = game.cards[i];
    //     }
    // }
    //cout << last.board[0][0].number << endl;
    cout << last.sum_of_unmarked() << endl;
    cout << game.final_score(last) << endl;
    return 0;
}
