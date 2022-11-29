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

void StiffnessMatrix::mat_multiply(float x[N_MASSES], float (&a)[N_MASSES]) {
	float stiffness = 0.05;
	a[0] = -stiffness * (x[0]-x[1]);
	/*a[1] = -0.01 * (x[1]-x[0]+x[1]-x[2]);
	a[2] = -0.01 * (x[2]-x[1]+x[2]-x[3]);
	a[3] = -0.01 * (x[3]-x[2]+x[3]-x[4]);*/
	
	for (int i=1; i<_n_modes-1; i++){
		a[i] = -stiffness * (-x[i-1]+2*x[i]-x[i+1]);
	}
	a[_n_modes-1] = -stiffness * (x[_n_modes-1]-x[_n_modes-2]);
	//-stiffness*(x2-x1);
	/*for(int i=0; i<_n_modes; i++) {
		res[i]=0;
		for(int j=0;j<_n_modes;j++) {
    		res[i] += _stiffness[i][j] * vec[j];
		}
	}*/
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