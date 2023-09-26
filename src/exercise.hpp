#include <iostream>
#include <Eigen/Core>
#include <Eigen/Dense>
#include <algorithm>
#include <random>
#include <utility>
#include <cstdlib>
#include <ctime>
#include <unordered_map>


const int BOARD_SIZE = 3;
const int EMPTY = 0;
const int user = 1.0;
const int opponent = -1.0;
const double REWARD_WIN = 1.0;
const double REWARD_LOSE = 0;
const double REWARD_DRAW = 0.5;
const double DISCOUNT_FACTOR = 0.98;
int userCount;
int opponentCount;

//Eigen::Matrix3d state_addAction;
//Eigen::Matrix3d state_Trasition;
//std::uniform_real_distribution<double> action;
//std::vector<Eigen::Matrix3d> winning_combinations;

Eigen::MatrixXd board = Eigen::MatrixXd::Zero(BOARD_SIZE,BOARD_SIZE);

int checkwin(Eigen::Matrix3d state){
    //check if the game has ended (base case)
    //check rows
    for(int i=0; i<3; i++){
        if(state(i,0)==1 && state(i,0) == state(i,1) && state(i,1) == state(i,2)){
            return 1;   //return 1 if winner
        }
    }
    //check columns
    for(int j=0; j<3; j++){
        if(state(0,j)==1 && state(0,j) == state(1,j) && state(1,j) == state(2,j)){
            return 1;   //return 1 if winner
        }
    }
    //check diagonals-"\"
    if(state(0,0)==1 && state(0,0) == state(1,1) && state(1,1) == state(2,2)){
        return 1;   //return 1 if winner
    }
    //check diagonals-"/"
    if(state(0,2)==1 && state(0,2) == state(1,1) && state(1,1) == state(2,0)){
        return 1;   //return 1 if winner
    }

    return 0; //return 0 if tie or not ended
}

int zeroCount(const Eigen::Matrix3d& state){
    int count = 0;
    for(int i=0; i<BOARD_SIZE; i++){
        for(int j=0; j<BOARD_SIZE; j++){
            if(state(i,j) == 0){
                count += 1;
            }
        }
    }
    return count;
}

int spaceCount(Eigen::Matrix3d state){
    int spaceMaxNum = 0;
    for(int i=0; i<BOARD_SIZE; i++){
        for(int j=0; j<BOARD_SIZE; j++){
            if(state(i,j) == 0)
                spaceMaxNum += 3;
        }
    }
    return spaceMaxNum;
}

//inline std::pair<int,int> action(const Eigen::Matrix3d& currentState,std::string first){
//    ///TODO : 만약에 first가 user였으면 user->opponent 취하고 first가 opponent엿으면 처음에는 user만 두고 그다음은 opponent->user
//    std::vector<std::pair<int,int>> validMoves;
//    for(int i=0; i<BOARD_SIZE; i++){
//        for(int j=0; j<BOARD_SIZE; j++){
//            if(currentState(i,j) == EMPTY)
//                validMoves.push_back(std::make_pair(i,j));
//        }
//    }
//    zeroCount = validMoves.size();
//    srand((unsigned int)time(NULL));
//    int randomIndex = rand() % zeroCount;
//
//    /// put the next pair and then put the next state
////    userCount = (currentState.array() == user).count();
////    opponentCount = (currentState.array() == opponent).count();
////    if(userCount > opponentCount) {
////        currentState(validMoves[randomIndex]) = opponent;
////    }else if(userCount < opponentCount) {
////        currentState(validMoves[randomIndex]) = user;
////    }else{
////        if(user first) currentState(validMoves[randomIndex]) = user;
////        else currentState(validMoves[randomIndex]) = opponent;
////    }
////
//    return validMoves[randomIndex]; //return the pair of next random state (row, column)
//}


bool isGameOver(const Eigen::Matrix3d& state){
    /// Win case
    for(int i=0; i<BOARD_SIZE; i++) {
        if(state(i,0) == user && state(i,1) == user && state(i,2) == user)
            return true;
        if(state(0,i) == user && state(1,i) == user && state(2,i) == user)
            return true;
    }
    if(state(0,0) == user && state(1,1) == user && state(2,2) == user)
        return true;
    if(state(0,2) == user && state(1,1) == user && state(2,0) == user)
        return true;
    /// Lose case
    for(int i=0; i<BOARD_SIZE; i++) {
        if(state(i,0) == opponent && state(i,1) == opponent && state(i,2) == opponent)
            return true;
        if(state(0,i) == opponent && state(1,i) == opponent && state(2,i) == opponent)
            return true;
    }
    if(state(0,0) == opponent && state(1,1) == opponent && state(2,2) == opponent)
        return true;
    if(state(0,2) == opponent && state(1,1) == opponent && state(2,0) == opponent)
        return true;
    /// Ongoing game where one state has not been filled at least
    for(int i=0; i<BOARD_SIZE; i++){
        for(int j=0; j<BOARD_SIZE; j++){
            if(state(i,j) == EMPTY)
                return false; // tic-tac-toe is not over, there are empty spaces
        }
    }
    /// Draw Case where all states are not empty
    return true;
}

double TerminalReward(const Eigen::Matrix3d& state){
    if(isGameOver(state)){
        /// Win case
        for(int i=0; i<BOARD_SIZE; i++) {
            if(state(i,0) == user && state(i,1) == user && state(i,2) == user)
                return REWARD_WIN;
            if(state(0,i) == user && state(1,i) == user && state(2,i) == user)
                return REWARD_WIN;
        }
        if(state(0,0) == user && state(1,1) == user && state(2,2) == user)
            return REWARD_WIN;
        if(state(0,2) == user && state(1,1) == user && state(2,0) == user)
            return REWARD_WIN;
        /// Lose case
        for(int i=0; i<BOARD_SIZE; i++) {
            if(state(i,0) == opponent && state(i,1) == opponent && state(i,2) == opponent)
                return REWARD_LOSE;
            if(state(0,i) == opponent && state(1,i) == opponent && state(2,i) == opponent)
                return REWARD_LOSE;
        }
        if(state(0,0) == opponent && state(1,1) == opponent && state(2,2) == opponent)
            return REWARD_LOSE;
        if(state(0,2) == opponent && state(1,1) == opponent && state(2,0) == opponent)
            return REWARD_LOSE;
        /// Draw Case where all states are not empty
        return REWARD_DRAW;
    }
    return 0;
}

/// TODO Action 하고 step에서 recursive하게 reward찾을 방안생각
//double step(const Eigen::Matrix3d& state, const std::pair<int,int>& action,
//            "TerminalState",
//            "stateTrajectory"){
//    double TerminalReward = TerminalReward(TerminalState);
//    절차 : 받은 state에서 1/(0갯수)의 확률로 0을 1 or -1 (turn에 맞게) 바꾸기
//    그리고 stateTraj[i]에 차곡차곡 저장하
//
//}

double valueIteration_recursive(const Eigen::Matrix3d& state, const int& count){
    double rewardSum;
    double MaxSum = 0;
    Eigen::Matrix3d state_addAction;
    Eigen::Matrix3d state_Trasition;

    if(isGameOver(state)){
        return TerminalReward(state);
    }

    for(int i=0; i<BOARD_SIZE; i++){
        for(int j=0; j<BOARD_SIZE; j++){
            if(state(i,j) == 0){
                state_addAction = state; // due to const state and add a meaning having action
                state_addAction(i,j) = user;
                if(isGameOver(state_addAction)){
                    return DISCOUNT_FACTOR * TerminalReward(state_addAction);
                }
                rewardSum = 0; // if game is not over

                for(int x=0; x<BOARD_SIZE; x++){
                    for(int y=0; y<BOARD_SIZE; y++){
                        if(state_addAction(x,y) == 0){
                            state_Trasition = state_addAction; // set only user element and get other rollout traj.
                            state_Trasition(x,y) = opponent;
                             rewardSum += (1./(count - 1)) * DISCOUNT_FACTOR * valueIteration_recursive(state_Trasition, count-2);
                        }
                    }
                }
                if(rewardSum > MaxSum){
                    MaxSum = rewardSum;
                }
            }
        }
    }
    return MaxSum;
}

/// DO NOT CHANGE THE NAME AND FORMAT OF THIS FUNCTION
double getOptimalValue(Eigen::Matrix3d state){
    int EMPTY_count;
    double value;

    EMPTY_count = zeroCount(state);
    value = valueIteration_recursive(state, EMPTY_count);

    return value; // return optimal value
}