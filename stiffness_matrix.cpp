//
//  stiffness_matrix.cpp
//  scratch
//
//  Created by clemens on 02.02.16.
//
//

#include "stiffness_matrix.hpp"

StiffnessMatrix::StiffnessMatrix(unsigned int n_modes):
_n_modes(n_modes)
{
    clear();
    buildUpString();
};

void StiffnessMatrix::clear() {
	for(int i=0; i<_n_modes; i++) {
		for(int j=0;j<_n_modes;j++) {
    	_stiffness[i][j] = 0;
		}
	}
};

void StiffnessMatrix::mat_multiply(float vec[N_MASSES], float (&res)[N_MASSES]) {
	//res[0] = -0.01 * vec[0];
	for(int i=0; i<_n_modes; i++) {
		res[i]=0;
		for(int j=0;j<_n_modes;j++) {
    		res[i] += _stiffness[i][j] * vec[j];
		}
	}
}

void StiffnessMatrix::buildUpString(){
	if (1==_n_modes) {
        _stiffness[0][0]=-0.01;
    }
    else {
        for (int i=0; i<_n_modes; ++i){
            for (int j=0; j<_n_modes; ++j){
                if (i==j){
                     _stiffness[i][j]=0.02;
                }
                if (i==j-1){
                    _stiffness[i][j]=-0.01;
                }
                if (i==j+1){
                    _stiffness[i][j]=-0.01;
                }
            }
        }
    }
    _stiffness[0][0]=0.01;
    _stiffness[_n_modes-1][_n_modes-1]=0.01;
};