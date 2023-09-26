#include <iostream>
#include <Eigen/Core>
#include <Eigen/Dense>

double getOptimalValuerecursive(Eigen::Matrix3d state, int freespaces); //prototype
int checkwin(Eigen::Matrix3d state);

/// DO NOT CHANGE THE NAME AND FORMAT OF THIS FUNCTION
double getOptimalValue(Eigen::Matrix3d state){
    /*
    For this problem, the Bellman Equation boils down to v*(s) = max(a)(gamma*E[v*(s')])
    where gamma = 0.98. That is, v*(s) is the maximum over all actions of the expectation
    of v*(s') given that p(s'|s,a) is uniform over all possible s'.

    We will implement a depth-first recursive method to compute the optimal value for the given state*/

    int freespaces = 0;
    //Find and count number of free spaces
    for(int i=0; i<3; i++){
        for(int j=0; j<3; j++){
            if(state(i,j) == 0){
                freespaces++;
            }
        }
    }

    // return optimal value
    return getOptimalValuerecursive(state, freespaces);
}

//recursive helper function
double getOptimalValuerecursive(Eigen::Matrix3d state, int freespaces){
    double sum;
    double maxsum=0;
    double nextoptimalvalue;
    Eigen::Matrix3d stateaction;
    Eigen::Matrix3d nextstate;


    //check if the game has ended (base case)
    //check rows
    for(int i=0; i<3; i++){
        if(state(i,0) == state(i,1) && state(i,1) == state(i,2)){
            if(state(i,0)==1){
                return 1;   //return 1 if winner
            }
            else if(state(i,0)==-1){
                return 0;   //return 0 if loser
            }
        }
    }
    //check columns
    for(int j=0; j<3; j++){
        if(state(0,j) == state(1,j) && state(1,j) == state(2,j)){
            if(state(0,j)==1){
                return 1;   //return 1 if winner
            }
            else if(state(0,j)==-1){
                return 0;   //return 0 if loser
            }
        }
    }
    //check diagonals-"\"
    if(state(0,0) == state(1,1) && state(1,1) == state(2,2)){
        if(state(0,0)==1){
            return 1;   //return 1 if winner
        }
        else if(state(0,0)==-1){
            return 0;   //return 0 if loser
        }
    }
    //check diagonals-"/"
    if(state(0,2) == state(1,1) && state(1,1) == state(2,0)){
        if(state(0,2)==1){
            return 1;   //return 1 if winner
        }
        else if(state(2,0)==-1){
            return 0;   //return 0 if loser
        }
    }
    //return 0.5 if result is a tie
    if(freespaces==0){
        return 0.5;
    }

    //find the max value function of the next state over all possible actions.
    //Search for possible actions (free spaces)
    for(int i=0; i<3; i++){
        for(int j=0; j<3; j++){
            if(state(i,j) == 0){
                stateaction = state;
                stateaction(i,j) = 1; //do action
                if(checkwin(stateaction)){
                    return 0.98*1; //return 1 discounted by gamma if user wins after action
                }
                else if(freespaces == 1){
                    return 0.98*0.5;  //no more free spaces after action and no win -> tie so return 0.5 discounted by gamma
                }

                //if game is not over, consider all s' for given action and take the sum
                sum = 0;
                //first, search for empty spaces
                for(int a=0; a<3; a++){
                    for(int b=0; b<3; b++){
                        if(stateaction(a,b) == 0){
                            nextstate = stateaction;
                            nextstate(a,b) = -1;  //opponent's move
                            sum = sum + getOptimalValuerecursive(nextstate, freespaces-2);
                        }
                    }
                }

                if(sum>maxsum){
                    maxsum = sum; //update maxsum to get the optimal value*number of s'
                }
            }
        }
    }

    nextoptimalvalue = maxsum/(freespaces-1); //divide by number of s' to get the optimal value of the next state
    return 0.98*nextoptimalvalue; //discount then return
}


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