//
//  stiffness_matrix.cpp
//  scratch
//
//  Created by clemens on 02.02.16.
//
//

#include "mass_interaction.hpp"
#include <iostream>
using namespace std;

MassInteraction::MassInteraction(){};

//void MassInteraction::initMassInteraction() {
	//_stiffness.resize(_n_masses, std::vector<float>(3, 0));
	//_stiffness_mod = _stiffness;
//}

void MassInteraction::modulateGlobalStiffness(float modulation) {
	/*for (int i=0; i<_n_masses; i++) {
		for (int j=0; j<_n_masses; j++) {
			_stiffness_mod[i][j]=_stiffness[i][j]*(1+modulation);
		}
	}*/
}

void MassInteraction::addSpring(int i, int j, float stiffness) {
	vector<float> new_spring = { (float) i, (float) j, stiffness };
	_springs.push_back(new_spring);
}

void MassInteraction::addWall(int i, float stiffness) {
	vector<float> new_wall= { (float) i, stiffness };
	_walls.push_back(new_wall);
}

void MassInteraction::buildUpString(int n_masses, float stiffness) {
	for(int i=0;i<n_masses-1;i++) {
		addSpring(i,i+1, stiffness);
	}
	addWall(0, stiffness);
	addWall(n_masses-1, stiffness);
};

void MassInteraction::buildUpPlate(int x, int y, float stiffness){
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
	addWall(0, stiffness);
	addWall(x*y-1, stiffness);
};

