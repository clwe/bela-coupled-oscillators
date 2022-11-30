
#pragma once

#include "stiffness_matrix.hpp"
#include "defines.hpp"
using namespace std;


class CoupledOscillators
{
public:
	enum Type {
		PLATE,
    	STRING
	};
	
	CoupledOscillators(Type t, float stiffness, int x, int y=0);
	float verletStep(float in, int input_node, int output_node);
	
	void setType(Type t) {
      _type = t;
	}

	Type getType() const {
		return _type;
	}

private:
	Type _type;
	void getStringAcceleration(float x[N_MASSES], float (&a)[N_MASSES], float stiffness);
	float simpleHighPass(float in);
	StiffnessMatrix _stiffM;
	float _friction;
	float _x[N_MASSES];
	float _a[N_MASSES];
	float _v[N_MASSES];
	float _in_old;
	unsigned int _string_len;
};