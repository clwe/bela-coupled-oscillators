//
//  stiffness_matrix.hpp
//  scratch
//
//  Created by clemens on 02.02.16.
//
//

#pragma once
#include "defines.hpp"
#include <vector>

class StiffnessMatrix {
public:
	
	
    StiffnessMatrix(float stiffness);
    
    inline const unsigned int getNMasses() const
    { return _n_masses; }
    
    void mat_multiply(float x_vec[N_MASSES], float (&a_res)[N_MASSES]);
    
    void addSpring(int i, int j, float stiffness);
    
    void buildUpString(int length, float stiffness);
    void buildUpPlate(int x, int y, float stiffness);
    inline float getStiffnessVal() {
    	return _stiffnessVal;
    }
   
private:
    void clear();
    unsigned int _n_masses;
    float _stiffness[N_MASSES][N_MASSES];
    float _stiffnessVal;
    
};