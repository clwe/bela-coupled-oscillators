//
//  mass_interaction.hpp
//  bela-coupled-oscillators
//
//  Created by clemens on 02.12.22.
//
//

#pragma once
#include <vector>

class MassInteraction {
public:
    MassInteraction();
    

    void addSpring(int i, int j, float stiffness);
    void addWall(int i, float stiffness);

    
    void buildUpString(int n_masses, float stiffness);
    void buildUpPlate(int x, int y, float stiffness);
    void modulateGlobalStiffness(float modulation);
    
    void getAcceleration(const std::vector<float> &x, std::vector<float> &a) {
    	
		std::fill(a.begin(), a.end(), 0);
		int n_springs = _springs.size();
		for (int idx=0; idx<n_springs; idx++){
			int i = _springs[idx][0];
			int j = _springs[idx][1];
			float stiffness = _springs[idx][2];
			float accel = -stiffness * (x[i]-x[j]);
			a[i] += accel;
			a[j] -= accel;
		}
		int n_walls = _walls.size();
		for (int idx=0; idx<n_walls; idx++){
			int i = _walls[idx][0];
			float stiffness = _walls[idx][1];
			float accel = -stiffness * x[i];
			a[i] += accel;
		}
	}

private:
    void clear();
    int _n_masses;
    std::vector<std::vector<float>> _springs;
    std::vector<std::vector<float>> _walls;
};