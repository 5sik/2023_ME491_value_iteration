//
// Created by ohsik on 23. 9. 19.
//

#ifndef ME_491_VI_WITHGPT_HPP
#define ME_491_VI_WITHGPT_HPP

#include <iostream>
#include <Eigen/Core>
#include <Eigen/Dense>

using namespace std;
using namespace Eigen;

const int BOARD_SIZE = 3;
const int EMPTY = 0;
const int X = 1;
const int O = 2;
const double REWARD_WIN = 1.0;
const double REWARD_LOSE = -1.0;
const double REWARD_DRAW = 0.0;
const double DISCOUNT_FACTOR = 0.9;

MatrixXi board(BOARD_SIZE, BOARD_SIZE);

MatrixXi winning_combinations[8] = {
        MatrixXi(3, 2) << 0, 0, 0, 1, 0, 2,
        MatrixXi(3, 2) << 1, 0, 1, 1, 1, 2,
        MatrixXi(3, 2) << 2, 0, 2, 1, 2, 2,
        MatrixXi(3, 2) << 0, 0, 1, 0, 2, 0,
        MatrixXi(3, 2) << 0, 1, 1, 1, 2, 1,
        MatrixXi(3, 2) << 0, 2, 1, 2, 2, 2,
        MatrixXi(3, 2) << 0, 0, 1, 1, 2, 2,
        MatrixXi(3, 2) << 0, 2, 1, 1, 2, 0
};

bool is_valid_move(int row, int col) {
    return row >= 0 && row < BOARD_SIZE && col >= 0 && col < BOARD_SIZE && board(row, col) == EMPTY;
}

bool is_board_full() {
    return (board.array() == EMPTY).count() == 0;
}

int check_winner() {
    for (int i = 0; i < 8; ++i) {
        if ((board(winning_combinations[i](0, 0), winning_combinations[i](0, 1)) == board(winning_combinations[i](1, 0), winning_combinations[i](1, 1))) &&
            (board(winning_combinations[i](1, 0), winning_combinations[i](1, 1)) == board(winning_combinations[i](2, 0), winning_combinations[i](2, 1))) &&
            (board(winning_combinations[i](0, 0), winning_combinations[i](0, 1)) != EMPTY)) {
            return board(winning_combinations[i](0, 0), winning_combinations[i](0, 1));
        }
    }
    if (is_board_full()) {
        return EMPTY;
    }
    return 0;
}

MatrixXi value_iteration() {
    MatrixXd value_function(BOARD_SIZE, BOARD_SIZE);
    value_function.setZero();
    double delta;

    do {
        delta = 0;
        for (int i = 0; i < BOARD_SIZE; ++i) {
            for (int j = 0; j < BOARD_SIZE; ++j) {
                if (board(i, j) == EMPTY) {
                    double max_value = -1e9;
                    for (int row = 0; row < BOARD_SIZE; ++row) {
                        for (int col = 0; col < BOARD_SIZE; ++col) {
                            if (is_valid_move(row, col)) {
                                board(row, col) = X;
                                double new_value = calculate_value(X, value_function);
                                max_value = max(max_value, new_value);
                                board(row, col) = EMPTY;
                            }
                        }
                    }
                    delta = max(delta, abs(max_value - value_function(i, j)));
                    value_function(i, j) = max_value;
                }
            }
        }
    } while (delta > 1e-6);

    MatrixXi best_action(1, 2);
    double best_value = -1e9;

    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            if (board(i, j) == EMPTY) {
                board(i, j) = X;
                double new_value = calculate_value(X, value_function);
                if (new_value > best_value) {
                    best_value = new_value;
                    best_action << i, j;
                }
                board(i, j) = EMPTY;
            }
        }
    }

    return best_action;
}

double calculate_value(int player, MatrixXd& value_function) {
    int winner = check_winner();
    if (winner == X) {
        return REWARD_WIN;
    } else if (winner == O) {
        return REWARD_LOSE;
    } else if (winner == EMPTY) {
        return REWARD_DRAW;
    }
    return 0.0;
}

void print_board() {
    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            if (board(i, j) == X) {
                cout << "X ";
            } else if (board(i, j) == O) {
                cout << "O ";
            } else {
                cout << ". ";
            }
        }
        cout << endl;
    }
}


#endif //ME_491_VI_WITHGPT_HPP
