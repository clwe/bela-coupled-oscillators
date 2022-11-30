//
//  stiffness_matrix.cpp
//  scratch
//
//  Created by clemens on 02.02.16.
//
//

#include "stiffness_matrix.hpp"
#include <iostream>


StiffnessMatrix::StiffnessMatrix(float stiffness):
_n_masses(N_MASSES),_stiffnessVal(stiffness)
{
    clear();
};

void StiffnessMatrix::clear() {
	for(int i=0; i<_n_masses; i++) {
		for(int j=0;j<_n_masses;j++) {
    		_stiffness[i][j] = 0;
		}
	}
};

void StiffnessMatrix::addSpring(int i, int j, float stiffness) {
	_stiffness[i][i] += stiffness;
	_stiffness[j][j] += stiffness;
	_stiffness[i][j] -= stiffness;
	_stiffness[j][i] -= stiffness;
}

void StiffnessMatrix::mat_multiply(float x[N_MASSES], float (&a)[N_MASSES]) {

	for (int i=0; i<_n_masses; i++) {
		a[i] = 0;
		for (int j=0; j<_n_masses; j++) {
			a[i] += -_stiffness[i][j] * x[j];
		}
	}
}

void StiffnessMatrix::buildUpString(int length, float stiffness) {
	if (length>_n_masses) {
		std::cout << "Not enough masses reserved for this length of string!" << std::endl;
		return;
	}
	clear();
	if (1==_n_masses) {
        _stiffness[0][0]=-stiffness;
    }
    else {
        for (int i=0; i<length-1; ++i) {
        	addSpring(i, i+1, stiffness);
        }
    }
};

void StiffnessMatrix::buildUpPlate(int x, int y, float stiffness){
	if (x*y>_n_masses) {
		std::cout << "Not enough masses reserved for this size of a plate!" << std::endl;
		return;
	}
	clear();
	if (1==_n_masses) {
        _stiffness[0][0]=-stiffness;
    }
    else {
		for (int i=0; i<y-1; ++i) {
			for (int j=0; j<x-1; ++j) {
				// horizontal connections
				addSpring(j+i*y, j+1+i*y, stiffness);
			}
		}
		for (int i=0; i<y-1; ++i) {
			for (int j=0; j<x-1; ++j) {
				// vertical connections
				addSpring(j+i*x, j+(i+1)*x, stiffness);
			}
		}
    }
};