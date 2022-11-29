
#pragma once

#include "stiffness_matrix.hpp"
#include "defines.hpp"
using namespace std;


class CoupledOscillators
{
public:
	CoupledOscillators();
	
	float spring(float x1, float x2, float stiffness);
	float damper(float x1, float x1_old, float x2, float x2_old, float friction);
	
	float massPosUpdate(float x, float x_old, float force_on_mass, float mass);
	float calculateInputVelocity(float new_audio_input);
	float getAccel(float stiffness, float x_new);
	
	float step(float in, int inputNode);
	float verlet_step(float in, int input_node, int output_node);
private:
	StiffnessMatrix _stiffM;
	float _x_pos[N_MASSES];
	float _x_pos_old[N_MASSES];
	//float _a_old[N_MASSES];
	//float _v_old[N_MASSES];
	float _force[N_MASSES];
	float _friction;
	float _stiffness;
	float _mass;
	float _old_audio_input;
	float _x[N_MASSES];
	float _a[N_MASSES];
	float _v[N_MASSES];
};