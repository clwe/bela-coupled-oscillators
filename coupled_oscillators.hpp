
#pragma once

#include "stiffness_matrix.hpp"
#include <vector>
using namespace std;

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
	float verletStep(float in, int input_node, int output_node);

private:
	COConfig _conf;
	void getStringAcceleration(vector<float> x, vector<float> &a);
	float simpleHighPass(float in);
	StiffnessMatrix _stiff_M;
	vector<float> _x;
	vector<float> _a;
	vector<float> _v;
	vector<float> _a_new;
	float _in_old;
	unsigned int _n_masses;
};