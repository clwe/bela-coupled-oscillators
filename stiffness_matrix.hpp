//
//  stiffness_matrix.hpp
//  scratch
//
//  Created by clemens on 02.02.16.
//
//

#pragma once
#include <vector>

class StiffnessMatrix {
public:
    StiffnessMatrix();
    
    void setSize(int n_masses);
    void matMultiply(const std::vector<float>& x_vec, std::vector<float> &a_res );
    void addSpring(int i, int j, float stiffness);
    
    void buildUpString(float stiffness);
    void buildUpPlate(int x, int y, float stiffness);
    void modulateGlobalStiffness(float modulation);

private:
    
    void init();
    void clear();
    int _n_masses;
    std::vector<std::vector<float>> _stiffness;
    std::vector<std::vector<float>> _stiffness_mod;
};