
#pragma once

#include "stiffness_matrix.hpp"
#include <vector>

enum Type {
		PLATE,
    	STRING
	};

typedef struct {
	Type type;
	int x;
	int y;
	int len;
	float stiffness;
	float damping;
} COConfig;


class CoupledOscillators
{
public:
	CoupledOscillators(COConfig conf);
	void modulateGlobalStiffness(float modulation);
	
	float step(float in, int input_node, int output_node) {
	   	for (int i=0; i<_n_masses; i++) {
	   		float dx = _v[i] + 0.5 * _a[i];
	   		_x[i] += dx;
	   		if(i==input_node) {
	   			_x[i] += simpleHighPass(in);
	   		}
	   	}
	   	
	   	if(_conf.type == STRING) {
	   		getStringAcceleration(_x, _a_new);
	   	}
	   	else {
	   		_stiff_M.matMultiply(_x, _a_new);
	   	}
	   	
	   	for (int i=0; i<_n_masses; i++) {
	   		float dv = 0.5 * (_a[i] + _a_new[i]);
	   		_v[i] += dv;
	   		_a[i] = _a_new[i];
	   		_v[i] += -2*(_conf.damping * _v[i]);
	   	}
	   	return _x[output_node];
	}



private:
	COConfig _conf;
	float _mod_stiffness;
	float simpleHighPass(float in);
	StiffnessMatrix _stiff_M;
	std::vector<float> _x;
	std::vector<float> _a;
	std::vector<float> _v;
	std::vector<float> _a_new;
	float _in_old;
	unsigned int _n_masses;
	
	void getStringAcceleration(const std::vector<float> &x, std::vector<float> &a) {
		if(_n_masses==1) {
			a[0] = -_mod_stiffness * x[0];
			return;
		}
		a[0] = -_mod_stiffness * (x[0]-x[1]);
		
		for (int i=1; i<_n_masses-1; i++){
			a[i] = -_mod_stiffness * (-x[i-1]+2*x[i]-x[i+1]);
		}
		a[_n_masses-1] = -_mod_stiffness * (x[_n_masses-1]-x[_n_masses-2]);
	}
};