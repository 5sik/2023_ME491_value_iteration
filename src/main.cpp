#include <iostream>
#include <Eigen/Core>
#include <Eigen/Dense>
#include <map>
#include "student_id.hpp"
//#include "1111student_id.hpp"
//#include "exercise.hpp"

int main() {
  Eigen::Matrix3d state;
//  state<<1,0,0,0,-1,0,0,0,0;
//  state << -1,0,-1,1,-1,1,0,0,1;
//  state <<  -1,1,-1,1,-1,1,1,-1,0;
//    state <<0,1,0,0,-1,0,0,0,0;
  state<<0,0,0,0,0,0,0,0,0;
//  state<<-1,0,-1,1,-1,1,0,0,1;
  std::cout<<"optimal value for state: "<<getOptimalValue(state)<<std::endl;
  return 0;
}

//int main() {
//    Eigen::Matrix3d state;
//    Eigen::Vector3d sex;
//    std::map<Eigen::Vector3d, int> park;
////    state<<1,1,1,0,-1,0,0,0,0;
//    sex << 1,2,3;
//    park[sex]=3;
////    count = (state.array() == 1.0).count();
//    std::cout<<park(sex)<<std::endl;
//    return 0;
//}
