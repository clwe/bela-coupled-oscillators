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
//#include "adjacency_matrix.hpp"

class StiffnessMatrix {
public:
    StiffnessMatrix(unsigned int n_modes);
    
    inline const unsigned int get_n_modes() const
    { return _n_modes; }
    
    void mat_multiply(float vec[N_MASSES], float (&res)[N_MASSES]);
    
    void buildUpString(void);
  
   
private:
    void clear();
    unsigned int _n_modes;
    float _stiffness[N_MASSES][N_MASSES];

};