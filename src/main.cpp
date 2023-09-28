#include <iostream>
#include <Eigen/Core>
#include <Eigen/Dense>
#include <map>
#include "20233460_오현식.hpp"
//#include "exercise.hpp"

int main() {
  Eigen::Matrix3d state;
//  state<<1,0,0,0,-1,0,0,0,0;
//  state << -1,0,-1,1,-1,1,0,0,1;
//  state <<  -1,1,-1,1,-1,1,1,-1,0;
//    state <<0,0,0,0,0,0,0,0,0;
  state<<0,1,0,0,-1,0,0,0,0;
//  state<<-1,0,-1,1,-1,1,0,0,1;
  std::cout<<"optimal value for state: "<<getOptimalValue(state)<<std::endl;
  return 0;
}

