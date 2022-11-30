//
//  stiffness_matrix.hpp
//  scratch
//
//  Created by clemens on 02.02.16.
//
//

#pragma once
#include <vector>
using namespace std;

class StiffnessMatrix {
public:
    StiffnessMatrix();
    
    void setSize(int n_masses);
    void matMultiply(vector<float> x_vec, vector<float> &a_res );
    void addSpring(int i, int j, float stiffness);
    
    void buildUpString(float stiffness);
    void buildUpPlate(int x, int y, float stiffness);
   
private:
    
    void init();
    void clear();
    int _n_masses;
    vector<vector<float>>  _stiffness;
};